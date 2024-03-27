/*!
 * \file
 * \brief file tg_image_draw_path.h
 *
 * image draw path functionalities
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#ifndef TG_IMAGE_DRAW_PATH_H
#define TG_IMAGE_DRAW_PATH_H

#include <stdint.h>
#include <vector>
#include "tg_image_draw_filler.h"
#include "../../item2d/tg_chart.h"

struct TgImageDrawPathPoint
{
    TgImageDrawPathPoint(float x, float y) :
        m_x(x),
        m_y(y) {}
    float m_x;
    float m_y;
};

class TgImageDrawPath
{
public:
    bool draw(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
              const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a,
              const uint32_t lineWidth);
    void addPoint(const TgImageDrawPathPoint &point);
    void clearPoints();

protected:
    TgChartLineAA m_pathAA = TgChartLineAA::TgChartLineAA_4;

private:
    std::vector<TgImageDrawPathPoint> m_listPoint;
    TgImageDrawFiller m_imageFiller;
    static int32_t getMin(float startPointPosition[4],
        float endPointPosition[4],
        float crossPoint[2]);
    static int32_t getMax(float startPointPosition[4],
        float endPointPosition[4],
        float crossPoint[2]);

    void generateImageData(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
    void fillPixelsFromQuadrilateralAA4(const uint32_t imageWidth, const uint32_t imageHeight,
                                        const float x0, const float y0,
                                        const float x1, const float y1,
                                        const float x2, const float y2,
                                        const float x3, const float y3);
    void fillPixelsFromQuadrilateralAA8(const uint32_t imageWidth, const uint32_t imageHeight,
                                        const float x0, const float y0,
                                        const float x1, const float y1,
                                        const float x2, const float y2,
                                        const float x3, const float y3);

    static uint32_t getStartValueMin(float v0, float v1, float v2, float v3);
    static uint32_t getStartValueMax(float v0, float v1, float v2, float v3, uint32_t maxValue);
};

#endif // TG_IMAGE_DRAW_PATH_H
