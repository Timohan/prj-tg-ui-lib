/*!
 * \file
 * \brief file tg_rectangle_private.cpp
 *
 * it holds general TgRectanglePrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_rectangle_private.h"
#include <cstring>
#include "../../global/tg_global_log.h"
#include "../../global/tg_global_application.h"
#include "../../image/tg_image_assets.h"
#include "../../shader/tg_shader_2d.h"
#include "../tg_item2d.h"
#include "../../window/tg_mainwindow_private.h"

TgRectanglePrivate::TgRectanglePrivate(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    m_initVerticesDone(false),
    m_initImageAssetDone(false),
    m_r(static_cast<float>(r)/255.0f),
    m_g(static_cast<float>(g)/255.0f),
    m_b(static_cast<float>(b)/255.0f),
    m_a(static_cast<float>(a)/255.0f)
{
    TG_FUNCTION_BEGIN();
    m_imageAsset.m_textureIndex = 0;
    m_imageAsset.m_type = TgImageType::PlainImage;
    m_imageAsset.m_imageData.m_plainImage.r = 255;
    m_imageAsset.m_imageData.m_plainImage.g = 255;
    m_imageAsset.m_imageData.m_plainImage.b = 255;
    m_imageAsset.m_imageData.m_plainImage.a = 255;
    TG_FUNCTION_END();
}

TgRectanglePrivate::~TgRectanglePrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgRectanglePrivate::getTextureIndex
 *
 * gets texture index for this rectangle
 * if texture index is not done before, it tries to create it
 *
 * \return texture index
 */
GLuint TgRectanglePrivate::getTextureIndex()
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
 * \brief TgRectanglePrivate::setColor
 *
 * set color for rectangle
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgRectanglePrivate::setColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_r = static_cast<float>(r)/255.0f;
    m_g = static_cast<float>(g)/255.0f;
    m_b = static_cast<float>(b)/255.0f;
    m_a = static_cast<float>(a)/255.0f;
    TG_FUNCTION_END();
}

/*!
 * \brief TgRectanglePrivate::init
 *
 * inits the rectangle
 * Generates the texture with function getTextureIndex()
 * Generates the vertices
 *
 * \return true on success
 */
bool TgRectanglePrivate::init()
{
    TG_FUNCTION_BEGIN();
    Vertice vertices[4];

    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[0].s = 0;
    vertices[0].t = 0;

    vertices[1].x = 10;
    vertices[1].y = 0;
    vertices[1].s = 1;
    vertices[1].t = 0;

    vertices[2].x = 0;
    vertices[2].y = 10;
    vertices[2].s = 0;
    vertices[2].t = 1;

    vertices[3].x = 10;
    vertices[3].y = 10;
    vertices[3].s = 1;
    vertices[3].t = 1;

    TG_FUNCTION_END();
    return TgRender::init(vertices, 4);
}

/*!
 * \brief TgRectanglePrivate::setTranform
 *
 * set transform matrix 4x4
 * \param currentItem
 */
void TgRectanglePrivate::setTranform(TgItem2d *currentItem)
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
 * \brief TgRectanglePrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgRectanglePrivate::checkPositionValues(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!m_initImageAssetDone) {
        getTextureIndex();
        m_initImageAssetDone = true;
    }

    if (!m_initVerticesDone) {
        init();
        m_initVerticesDone = true;
    }
    setTranform(currentItem);
    TG_FUNCTION_END();
}

/*!
 * \brief TgRectanglePrivate::render
 *
 * Renders the rectangle
 * \param windowInfo
 * \param currentItem
 */
void TgRectanglePrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    glUniform4f(windowInfo->m_maxRenderValues,
                currentItem->getXminOnVisible(), currentItem->getYminOnVisible(),
                currentItem->getXmaxOnVisible(windowInfo),
                currentItem->getYmaxOnVisible(windowInfo));
    glUniform4f( windowInfo->m_shaderColorIndex, m_r, m_g, m_b, m_a);
    glUniformMatrix4fv(windowInfo->m_shaderTransformIndex, 1, 0, m_transform.getMatrixTable()->data);
    TgRender::render(getTextureIndex());
    TG_FUNCTION_END();
}

