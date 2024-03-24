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
}

uint8_t TgImageDrawFiller::get(const uint32_t &x, const uint32_t &y)
{
    return m_imageData[y*m_imageWidth + x];
}
