/*!
 * \file
 * \brief file tg_image_part_private.cpp
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_image_part_private.h"
#include <cmath>
#include "../../global/tg_global_application.h"
#include "../../global/tg_global_log.h"
#include "../tg_item2d.h"
#include "../../window/tg_mainwindow_private.h"

TgImagePartPrivate::TgImagePartPrivate(TgItem2d *currentItem, const char *imageFilename, TgImagePartType type) :
    m_type(type),
    m_initImageAssetDone(false),
    m_initVerticesDone(false),
    m_currentItem(currentItem),
    m_imageCropLeft(0), m_imageCropTop(0), m_imageCropRight(0), m_imageCropBottom(0),
    m_leftAreaSize(0), m_topAreaSize(0), m_rightAreaSize(0), m_bottomAreaSize(0),
    m_imageCrop3Left(0), m_imageCrop3Right(0),
    m_leftArea3Size(0), m_rightArea3Size(0),
    m_imageCrop3Top(0), m_imageCrop3Bottom(0),
    m_topArea3Size(0), m_bottomArea3Size(0)
{
    TG_FUNCTION_BEGIN();
    m_imageAsset.m_textureIndex = 0;
    m_imageAsset.m_type = TgImageType::LoadedImage;
    m_imageAsset.m_filename = imageFilename;
    TG_FUNCTION_END();
}

TgImagePartPrivate::~TgImagePartPrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgImagePartType TgImagePartPrivate::getType() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_type;
}

void TgImagePartPrivate::setType(TgImagePartType type)
{
    TG_FUNCTION_BEGIN();
    if (type == m_type) {
        TG_FUNCTION_END();
        return;
    }
    m_type = type;
    m_currentItem->setPositionChanged(true);
    m_initVerticesDone = false;
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePartPrivate::setImageCropPosition
 *
 * set image's crop position, all values must be under 1
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropLeft = 0.2 and imageCropTop = 0.2, imageCropRight = 0.2, imageCropBottom = 0.2
 * then setImageAreaSize's leftAreaSize = 10, topAreaSize = 10, rightAreaSize = 10, bottomAreaSize = 10
 * -> top-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * -> left area on UI is 10x180 pixels, and from original image it's 72.4x219.2 pixels (60% of 362)
 * -> bottom-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * center and right size of the area goes similar way.
 *
 * \param imageCropLeft percent the image's crop left area (must be under 1)
 * \param imageCropTop percent the image's crop top area (must be under 1)
 * \param imageCropRight percent the image's crop right area (must be under 1)
 * \param imageCropBottom percent the image's crop bottom area (must be under 1)
 */
void TgImagePartPrivate::setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_imageCropLeft - imageCropLeft) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCropTop - imageCropTop) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCropRight - imageCropRight) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCropBottom - imageCropBottom) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_imageCropLeft = imageCropLeft;
    m_imageCropTop = imageCropTop;
    m_imageCropRight = imageCropRight;
    m_imageCropBottom = imageCropBottom;
    if (m_type == TgImagePartType::TgImagePartType_Part9) {
        m_currentItem->setPositionChanged(true);
        m_initVerticesDone = false;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePartPrivate::setImageAreaSize
 *
 * set image's crop positions, all values are pixels
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * setImageCropPosition's imageCropLeft = 0.2 and imageCropTop = 0.2, imageCropRight = 0.2, imageCropBottom = 0.2
 * then leftAreaSize = 10, topAreaSize = 10, rightAreaSize = 10, bottomAreaSize = 10
 * -> top-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * -> left area on UI is 10x180 pixels, and from original image it's 72.4x219.2 pixels (60% of 362)
 * -> bottom-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * center and right size of the area goes similar way.
 *
 * \param leftAreaSize left area's pixel size
 * \param topAreaSize top area's pixel size
 * \param rightAreaSize right area's pixel size
 * \param bottomAreaSize bottom area's pixel size
 */
void TgImagePartPrivate::setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_leftAreaSize - leftAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_topAreaSize - topAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_rightAreaSize - rightAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_bottomAreaSize - bottomAreaSize) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_leftAreaSize = leftAreaSize;
    m_topAreaSize = topAreaSize;
    m_rightAreaSize = rightAreaSize;
    m_bottomAreaSize = bottomAreaSize;
    if (m_type == TgImagePartType::TgImagePartType_Part9) {
        m_currentItem->setPositionChanged(true);
        m_initVerticesDone = false;
    }
    TG_FUNCTION_END();
}

void TgImagePartPrivate::setImageCropPosition3LeftToRight(float imageCropLeft, float imageCropRight)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_imageCrop3Left - imageCropLeft) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCrop3Right - imageCropRight) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_imageCrop3Left = imageCropLeft;
    m_imageCrop3Right = imageCropRight;
    if (m_type == TgImagePartType::TgImagePartType_Part3_LeftToRight) {
        m_currentItem->setPositionChanged(true);
        m_initVerticesDone = false;
    }
    TG_FUNCTION_END();
}

void TgImagePartPrivate::setImageAreaSize3LeftToRight(float leftAreaSize, float rightAreaSize)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_leftArea3Size - leftAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_rightArea3Size - rightAreaSize) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_leftArea3Size = leftAreaSize;
    m_rightArea3Size = rightAreaSize;
    if (m_type == TgImagePartType::TgImagePartType_Part3_LeftToRight) {
        m_currentItem->setPositionChanged(true);
        m_initVerticesDone = false;
    }
    TG_FUNCTION_END();
}

void TgImagePartPrivate::setImageCropPosition3TopToBottom(float imageCropTop, float imageCropBottom)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_imageCrop3Top - imageCropTop) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCrop3Bottom - imageCropBottom) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_imageCrop3Top = imageCropTop;
    m_imageCrop3Bottom = imageCropBottom;
    if (m_type == TgImagePartType::TgImagePartType_Part3_UpToDown) {
        m_currentItem->setPositionChanged(true);
        m_initVerticesDone = false;
    }
    TG_FUNCTION_END();
}

void TgImagePartPrivate::setImageAreaSize3TopToBottom(float topAreaSize, float bottomAreaSize)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_topArea3Size - topAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_bottomArea3Size - bottomAreaSize) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_topArea3Size = topAreaSize;
    m_bottomArea3Size = bottomAreaSize;
    if (m_type == TgImagePartType::TgImagePartType_Part3_UpToDown) {
        m_currentItem->setPositionChanged(true);
        m_initVerticesDone = false;
    }
    TG_FUNCTION_END();
}


void TgImagePartPrivate::setTexturePosition(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT],
                                            VerticeIndexPositionParts9 part,
                                            float textureX, float textureY,
                                            float textureWidth, float textureHeight,
                                            float x, float y, float width, float height)
{
    vertices[0+static_cast<int>(part)].x = x;
    vertices[0+static_cast<int>(part)].y = y;
    vertices[0+static_cast<int>(part)].s = textureX;
    vertices[0+static_cast<int>(part)].t = textureY;

    vertices[1+static_cast<int>(part)].x = x+width;
    vertices[1+static_cast<int>(part)].y = y;
    vertices[1+static_cast<int>(part)].s = textureX+textureWidth;
    vertices[1+static_cast<int>(part)].t = textureY;

    vertices[2+static_cast<int>(part)].x = x;
    vertices[2+static_cast<int>(part)].y = y+height;
    vertices[2+static_cast<int>(part)].s = textureX;
    vertices[2+static_cast<int>(part)].t = textureY+textureHeight;

    vertices[3+static_cast<int>(part)].x = x+width;
    vertices[3+static_cast<int>(part)].y = y+height;
    vertices[3+static_cast<int>(part)].s = textureX+textureWidth;
    vertices[3+static_cast<int>(part)].t = textureY+textureHeight;
}

void TgImagePartPrivate::generateVertices3LeftToRight(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT])
{
    for (int i=static_cast<int>(VerticeIndexPositionParts9::MiddleLeft);i<33;i+=4) {
        setTexturePosition(vertices, static_cast<VerticeIndexPositionParts9>(i),
                           0, 0,
                           1, 1,
                           0, 0,
                           1, 1);
    }

    setTexturePosition(vertices, VerticeIndexPositionParts9::TopLeft,
                       0, 0,
                       m_imageCrop3Left, 1,
                       0, 0,
                       m_leftArea3Size, m_currentItem->getHeight());

    setTexturePosition(vertices, VerticeIndexPositionParts9::TopMiddle,
                       m_imageCrop3Left, 0,
                       1.0f-m_imageCrop3Right-m_imageCrop3Left, 1,
                       m_leftArea3Size, 0,
                       m_currentItem->getWidth()-m_leftArea3Size-m_rightArea3Size, m_currentItem->getHeight());

    setTexturePosition(vertices, VerticeIndexPositionParts9::TopRight,
                       1.0f-m_imageCrop3Right, 0,
                       m_imageCrop3Right, 1,
                       m_currentItem->getWidth()-m_rightArea3Size, 0,
                       m_rightArea3Size, m_currentItem->getHeight());
}

void TgImagePartPrivate::generateVertices3TopToBottom(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT])
{
    for (int i=static_cast<int>(VerticeIndexPositionParts9::MiddleLeft);i<33;i+=4) {
        setTexturePosition(vertices, static_cast<VerticeIndexPositionParts9>(i),
                           0, 0,
                           1, 1,
                           0, 0,
                           1, 1);
    }

    setTexturePosition(vertices, VerticeIndexPositionParts9::TopLeft,
                       0, 0,
                       1, m_imageCrop3Top,
                       0, 0,
                       m_currentItem->getWidth(), m_topArea3Size);
    setTexturePosition(vertices, VerticeIndexPositionParts9::TopMiddle,
                       0, m_imageCrop3Top,
                       1, 1.0f-m_imageCrop3Top-m_imageCrop3Bottom,
                       0, m_topArea3Size,
                       m_currentItem->getWidth(), m_currentItem->getHeight()-m_topArea3Size-m_bottomArea3Size);
    setTexturePosition(vertices, VerticeIndexPositionParts9::TopRight,
                       0, 1.0f-m_imageCrop3Bottom,
                       1, m_imageCrop3Bottom,
                       0, m_currentItem->getHeight()-m_bottomArea3Size,
                       m_currentItem->getWidth(), m_bottomArea3Size);
}

/*!
 * \brief TgImagePartPrivate::generateVertices
 *
 * generates vertices
 *
 * \param vertices
 */
void TgImagePartPrivate::generateVertices(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT])
{
    if (m_type == TgImagePartType::TgImagePartType_Part3_LeftToRight) {
        generateVertices3LeftToRight(vertices);
        return;
    }
    if (m_type == TgImagePartType::TgImagePartType_Part3_UpToDown) {
        generateVertices3TopToBottom(vertices);
        return;
    }
    // top-left
    setTexturePosition(vertices, VerticeIndexPositionParts9::TopLeft,
                       0, 0,
                       m_imageCropLeft, m_imageCropTop,
                       0, 0,
                       m_leftAreaSize, m_topAreaSize);

    setTexturePosition(vertices, VerticeIndexPositionParts9::TopMiddle,
                       m_imageCropLeft, 0,
                       1.0f-m_imageCropRight-m_imageCropLeft, m_imageCropTop,
                       m_leftAreaSize, 0,
                       m_currentItem->getWidth()-m_leftAreaSize-m_rightAreaSize, m_topAreaSize);

    setTexturePosition(vertices, VerticeIndexPositionParts9::TopRight,
                       1.0f-m_imageCropRight, 0,
                       m_imageCropRight, m_imageCropTop,
                       m_currentItem->getWidth()-m_rightAreaSize, 0,
                       m_rightAreaSize, m_topAreaSize);

    // middle
    setTexturePosition(vertices, VerticeIndexPositionParts9::MiddleLeft,
                       0, m_imageCropTop,
                       m_imageCropLeft, 1.0f-m_imageCropBottom-m_imageCropTop,
                       0, m_topAreaSize,
                       m_leftAreaSize, m_currentItem->getHeight()-m_topAreaSize-m_bottomAreaSize);

    setTexturePosition(vertices, VerticeIndexPositionParts9::MiddleMiddle,
                       m_imageCropLeft, m_imageCropTop,
                       1.0f-m_imageCropRight-m_imageCropLeft, 1.0f-m_imageCropBottom-m_imageCropTop,
                       m_leftAreaSize, m_topAreaSize,
                       m_currentItem->getWidth()-m_leftAreaSize-m_rightAreaSize,
                       m_currentItem->getHeight()-m_topAreaSize-m_bottomAreaSize);

    setTexturePosition(vertices, VerticeIndexPositionParts9::MiddleRight,
                       1.0f-m_imageCropRight, m_imageCropTop,
                       m_imageCropRight, 1.0f-m_imageCropBottom-m_imageCropTop,
                       m_currentItem->getWidth()-m_rightAreaSize, m_topAreaSize,
                       m_rightAreaSize, m_currentItem->getHeight()-m_topAreaSize-m_bottomAreaSize);

    //bottom
    setTexturePosition(vertices, VerticeIndexPositionParts9::BottomLeft,
                       0, 1.0f-m_imageCropBottom,
                       m_imageCropLeft, m_imageCropBottom,
                       0, m_currentItem->getHeight()-m_bottomAreaSize,
                       m_leftAreaSize, m_bottomAreaSize);

    setTexturePosition(vertices, VerticeIndexPositionParts9::BottomMiddle,
                       m_imageCropLeft, 1.0f-m_imageCropBottom,
                       1.0f-m_imageCropRight-m_imageCropLeft, m_imageCropBottom,
                       m_leftAreaSize, m_currentItem->getHeight()-m_bottomAreaSize,
                       m_currentItem->getWidth()-m_leftAreaSize-m_rightAreaSize, m_bottomAreaSize);

    setTexturePosition(vertices, VerticeIndexPositionParts9::BottomRight,
                       1.0f-m_imageCropRight, 1.0f-m_imageCropBottom,
                       m_imageCropRight, m_imageCropBottom,
                       m_currentItem->getWidth()-m_rightAreaSize, m_currentItem->getHeight()-m_bottomAreaSize,
                       m_rightAreaSize, m_bottomAreaSize);
}

/*!
 * \brief TgImagePartPrivate::getTextureIndex
 *
 * gets texture index for this rectangle
 * if texture index is not done before, it tries to create it
 *
 * \return texture index
 */
GLuint TgImagePartPrivate::getTextureIndex()
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
 * \brief TgImagePartPrivate::init
 *
 * inits the vertices
 * Generates the vertices
 *
 * \return true on success
 */
bool TgImagePartPrivate::init()
{
    TG_FUNCTION_BEGIN();
    Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT];
    generateVertices(vertices);
    TG_FUNCTION_END();
    return TgRender::init(vertices, IMAGE_PARTS_VERTICES_MAX_COUNT);
}

/*!
 * \brief TgRectanglePrivate::setTranform
 *
 * set transform matrix 4x4
 * \param currentItem
 */
void TgImagePartPrivate::setTranform(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!currentItem->getPositionChanged()) {
        TG_FUNCTION_END();
        return;
    }
    m_transform.setTransform(currentItem->getXonWindow(), currentItem->getYonWindow());
    currentItem->setPositionChanged(false);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePartPrivate::render
 *
 * Renders the image
 * \param windowInfo
 * \param currentItem
 */
void TgImagePartPrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    glUniform4f(windowInfo->m_maxRenderValues,
                currentItem->getXminOnVisible(),
                currentItem->getYminOnVisible(),
                currentItem->getXmaxOnVisible(windowInfo),
                currentItem->getYmaxOnVisible(windowInfo));
    glUniform4f( windowInfo->m_shaderColorIndex, 1, 1, 1, 1);
    glUniformMatrix4fv(windowInfo->m_shaderTransformIndex, 1, 0, m_transform.getMatrixTable()->data);
    glBindTexture(GL_TEXTURE_2D, getTextureIndex());
    if (TgImagePartType::TgImagePartType_Part9 == m_type) {
        for (int i=0;i<9;i++) {
            TgRender::render(getTextureIndex(), i*4, 4);
        }
    } else {
        for (int i=0;i<3;i++) {
            TgRender::render(getTextureIndex(), i*4, 4);
        }
    }
    glBindVertexArray(0);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePartPrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgImagePartPrivate::checkPositionValues(TgItem2d *currentItem)
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
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePartPrivate::setImage
 *
 * set image's filename
 *
 * \param filename
 */
void TgImagePartPrivate::setImage(const char *filename)
{
    TG_FUNCTION_BEGIN();
    if (m_imageAsset.m_filename.compare(filename) == 0) {
        TG_FUNCTION_END();
        return;
    }
    m_imageAsset.m_textureIndex = 0;
    m_imageAsset.m_filename = filename;
    m_initImageAssetDone = false;
    TG_FUNCTION_END();
}
