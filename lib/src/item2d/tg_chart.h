/*!
 * \file
 * \brief file tg_chart.h
 *
 * Inits and draws chart
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_CHART_H
#define TG_CHART_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

class TgChartPrivate;
struct TgWindowInfo;

struct TgChartPosition
{
    double x;
    double y;
};

/*! TgChartLineAA
 * anti-aliasing method to draw the paths
 */
enum TgChartLineAA {
    TgChartLineAA_4 = 0,    /*!< drawing similar as MSAA x4 */
    TgChartLineAA_8,        /*!< drawing similar as MSAA x8 */
};

/*!
 * \brief TgChart
 * draws chart
 */
class TG_MAINWINDOW_EXPORT TgChart : public TgItem2d
{
public:
    explicit TgChart(TgItem2d *parent, const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    explicit TgChart(TgItem2d *parent, float x, float y, float width, float height,
                         const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    ~TgChart();
    void setBackgroundColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255);
    void setGridLinesColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255);
    void setLineColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255);
    void setTextColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255);
    void setChartPosition(const std::vector<TgChartPosition> &listPosition);
    void addChartPosition(TgChartPosition position);
    void clear();

    uint32_t getBottomGridMargin() const;
    uint32_t getTopGridMargin() const;
    uint32_t getLeftGridMargin() const;
    uint32_t getRightGridMargin() const;

    void setBottomGridMargin(uint32_t gridBottomMargin);
    void setTopGridMargin(uint32_t gridTopMargin);
    void setLeftGridMargin(uint32_t gridLeftMargin);
    void setRightGridMargin(uint32_t gridRightMargin);

    void setXyDrawIsMaxedToSize(bool xyDrawIsMaxedToSize);
    bool getXyDrawIsMaxedToSize();

    void setFontSize(float fontSize);
    float getFontSize();

    void setChartResolution(int width, int height);
    int getChartResolutionWidth();
    int getChartResolutionHeight();

    void setDrawText(bool draw);
    bool getDrawText();

    void setLineWidth(uint32_t lineWidth);
    uint32_t getLineWidth();

    void setChartLineAA(TgChartLineAA aa);
    TgChartLineAA getChartLineAA();
protected:
    virtual bool render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;

private:
    TgChartPrivate *m_private;
};

#endif // TG_CHART_H
