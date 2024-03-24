#include "tg_image_draw_path.h"
#include "../../math/tg_math2d.h"
#include <cstddef>
#include <map>
#include <math.h>
#include "tg_image_draw.h"
#include "../../global/tg_global_log.h"

/*! \brief TgImageDrawPath::draw
 * draws path to imageData
 * \param imageData [in/out] draws path to this image data
 * \param imageWidth width of imageData
 * \param imageHeight height of imageData
 * \param r, g, b, a path color to add
 * \param lineWidth path's width
 */
bool TgImageDrawPath::draw(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
              const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a,
              const uint32_t lineWidth)
{
    size_t i;
    uint32_t x, y, ip;
    float startPointPositionX[4];
    float startPointPositionY[4];
    float endPointPositionX[4];
    float endPointPositionY[4];
    float crossPointX[2], crossPointY[2];
    bool result[8];

    switch (m_pathAA) {
        case TgChartLineAA::TgChartLineAA_4:
        default:
            m_imageFiller.init(imageWidth*4, imageHeight);
            break;
        case TgChartLineAA::TgChartLineAA_8:
            m_imageFiller.init(imageWidth*8, imageHeight);
            break;
    }


    for (i=1;i<m_listPoint.size();i++) {
        if (i == 1) {
            TgMath2d::rotateByAngle(m_listPoint[i-1].m_x, m_listPoint[i-1].m_y, m_listPoint[i].m_x, m_listPoint[i].m_y,
                static_cast<float>(M_PI)/-2.0f, static_cast<float>(lineWidth)/2.0f,
                &startPointPositionX[0], &startPointPositionY[0]);
            TgMath2d::rotateByAngle(m_listPoint[i-1].m_x, m_listPoint[i-1].m_y, m_listPoint[i].m_x, m_listPoint[i].m_y,
                static_cast<float>(M_PI)/2.0f, static_cast<float>(lineWidth)/2.0f,
                &startPointPositionX[2], &startPointPositionY[2]);
        } else {
            startPointPositionX[0] = crossPointX[0];
            startPointPositionY[0] = crossPointY[0];
            startPointPositionX[2] = crossPointX[1];
            startPointPositionY[2] = crossPointY[1];
        }

        if (i + 1 < m_listPoint.size()) {
            startPointPositionX[1] = startPointPositionX[0] + m_listPoint[i].m_x - m_listPoint[i-1].m_x;
            startPointPositionY[1] = startPointPositionY[0] + m_listPoint[i].m_y - m_listPoint[i-1].m_y;
            startPointPositionX[3] = startPointPositionX[2] + m_listPoint[i].m_x - m_listPoint[i-1].m_x;
            startPointPositionY[3] = startPointPositionY[2] + m_listPoint[i].m_y - m_listPoint[i-1].m_y;

            TgMath2d::rotateByAngle(m_listPoint[i].m_x, m_listPoint[i].m_y, m_listPoint[i+1].m_x, m_listPoint[i+1].m_y,
                    static_cast<float>(M_PI)/-2.0f, static_cast<float>(lineWidth)/2.0f,
                    &endPointPositionX[0], &endPointPositionY[0]);
            TgMath2d::rotateByAngle(m_listPoint[i].m_x, m_listPoint[i].m_y, m_listPoint[i+1].m_x, m_listPoint[i+1].m_y,
                    static_cast<float>(M_PI)/2.0f, static_cast<float>(lineWidth)/2.0f,
                    &endPointPositionX[2], &endPointPositionY[2]);

            endPointPositionX[1] = endPointPositionX[0] + m_listPoint[i+1].m_x - m_listPoint[i].m_x;
            endPointPositionY[1] = endPointPositionY[0] + m_listPoint[i+1].m_y - m_listPoint[i].m_y;
            endPointPositionX[3] = endPointPositionX[2] + m_listPoint[i+1].m_x - m_listPoint[i].m_x;
            endPointPositionY[3] = endPointPositionY[2] + m_listPoint[i+1].m_y - m_listPoint[i].m_y;

            TgMath2d::calculateCrossPoint(startPointPositionX[0], startPointPositionY[0], startPointPositionX[1], startPointPositionY[1],
                                          endPointPositionX[0], endPointPositionY[0], endPointPositionX[1], endPointPositionY[1],
                                          &crossPointX[0], &crossPointY[0]);
            TgMath2d::calculateCrossPoint(startPointPositionX[2], startPointPositionY[2], startPointPositionX[3], startPointPositionY[3],
                                          endPointPositionX[2], endPointPositionY[2], endPointPositionX[3], endPointPositionY[3],
                                          &crossPointX[1], &crossPointY[1]);
        } else {
            TgMath2d::rotateByAngle(m_listPoint[i].m_x, m_listPoint[i].m_y, m_listPoint[i-1].m_x, m_listPoint[i-1].m_y,
                static_cast<float>(M_PI)/2.0f, static_cast<float>(lineWidth)/2.0f,
                &crossPointX[0], &crossPointY[0]);
            TgMath2d::rotateByAngle(m_listPoint[i].m_x, m_listPoint[i].m_y, m_listPoint[i-1].m_x, m_listPoint[i-1].m_y,
                static_cast<float>(M_PI)/-2.0f, static_cast<float>(lineWidth)/2.0f,
                &crossPointX[1], &crossPointY[1]);
        }

        for (x=0;x<imageWidth;x++) {
            for (y=0;y<imageHeight;y++) {
                switch (m_pathAA) {
                    case TgChartLineAA::TgChartLineAA_4:
                    default:
                        TgMath2d::isInQuadrilateralAA4(
                            static_cast<float>(x), static_cast<float>(y),
                            startPointPositionX[0], startPointPositionY[0],
                            crossPointX[0], crossPointY[0],
                            crossPointX[1], crossPointY[1],
                            startPointPositionX[2], startPointPositionY[2],
                            result);
                        for (ip=0;ip<4;ip++) {
                            if (result[ip]) {
                                m_imageFiller.set(x*4+ip, y, 1);
                            }
                        }
                        break;
                    case TgChartLineAA::TgChartLineAA_8:
                        TgMath2d::isInQuadrilateralAA8(
                            static_cast<float>(x), static_cast<float>(y),
                            startPointPositionX[0], startPointPositionY[0],
                            crossPointX[0], crossPointY[0],
                            crossPointX[1], crossPointY[1],
                            startPointPositionX[2], startPointPositionY[2],
                            result);
                        for (ip=0;ip<8;ip++) {
                            if (result[ip]) {
                                m_imageFiller.set(x*4+ip, y, 1);
                            }
                        }
                        break;
                }
            }
        }
    }

    generateImageData(imageData, imageWidth, imageHeight, r, g, b, a);

    return false;
}

/*! \brief TgImageDrawPath::generateImageData
 * generates data from m_imageFiller to image data
 * \param imageData [in/out] generated data will be filled here
 * \param imageWidth width of imageData
 * \param imageHeight height of imageData
 * \param r, g, b, a path color to add
 */
void TgImageDrawPath::generateImageData(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                                        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    uint32_t i;
    uint32_t x, y;
    uint8_t value;
    for (x=0;x<imageWidth;x++) {
        for (y=0;y<imageHeight;y++) {
            switch (m_pathAA) {
                case TgChartLineAA::TgChartLineAA_4:
                default:
                    value = m_imageFiller.get(x*4, y);
                    for (i=1;i<4;i++) {
                        value += m_imageFiller.get(x*4+i, y);
                    }

                    switch (value) {
                        case 0:
                            break;
                        case 1:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/4);
                            break;
                        case 2:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/2);
                            break;
                        case 3:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a-(a/4));
                            break;
                        case 4:
                        default:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a);
                            break;
                    }
                    break;
                case TgChartLineAA::TgChartLineAA_8:
                    value = m_imageFiller.get(x*8, y);
                    for (i=1;i<8;i++) {
                        value += m_imageFiller.get(x*8+i, y);
                    }

                    switch (value) {
                        case 0:
                            break;
                        case 1:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/8);
                            break;
                        case 2:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/4);
                            break;
                        case 3:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, static_cast<uint8_t>(static_cast<uint32_t>(a)*3/8) );
                            break;
                        case 4:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/2);
                            break;
                        case 5:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, static_cast<uint8_t>(static_cast<uint32_t>(a)*3/8) );
                            break;
                        case 6:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a - a/4 );
                            break;
                        case 7:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a-(a/8));
                            break;
                        case 8:
                        default:
                            TgImageDraw::addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a);
                            break;
                    }                
                    break;
            }
        }
    }
}

int32_t TgImageDrawPath::getMin(float startPointPosition[4],
    float endPointPosition[4],
    float crossPoint[2])
{
    int32_t min = static_cast<int32_t>(startPointPosition[0]);
    min = std::min(static_cast<int32_t>(startPointPosition[1]), min);
    min = std::min(static_cast<int32_t>(startPointPosition[2]), min);
    min = std::min(static_cast<int32_t>(startPointPosition[3]), min);
    min = std::min(static_cast<int32_t>(crossPoint[0]), min);
    min = std::min(static_cast<int32_t>(crossPoint[1]), min);
    min = std::min(static_cast<int32_t>(endPointPosition[0]), min);
    min = std::min(static_cast<int32_t>(endPointPosition[1]), min);
    min = std::min(static_cast<int32_t>(endPointPosition[2]), min);
    min = std::min(static_cast<int32_t>(endPointPosition[3]), min);
    return min;
}

int32_t TgImageDrawPath::getMax(float startPointPosition[4],
    float endPointPosition[4],
    float crossPoint[2])
{
    int32_t max = static_cast<int32_t>(startPointPosition[0]);
    max = std::max(static_cast<int32_t>(startPointPosition[1]), max);
    max = std::max(static_cast<int32_t>(startPointPosition[2]), max);
    max = std::max(static_cast<int32_t>(startPointPosition[3]), max);
    max = std::max(static_cast<int32_t>(crossPoint[0]), max);
    max = std::max(static_cast<int32_t>(crossPoint[1]), max);
    max = std::max(static_cast<int32_t>(endPointPosition[0]), max);
    max = std::max(static_cast<int32_t>(endPointPosition[1]), max);
    max = std::max(static_cast<int32_t>(endPointPosition[2]), max);
    max = std::max(static_cast<int32_t>(endPointPosition[3]), max);
    return max;
}

/*! \brief TgImageDrawPath::init
 * add new point to path
 * \param point new point to add
 */
void TgImageDrawPath::addPoint(const TgImageDrawPathPoint &point)
{
    if (m_listPoint.size() <= 1) {
        m_listPoint.push_back(point);
        return;
    }

    float xDiff0 = m_listPoint.back().m_x - m_listPoint[m_listPoint.size()-2].m_x;
    float yDiff0 = m_listPoint.back().m_y - m_listPoint[m_listPoint.size()-2].m_y;
    float xDiff1 = point.m_x - m_listPoint[m_listPoint.size()-2].m_x;
    float yDiff1 = point.m_y - m_listPoint[m_listPoint.size()-2].m_y;

    if ( (yDiff0 >= 0 && yDiff1 < 0) || (yDiff0 < 0 && yDiff1 >= 0) ) {
        m_listPoint.push_back(point);
        return;
    }

    if ( (xDiff0 >= 0 && xDiff1 < 0) || (xDiff0 < 0 && xDiff1 >= 0) ) {
        m_listPoint.push_back(point);
        return;
    }

    if (std::fabs(TgMath2d::calculateAngle(xDiff0, yDiff0) - TgMath2d::calculateAngle(xDiff1, yDiff1))  <= 0.000001f) {
        m_listPoint.back().m_x = point.m_x;
        m_listPoint.back().m_y = point.m_y;
        return;
    }
    m_listPoint.push_back(point);
}

/*! \brief TgImageDrawPath::clearPoints
 * clear all points
 */
void TgImageDrawPath::clearPoints()
{
    m_listPoint.clear();
}
