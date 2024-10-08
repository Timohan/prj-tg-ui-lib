/*!
 * \file
 * \brief file tg_image_private.cpp
 *
 * it holds general TgImagePrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_image_private.h"
#include "../../global/tg_global_application.h"
#include "../../global/tg_global_log.h"
#include "../tg_item2d.h"
#include "../../shader/tg_shader_2d.h"
#include "../../window/tg_mainwindow_private.h"
#include "../../global/private/tg_global_wait_renderer.h"
#include "item2d/tg_item2d_position.h"
#include "../../image/draw/tg_image_draw.h"

TgImagePrivate::TgImagePrivate(const char *filename) :
    m_topLeftS(0), m_topLeftT(0),
    m_topRightS(1), m_topRightT(0),
    m_bottomRightS(1), m_bottomRightT(1),
    m_bottomLeftS(0), m_bottomLeftT(1),
    m_initVerticesDone(false),
    m_initImageAssetDone(false)
{
    TG_FUNCTION_BEGIN();
    m_imageAsset.m_textureIndex = 0;
    m_imageAsset.m_type = TgImageType::LoadedImage;
    m_imageAsset.m_filename = filename;
    TG_FUNCTION_END();
}

TgImagePrivate::~TgImagePrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePrivate::getTextureIndex
 *
 * gets texture index for this rectangle
 * if texture index is not done before, it tries to create it
 *
 * \return texture index
 */
GLuint TgImagePrivate::getTextureIndex()
{
    TG_FUNCTION_BEGIN();
    if (m_imageAsset.m_textureIndex) {
        TG_FUNCTION_END();
        return m_imageAsset.m_textureIndex;
    }
    m_imageAsset.m_textureIndex = TgGlobalApplication::getInstance()->getImageAssets()->generateImage(m_imageAsset);
    TG_FUNCTION_END();
    return m_imageAsset.m_textureIndex;
}

/*!
 * \brief TgImagePrivate::init
 *
 * inits the vertices
 * Generates the texture with function getTextureIndex()
 * Generates the vertices
 *
 * \return true on success
 */
bool TgImagePrivate::init()
{
    TG_FUNCTION_BEGIN();
    Vertice vertices[4];
    generateVertices(vertices);
    TG_FUNCTION_END();
    return TgRender::init(vertices, 4);
}

/*!
 * \brief TgImagePrivate::setTextureST
 *
 * set texture ST values
 *
 * All values must be <= 1.0f (or at least should be)
 *
 * \param topLeftS topleft S
 * \param topLeftT topleft T
 * \param topRightS topRight S
 * \param topRightT topRight T
 * \param bottomRightS bottomRight S
 * \param bottomRightT bottomRight T
 * \param bottomLeftS bottomLeft S
 * \param bottomLeftT bottomLeft T
 */
void TgImagePrivate::setTextureST(float topLeftS, float topLeftT,
                                  float topRightS, float topRightT,
                                  float bottomRightS, float bottomRightT,
                                  float bottomLeftS, float bottomLeftT)
{
    m_topLeftS = topLeftS;
    m_topLeftT = topLeftT;
    m_topRightS = topRightS;
    m_topRightT = topRightT;
    m_bottomRightS = bottomRightS;
    m_bottomRightT = bottomRightT;
    m_bottomLeftS = bottomLeftS;
    m_bottomLeftT = bottomLeftT;
    m_initVerticesDone = false;
}

/*!
 * \brief TgImagePrivate::generateVertices
 *
 * generates vertices
 *
 * \param vertices
 */
void TgImagePrivate::generateVertices(Vertice vertices[4])
{
    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[0].s = m_topLeftS;
    vertices[0].t = m_topLeftT;

    vertices[1].x = 10;
    vertices[1].y = 0;
    vertices[1].s = m_topRightS;
    vertices[1].t = m_topRightT;

    vertices[2].x = 0;
    vertices[2].y = 10;
    vertices[2].s = m_bottomLeftS;
    vertices[2].t = m_bottomLeftT;

    vertices[3].x = 10;
    vertices[3].y = 10;
    vertices[3].s = m_bottomRightS;
    vertices[3].t = m_bottomRightT;
}

/*!
 * \brief TgRectanglePrivate::setTranform
 *
 * set transform matrix 4x4
 * \param currentItem
 */
void TgImagePrivate::setTranform(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!currentItem->getPositionChanged()) {
        TG_FUNCTION_END();
        return;
    }
    TgMatrix4x4 position;
    TgMatrix4x4 scale;
    position.setTransform(currentItem->getXonWindow(), currentItem->getYonWindow());
    scale.setScale(currentItem->getWidth()/10.0f, currentItem->getHeight()/10.0f);
    m_transform.mul(scale.getMatrixTable(), position.getMatrixTable());
    currentItem->setPositionChanged(false);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePrivate::render
 *
 * Renders the image
 * \param windowInfo
 * \param currentItem
 * \param itemPosition
 * \param opacity opacity for rendering
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgImagePrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem, TgItem2dPosition *itemPosition, float opacity)
{
    TG_FUNCTION_BEGIN();
    if (!itemPosition->isRenderVisible(windowInfo)) {
        TG_FUNCTION_END();
        return false;
    }
    glUniform4f(windowInfo->m_maxRenderValues,
                currentItem->getXminOnVisible(),
                currentItem->getYminOnVisible(),
                currentItem->getXmaxOnVisible(windowInfo),
                currentItem->getYmaxOnVisible(windowInfo));
    glUniform4f( windowInfo->m_shaderColorIndex, 1, 1, 1, 1);
    glUniform1f( windowInfo->m_shaderOpacityIndex, opacity);
    glUniformMatrix4fv(windowInfo->m_shaderTransformIndex, 1, 0, m_transform.getMatrixTable()->data);
    TgRender::render(getTextureIndex());
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgImagePrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgImagePrivate::checkPositionValues(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!m_initImageAssetDone) {
        getTextureIndex();
        m_initImageAssetDone = true;
    }

    if (!m_initVerticesDone || currentItem->getPositionChanged()) {
        init();
        setTranform(currentItem);
        m_initVerticesDone = true;
    }
    m_mutex.lock();
    if (!m_listPixelChange.empty()) {
        if (m_imageAsset.m_type == TgImageType::LoadedImage) {
            if (TgGlobalApplication::getInstance()->getImageAssets()->convertLoadedImageToGeneratedImage(m_imageAsset) == 0) {
                m_mutex.unlock();
                TG_FUNCTION_END();
                return;
            }
        }
        for (const TgImagePrivatePixelChange &pixel : m_listPixelChange) {
            TgImageDraw::setColor(m_imageAsset.m_imageData.m_generatedImage.m_imageData,
                           m_imageAsset.m_imageData.m_generatedImage.m_width,
                           m_imageAsset.m_imageData.m_generatedImage.m_height,
                           pixel.m_x, pixel.m_y,
                           pixel.m_r, pixel.m_g, pixel.m_b, pixel.m_a);
        }
        TgGlobalApplication::getInstance()->getImageAssets()->modifyTexture(
            m_imageAsset.m_imageData.m_generatedImage.m_imageData,
            m_imageAsset.m_imageData.m_generatedImage.m_width,
            m_imageAsset.m_imageData.m_generatedImage.m_height,
            m_imageAsset.m_textureIndex);
        m_listPixelChange.clear();
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePrivate::setImage
 *
 * set image's filename
 *
 * \param filename
 */
void TgImagePrivate::setImage(const char *filename)
{
    TG_FUNCTION_BEGIN();
    if (m_imageAsset.m_filename.compare(filename) == 0) {
        TG_FUNCTION_END();
        return;
    }
    m_imageAsset.m_textureIndex = 0;
    m_imageAsset.m_type = TgImageType::LoadedImage;
    m_imageAsset.m_filename = filename;
    m_initImageAssetDone = false;
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/**
 * @brief TgImagePrivate::setPixel
 *
 * set pixel x/y RGBA
 *
 * @param x
 * @param y
 * @param r red color
 * @param g green color
 * @param b blue color
 * @param a alpha color
 * @return true if pixel was possible to set
 * @return false
 */
bool TgImagePrivate::setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    switch (m_imageAsset.m_type) {
        case TgImageType::LoadedImage:
            if (x >= static_cast<uint32_t>(m_imageAsset.m_imageData.m_loadedImage.m_width)
                || y >= static_cast<uint32_t>(m_imageAsset.m_imageData.m_loadedImage.m_height)) {
                TG_WARNING_LOG("Set pixel to position failed", x, "/", y,
                    "max width/height is",
                    m_imageAsset.m_imageData.m_loadedImage.m_width, "/",
                    m_imageAsset.m_imageData.m_loadedImage.m_height);
                m_mutex.unlock();
                return false;
            }
            break;
        case TgImageType::GeneratedImage:
            if (x >= static_cast<uint32_t>(m_imageAsset.m_imageData.m_generatedImage.m_width)
                || y >= static_cast<uint32_t>(m_imageAsset.m_imageData.m_generatedImage.m_height)) {
                TG_WARNING_LOG("Set pixel to position failed", x, "/", y,
                    "max width/height is",
                    m_imageAsset.m_imageData.m_generatedImage.m_width, "/",
                    m_imageAsset.m_imageData.m_generatedImage.m_height);
                m_mutex.unlock();
                return false;
            }
            break;
        case TgImageType::PlainImage:
        case TgImageType::ImageTypeNA:
        default:
            return false;
    }
    TgImagePrivatePixelChange pixel;
    pixel.m_x = x;
    pixel.m_y = y;
    pixel.m_r = r;
    pixel.m_g = g;
    pixel.m_b = b;
    pixel.m_a = a;
    m_listPixelChange.push_back(pixel);
    m_mutex.unlock();
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
    return true;
}

/**
 * @brief TgImagePrivate::getImageWidth
 *
 * @return uint32_t image width
 */
uint32_t TgImagePrivate::getImageWidth()
{
    TG_FUNCTION_BEGIN();
    switch (m_imageAsset.m_type) {
        case TgImageType::LoadedImage:
            return m_imageAsset.m_imageData.m_loadedImage.m_width;
            TG_FUNCTION_END();
        case TgImageType::GeneratedImage:
            return m_imageAsset.m_imageData.m_generatedImage.m_width;
            TG_FUNCTION_END();
        case TgImageType::PlainImage:
            TG_FUNCTION_END();
            return 1;
        case TgImageType::ImageTypeNA:
        default:
            break;
    }
    TG_FUNCTION_END();
    return 0;
}

/**
 * @brief TgImagePrivate::getImageHeight
 *
 * @return uint32_t image height
 */
uint32_t TgImagePrivate::getImageHeight()
{
    TG_FUNCTION_BEGIN();
    switch (m_imageAsset.m_type) {
        case TgImageType::LoadedImage:
            TG_FUNCTION_END();
            return m_imageAsset.m_imageData.m_loadedImage.m_height;
        case TgImageType::GeneratedImage:
            TG_FUNCTION_END();
            return m_imageAsset.m_imageData.m_generatedImage.m_height;
        case TgImageType::PlainImage:
            TG_FUNCTION_END();
            return 1;
        case TgImageType::ImageTypeNA:
        default:
            break;
    }
    TG_FUNCTION_END();
    return 0;
}
