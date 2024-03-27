#include "tg_image_draw_filler.h"
#include <string.h>

TgImageDrawFiller::~TgImageDrawFiller()
{
    if (m_imageData) {
        delete[] m_imageData;
    }
}

/*! \brief TgImageDrawFiller::init
 * inits the filler data
 * \param imageWidth width of image
 * \param imageHeight height of image
 */
void TgImageDrawFiller::init(const uint32_t &imageWidth, const uint32_t &imageHeight)
{
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_minX = imageWidth;
    m_maxX = 0;
    m_minY = imageHeight;
    m_maxY = 0;
    if (m_imageDataSize >= imageWidth*imageHeight) {
        memset(m_imageData, 0, sizeof(uint8_t)*imageWidth*imageHeight);
        return;
    }
    if (m_imageData) {
        delete[] m_imageData;
    }
    m_imageData = new uint8_t[imageWidth*imageHeight];
    memset(m_imageData, 0, sizeof(uint8_t)*imageWidth*imageHeight);
}

void TgImageDrawFiller::set(const uint32_t &x, const uint32_t &y, const uint8_t &value)
{
    m_imageData[y*m_imageWidth + x] = value;
    if (x < m_minX) {
        m_minX = x;
    }
    if (y < m_minY) {
        m_minY = y;
    }
    if (x > m_maxX) {
        m_maxX = x;
    }
    if (y > m_maxY) {
        m_maxY = y;
    }
}

uint8_t TgImageDrawFiller::get(const uint32_t &x, const uint32_t &y)
{
    return m_imageData[y*m_imageWidth + x];
}

uint32_t TgImageDrawFiller::getMinX()
{
    return m_minX;
}
uint32_t TgImageDrawFiller::getMaxX()
{
    return m_maxX;
}
uint32_t TgImageDrawFiller::getMinY()
{
    return m_minY;
}
uint32_t TgImageDrawFiller::getMaxY()
{
    return m_maxY;
}
