/*!
 * \file
 * \brief file tg_chart_private.cpp
 *
 * it holds general TgChartPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_chart_private.h"
#include <cstring>
#include <math.h>
#include "../../global/tg_global_log.h"
#include "../../global/tg_global_application.h"
#include "../../image/tg_image_assets.h"
#include "../../shader/tg_shader_2d.h"
#include "../tg_item2d.h"
#include "../../window/tg_mainwindow_private.h"
#include "../../global/private/tg_global_wait_renderer.h"
#include "item2d/tg_item2d_position.h"
#include "../../image/draw/tg_image_draw.h"

TgChartPrivate::TgChartPrivate(TgItem2d *parent, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    m_parent(parent),
    m_initVerticesDone(false),
    m_initImageAssetDone(false),
    m_BackgroundR(r),
    m_BackgroundG(g),
    m_BackgroundB(b),
    m_BackgroundA(a)
{
    TG_FUNCTION_BEGIN();
    m_imageAsset.m_textureIndex = 0;
    m_imageAsset.m_type = TgImageType::GeneratedImage;
    m_imageAsset.m_imageData.m_generatedImage.m_width = static_cast<int>(m_parent->getWidth());
    m_imageAsset.m_imageData.m_generatedImage.m_height = static_cast<int>(m_parent->getHeight());
    m_chartResolutionWidth = static_cast<int>(m_parent->getWidth());
    m_chartResolutionHeight = static_cast<int>(m_parent->getHeight());
    m_imageAsset.m_imageData.m_generatedImage.m_imageData = nullptr;
    m_fontFile = TgGlobalApplication::getInstance()->getFontDefault()->getDefaultFont();
    TG_FUNCTION_END();
}

TgChartPrivate::~TgChartPrivate()
{
    TG_FUNCTION_BEGIN();
    if (m_imageAsset.m_imageData.m_generatedImage.m_imageData) {
        delete[] m_imageAsset.m_imageData.m_generatedImage.m_imageData;
    }
    TgGlobalApplication::getInstance()->getImageAssets()->deleteImage(m_imageAsset);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::getTextureIndex
 *
 * gets texture index for this rectangle
 * if texture index is not done before, it tries to create it
 *
 * \return texture index
 */
GLuint TgChartPrivate::getTextureIndex()
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
 * \brief TgChartPrivate::setGridLinesColor
 *
 * set gridline color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChartPrivate::setGridLinesColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_gridLinesR = r;
    m_gridLinesG = g;
    m_gridLinesB = b;
    m_gridLinesA = a;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::setBackgroundColor
 *
 * set background color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChartPrivate::setBackgroundColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_BackgroundR = r;
    m_BackgroundG = g;
    m_BackgroundB = b;
    m_BackgroundA = a;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::setLineColor
 *
 * set line color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChartPrivate::setLineColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_lineR = r;
    m_lineG = g;
    m_lineB = b;
    m_lineA = a;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setTextColor
 *
 * set text color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChartPrivate::setTextColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_textR = r;
    m_textG = g;
    m_textB = b;
    m_textA = a;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::init
 *
 * inits the rectangle
 * Generates the texture with function getTextureIndex()
 * Generates the vertices
 *
 * \return true on success
 */
bool TgChartPrivate::init()
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
 * \brief TgChartPrivate::setTranform
 *
 * set transform matrix 4x4
 * \param currentItem
 */
void TgChartPrivate::setTranform(TgItem2d *currentItem)
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
 * \brief TgChartPrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgChartPrivate::checkPositionValues(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if ((m_chartResolutionWidth <= 0 || m_chartResolutionHeight <= 0)
        && !m_textureRequiresReset) {
        m_mutex.unlock();
        return;
    }
    if (m_textureRequiresReset) {
        m_drawingTextOnImage = m_newDrawText;
        if (m_imageAsset.m_imageData.m_generatedImage.m_imageData) {
            TgGlobalApplication::getInstance()->getImageAssets()->deleteImage(m_imageAsset);
        }

        if (m_chartResolutionWidth != m_imageAsset.m_imageData.m_generatedImage.m_width
            || m_chartResolutionHeight != m_imageAsset.m_imageData.m_generatedImage.m_height) {
            if (m_imageAsset.m_imageData.m_generatedImage.m_imageData) {
                delete[] m_imageAsset.m_imageData.m_generatedImage.m_imageData;
                m_imageAsset.m_imageData.m_generatedImage.m_imageData = nullptr;
            }
            m_imageAsset.m_imageData.m_generatedImage.m_width = m_chartResolutionWidth;
            m_imageAsset.m_imageData.m_generatedImage.m_height = m_chartResolutionHeight;
            if (m_imageAsset.m_imageData.m_generatedImage.m_width <= 0
                || m_imageAsset.m_imageData.m_generatedImage.m_height <= 0) {
                m_mutex.unlock();
                return;
            }
        }

        if (!m_imageAsset.m_imageData.m_generatedImage.m_imageData) {
            m_imageAsset.m_imageData.m_generatedImage.m_imageData = new unsigned char[m_imageAsset.m_imageData.m_generatedImage.m_width*m_imageAsset.m_imageData.m_generatedImage.m_height*4];
        }
        m_initImageAssetDone = false;
        m_textureRequiresReset = false;
        m_imageAsset.m_textureIndex = 0;
        drawBackground();
        drawGrid();
        drawLines();
        drawText();
    }
    if (!m_initImageAssetDone) {
        getTextureIndex();
        m_initImageAssetDone = true;
    }

    if (!m_initVerticesDone) {
        init();
        m_initVerticesDone = true;
    }
    setTranform(currentItem);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::render
 *
 * Renders the rectangle
 * \param windowInfo
 * \param currentItem
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgChartPrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem, TgItem2dPosition *itemPosition)
{
    TG_FUNCTION_BEGIN();
    if (m_imageAsset.m_imageData.m_generatedImage.m_width <= 0 ||
        m_imageAsset.m_imageData.m_generatedImage.m_height <= 0) {
        return false;
    }
    if (!itemPosition->isRenderVisible(windowInfo)) {
        return false;
    }
    glUniform4f(windowInfo->m_maxRenderValues,
                currentItem->getXminOnVisible(), currentItem->getYminOnVisible(),
                currentItem->getXmaxOnVisible(windowInfo),
                currentItem->getYmaxOnVisible(windowInfo));
    glUniform4f( windowInfo->m_shaderColorIndex, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(windowInfo->m_shaderTransformIndex, 1, 0, m_transform.getMatrixTable()->data);
    TgRender::render(getTextureIndex());
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgChartPrivate::setLineWidth
 *
 * set line width
 * \param lineWidth
 */
void TgChartPrivate::setLineWidth(uint32_t lineWidth)
{
    m_mutex.lock();
    if (lineWidth == m_lineWidth) {
        m_mutex.unlock();
        return;
    }
    m_textureRequiresReset = true;
    m_lineWidth = lineWidth;
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::getLineWidth
 *
 * \return line width
 */
uint32_t TgChartPrivate::getLineWidth()
{
    m_mutex.lock();
    uint32_t ret = m_lineWidth;
    m_mutex.unlock();
    return ret;
}


/*!
 * \brief TgChartPrivate::addChartPosition
 *
 * add new chart position
 * \param position
 */
void TgChartPrivate::addChartPosition(const TgChartPosition &position)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_listPosition.push_back(position);
    m_textureRequiresReset = true;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::clear
 *
 * clear all chart positions
 */
void TgChartPrivate::clear()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_listPosition.clear();
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::setChartPosition
 *
 * set new chart list position
 * \param listPosition
 */
void TgChartPrivate::setChartPosition(const std::vector<TgChartPosition> &listPosition)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_listPosition.assign(listPosition.begin(), listPosition.end());
    m_textureRequiresReset = true;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::setFontSize
 *
 * \param fontSize fon size for text
 */
void TgChartPrivate::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (std::fabs(m_fontSize - fontSize) <= std::numeric_limits<double>::epsilon()) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_fontSize = fontSize;
    m_textureRequiresReset = true;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChartPrivate::getFontSize
 *
 * \return font size
 */
float TgChartPrivate::getFontSize()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_fontSize;
}

/*!
 * \brief TgChartPrivate::getBottomGridMargin
 * \return bottom grid margin
 */
uint32_t TgChartPrivate::getBottomGridMargin() const
{
    return m_gridBottomMargin;
}

/*!
 * \brief TgChartPrivate::getTopGridMargin
 * \return top grid margin
 */
uint32_t TgChartPrivate::getTopGridMargin() const
{
    return m_gridTopMargin;
}

/*!
 * \brief TgChartPrivate::getLeftGridMargin
 * \return left grid margin
 */
uint32_t TgChartPrivate::getLeftGridMargin() const
{
    return m_gridLeftMargin;
}

/*!
 * \brief TgChartPrivate::getRightGridMargin
 * \return right grid margin
 */
uint32_t TgChartPrivate::getRightGridMargin() const
{
    return m_gridRightMargin;
}

/*!
 * \brief TgChartPrivate::setChartResolution
 * set new chart resolution
 * \param width
 * \param height
 */
void TgChartPrivate::setChartResolution(int width, int height)
{
    m_mutex.lock();
    if (m_chartResolutionWidth == width
        && m_chartResolutionHeight == height) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_chartResolutionWidth = width;
    m_chartResolutionHeight = height;
    m_textureRequiresReset = true;
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::getChartResolutionWidth
 *
 * \return chart resolution width
 */
int TgChartPrivate::getChartResolutionWidth()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    int ret = m_chartResolutionWidth;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgChartPrivate::setDrawText
 * draw text or not?
 * \param draw
 */
void TgChartPrivate::setDrawText(bool draw)
{
    m_mutex.lock();
    if (m_newDrawText == draw) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_newDrawText = draw;
    m_textureRequiresReset = true;
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::getDrawText
 *
 * \return draw text
 */
bool TgChartPrivate::getDrawText()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    bool ret = m_newDrawText;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*! TgChartPrivate::setChartLineAA
 * \param aa anti-aliasing method to set for chart
 */
void TgChartPrivate::setChartLineAA(TgChartLineAA aa)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_pathAA = aa;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*! TgChart::getChartLineAA
 * \return get anti-aliasing of chart method
 */
TgChartLineAA TgChartPrivate::getChartLineAA()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    TgChartLineAA ret = m_pathAA;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgChartPrivate::getChartResolutionHeight
 *
 * \return chart resolution height
 */
int TgChartPrivate::getChartResolutionHeight()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    int ret = m_chartResolutionHeight;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgChartPrivate::setBottomGridMargin
 * \param gridBottomMargin bottom grid margin
 */
void TgChartPrivate::setBottomGridMargin(uint32_t gridBottomMargin)
{
    m_mutex.lock();
    m_gridBottomMargin = gridBottomMargin;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::setTopGridMargin
 * \param gridTopMargin top grid margin
 */
void TgChartPrivate::setTopGridMargin(uint32_t gridTopMargin)
{
    m_mutex.lock();
    m_gridTopMargin = gridTopMargin;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::setLeftGridMargin
 * \param gridLeftMargin left grid margin
 */
void TgChartPrivate::setLeftGridMargin(uint32_t gridLeftMargin)
{
    m_mutex.lock();
    m_gridLeftMargin = gridLeftMargin;
    m_textureRequiresReset = true;
    TgGlobalWaitRenderer::getInstance()->release();
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::setRightGridMargin
 * \param gridRightMargin right grid margin
 */
void TgChartPrivate::setRightGridMargin(uint32_t gridRightMargin)
{
    m_mutex.lock();
    m_gridRightMargin = gridRightMargin;
    m_textureRequiresReset = true;
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::setXyDrawIsMaxedToSize
 * \param xyDrawIsMaxedToSize
 */
void TgChartPrivate::setXyDrawIsMaxedToSize(bool xyDrawIsMaxedToSize)
{
    m_mutex.lock();
    m_xyDrawIsMaxedToSize = xyDrawIsMaxedToSize;
    m_textureRequiresReset = true;
    m_mutex.unlock();
}

/*!
 * \brief TgChartPrivate::getXyDrawIsMaxedToSize
 *
 * get current if X/Y draw is maxed to size
 * = true, x/y positions are maxed
 * = false, x/y positions is linear
 *
 * \return xy draw is maxed to fize
 */
bool TgChartPrivate::getXyDrawIsMaxedToSize()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    bool ret = m_xyDrawIsMaxedToSize;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgChartPrivate::getMinMaxPosition
 * get min/max grid positions from listPosition
 * \param listPosition list of min/max positions to search
 * \param minX [out] min X position, if listPosition is empty, then 0
 * \param minY [out] min Y position, if listPosition is empty, then 0
 * \param maxX [out] max X position, if listPosition is empty, then 0
 * \param maxY [out] max Y position, if listPosition is empty, then 0
 */
void TgChartPrivate::getMinMaxPosition(const std::vector<TgChartPosition>&listPosition,
                                       double &minX, double &minY,
                                       double &maxX, double &maxY)
{
    if (listPosition.empty()) {
        minX = 0;
        maxX = 0;
        minY = 0;
        maxY = 0;
        return;
    }
    minX = listPosition.front().x;
    maxX = listPosition.front().x;
    minY = listPosition.front().y;
    maxY = listPosition.front().y;
    std::vector<TgChartPosition>::const_iterator it;
    for (it=listPosition.begin();it!=listPosition.end();it++) {
        minX = std::min(minX, it->x);
        minY = std::min(minY, it->y);
        maxX = std::max(maxX, it->x);
        maxY = std::max(maxY, it->y);
    }
}

/*!
 * \brief TgChartPrivate::drawBackground
 * draws background
 */
void TgChartPrivate::drawBackground()
{
    int x, y, i = 0;
    for (x=0;x<m_imageAsset.m_imageData.m_generatedImage.m_width;x++) {
        for (y=0;y<m_imageAsset.m_imageData.m_generatedImage.m_height;y++) {
            m_imageAsset.m_imageData.m_generatedImage.m_imageData[i++] = m_BackgroundR;
            m_imageAsset.m_imageData.m_generatedImage.m_imageData[i++] = m_BackgroundG;
            m_imageAsset.m_imageData.m_generatedImage.m_imageData[i++] = m_BackgroundB;
            m_imageAsset.m_imageData.m_generatedImage.m_imageData[i++] = m_BackgroundA;
        }
    }
}

/*!
 * \brief TgChartPrivate::drawGrid
 * draws grid
 */
void TgChartPrivate::drawGrid()
{
    if (m_imageAsset.m_imageData.m_generatedImage.m_height > 0
        && static_cast<uint32_t>(m_imageAsset.m_imageData.m_generatedImage.m_height) > m_gridBottomMargin) {
        TgImageDraw::drawLine(m_imageAsset.m_imageData.m_generatedImage.m_imageData,
                              m_imageAsset.m_imageData.m_generatedImage.m_width,
                              m_imageAsset.m_imageData.m_generatedImage.m_height,
                              m_gridLeftMargin, m_gridTopMargin,
                              m_gridLeftMargin, m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin,
                              m_gridLinesR, m_gridLinesG, m_gridLinesB, m_gridLinesA, 1);
        TgImageDraw::drawLine(m_imageAsset.m_imageData.m_generatedImage.m_imageData,
                              m_imageAsset.m_imageData.m_generatedImage.m_width,
                              m_imageAsset.m_imageData.m_generatedImage.m_height,
                              m_gridLeftMargin, m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin,
                              m_imageAsset.m_imageData.m_generatedImage.m_width-m_gridRightMargin, m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin,
                              m_gridLinesR, m_gridLinesG, m_gridLinesB, m_gridLinesA, 1);
    }
}

/*!
 * \brief TgChartPrivate::drawLines
 * draws grid lines
 */
void TgChartPrivate::drawLines()
{
    if (m_listPosition.empty()) {
        return;
    }

    size_t i;
    double minX, minY, maxX, maxY;
    uint32_t chartAreaTopLeftX = m_gridLeftMargin;
    uint32_t chartAreaTopLeftY = m_gridTopMargin;
    uint32_t chartAreaBottomRightX = m_imageAsset.m_imageData.m_generatedImage.m_width-m_gridRightMargin;
    uint32_t chartAreaBottomRightY = m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin;
    getMinMaxPositionForChart(minX, minY, maxX, maxY);
    if (m_listPosition.size() == 1) {
        double x0 = static_cast<double>(chartAreaBottomRightX - chartAreaTopLeftX)*(m_listPosition.at(0).x - minX) / (maxX - minX);
        double y0 = static_cast<double>(chartAreaBottomRightY - chartAreaTopLeftY)*(m_listPosition.at(0).y - minY) / (maxY - minY);
        TgImageDraw::drawLine(m_imageAsset.m_imageData.m_generatedImage.m_imageData,
                              m_imageAsset.m_imageData.m_generatedImage.m_width,
                              m_imageAsset.m_imageData.m_generatedImage.m_height,
                              static_cast<uint32_t>(x0)+chartAreaTopLeftX, chartAreaBottomRightY-(static_cast<uint32_t>(y0)),
                              static_cast<uint32_t>(x0)+chartAreaTopLeftX, chartAreaBottomRightY-(static_cast<uint32_t>(y0)),
                              m_lineR, m_lineG, m_lineB, m_lineA, m_lineWidth);
        return;
    }

    if (m_listPosition.empty()) {
        return;
    }

    clearPoints();

    for (i=0;i<m_listPosition.size();i++) {
        addPoint(
            TgImageDrawPathPoint(static_cast<float>(chartAreaTopLeftX)+0.5f+
                static_cast<float>(static_cast<double>(chartAreaBottomRightX - chartAreaTopLeftX)*(m_listPosition.at(i).x - minX) / (maxX - minX)),
                static_cast<float>(chartAreaBottomRightY)-0.5f-
                static_cast<float>(static_cast<double>(chartAreaBottomRightY - chartAreaTopLeftY)*(m_listPosition.at(i).y - minY) / (maxY - minY))));
    }
    TgImageDrawPath::draw(m_imageAsset.m_imageData.m_generatedImage.m_imageData,
                              m_imageAsset.m_imageData.m_generatedImage.m_width,
                              m_imageAsset.m_imageData.m_generatedImage.m_height,
                            m_lineR, m_lineG, m_lineB, m_lineA, m_lineWidth);
}

/*!
 * \brief TgChartPrivate::getMinMaxPositionForChart
 * get min/max grid positions from m_listPosition for chart
 * \param minX [out] min X position, if listPosition is empty, then -0.1f
 * \param minY [out] min Y position, if listPosition is empty, then +0.1f
 * \param maxX [out] max X position, if listPosition is empty, then -0.1f
 * \param maxY [out] max Y position, if listPosition is empty, then +0.1f
 */
void TgChartPrivate::getMinMaxPositionForChart(double &minX, double &minY, double &maxX, double &maxY)
{
    uint32_t chartAreaTopLeftX = m_gridLeftMargin;
    uint32_t chartAreaTopLeftY = m_gridTopMargin;
    uint32_t chartAreaBottomRightX = m_imageAsset.m_imageData.m_generatedImage.m_width-m_gridRightMargin;
    uint32_t chartAreaBottomRightY = m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin;
    getMinMaxPosition(m_listPosition, minX, minY, maxX, maxY);
    uint32_t sizeX = chartAreaBottomRightX - chartAreaTopLeftX;
    uint32_t sizeY = chartAreaBottomRightY - chartAreaTopLeftY;
    double diffMinMaxX = maxX - minX;
    double diffMinMaxY = maxY - minY;
    if (diffMinMaxX <= 0.2) {
        minX -= 0.1f;
        maxX += 0.1f;
        diffMinMaxX = maxX - minX;
    }
    if (diffMinMaxY <= 0.2) {
        minY -= 0.1f;
        maxY += 0.1f;
        diffMinMaxY = maxY - minY;
    }
    if (m_xyDrawIsMaxedToSize) {
        return;
    }
    double pixelSizeX = static_cast<double>(sizeX)/(diffMinMaxX);
    double pixelSizeY = static_cast<double>(sizeY)/(diffMinMaxY);
    if (pixelSizeX > pixelSizeY) {
        maxX = (diffMinMaxY)*static_cast<double>(sizeX)/static_cast<double>(sizeY) + minX;
    } else {
        maxY = (diffMinMaxX)*static_cast<double>(sizeY)/static_cast<double>(sizeX) + minY;
    }
}

/*!
 * \brief TgChartPrivate::drawText
 * draws text
 */
void TgChartPrivate::drawText()
{
    if (m_listPosition.empty() || !m_drawingTextOnImage) {
        return;
    }
    double x, y;
    double minXchart, minYchart, maxXchart, maxYchart;
    double minX, minY, maxX, maxY;
    getMinMaxPositionForChart(minXchart, minYchart, maxXchart, maxYchart);
    getMinMaxPosition(m_listPosition, minX, minY, maxX, maxY);
    std::string yMinString = getChartNumber(std::to_string(minY));
    std::string xMinString = getChartNumber(std::to_string(minX));
    uint32_t chartAreaTopLeftX = m_gridLeftMargin;
    uint32_t chartAreaTopLeftY = m_gridTopMargin;
    uint32_t chartAreaBottomRightX = m_imageAsset.m_imageData.m_generatedImage.m_width-m_gridRightMargin;
    uint32_t chartAreaBottomRightY = m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin;

    std::string yMaxString = getChartNumber(std::to_string(maxY));
    y = static_cast<double>(chartAreaBottomRightY) - static_cast<double>(chartAreaBottomRightY - chartAreaTopLeftY)*(maxY - minYchart) / (maxYchart - minYchart);

    m_fontToImageMaxY.setListText(m_fontFile.c_str(), m_fontSize, yMaxString.c_str());
    m_fontToImageMaxY.fillIntoImage(m_imageAsset, static_cast<float>(m_gridLeftMargin)-static_cast<float>(m_textToGridMarginX),
        static_cast<float>(y),
        m_textR, m_textG, m_textB, m_textA, TgTextfieldHorizontalAlign::AlignRight, TgTextfieldVerticalAlign::AlignCenterV);

    m_fontToImageMinY.setListText(m_fontFile.c_str(), m_fontSize, yMinString.c_str());
    m_fontToImageMinY.fillIntoImage(m_imageAsset, static_cast<float>(m_gridLeftMargin)-static_cast<float>(m_textToGridMarginX),
        static_cast<float>(m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin),
        m_textR, m_textG, m_textB, m_textA, TgTextfieldHorizontalAlign::AlignRight, TgTextfieldVerticalAlign::AlignBottom);

    m_fontToImageMinX.setListText(m_fontFile.c_str(), m_fontSize, xMinString.c_str());
    m_fontToImageMinX.fillIntoImage(m_imageAsset, static_cast<float>(m_gridLeftMargin)-static_cast<float>(m_textToGridMarginX),
        static_cast<float>(m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin+m_textToGridMarginY),
        m_textR, m_textG, m_textB, m_textA, TgTextfieldHorizontalAlign::AlignCenterH, TgTextfieldVerticalAlign::AlignTop);


    x = static_cast<double>(chartAreaBottomRightX - chartAreaTopLeftX)*(maxX - minXchart) / (maxXchart - minXchart) + static_cast<double>(chartAreaTopLeftX);
    std::string xMaxString = getChartNumber(std::to_string(maxX));

    m_fontToImageMaxX.setListText(m_fontFile.c_str(), m_fontSize, xMaxString.c_str());
    m_fontToImageMaxX.fillIntoImage(m_imageAsset,
        static_cast<float>(x),
        static_cast<float>(m_imageAsset.m_imageData.m_generatedImage.m_height-m_gridBottomMargin+m_textToGridMarginY),
        m_textR, m_textG, m_textB, m_textA, TgTextfieldHorizontalAlign::AlignRight, TgTextfieldVerticalAlign::AlignTop);
}

std::string TgChartPrivate::getChartNumber(const std::string &number)
{
    std::string ret = number;
    while (1) {
        if (ret.empty() || ret.size() == 1) {
            break;
        }
        if (ret.back() == '.') {
            ret.resize(ret.size()-1);
            break;
        }
        if (ret.back() == '0') {
            ret.resize(ret.size()-1);
            continue;
        }
        break;
    }
    if (ret.empty()) {
        ret = "0";
    }
    return ret;
}
