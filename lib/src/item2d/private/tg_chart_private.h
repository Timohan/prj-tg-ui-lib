/*!
 * \file
 * \brief file tg_chart_private.h
 *
 * it holds general TgChartPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_CHART_PRIVATE_H
#define TG_CHART_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include <mutex>
#include <vector>
#include "../../math/tg_matrix4x4.h"
#include "../../render/tg_render.h"
#include "../tg_chart.h"
#include "../../font/image/tg_font_to_image.h"
#include "../../image/draw/tg_image_draw_path.h"

#define TG_CHART_DEFAULT_FONT_SIZE  16

class TgItem2d;
struct TgWindowInfo;
class TgItem2dPosition;

class TgChartPrivate : protected TgRender, private TgImageDrawPath
{
public:
    explicit TgChartPrivate(TgItem2d *parent, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    ~TgChartPrivate();
    bool render(const TgWindowInfo *windowInfo, TgItem2d *currentItem, TgItem2dPosition *itemPosition);
    void setBackgroundColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void setGridLinesColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void setLineColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void setTextColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void setTextFontSize(float fontSize);
    void setTextDefaultFont(const char *fontFile);
    void checkPositionValues(TgItem2d *currentItem);

    void setChartPosition(const std::vector<TgChartPosition> &position);
    void addChartPosition(const TgChartPosition &position);
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
    void setLineWidth(uint32_t lineWidth);
    uint32_t getLineWidth();

    void setChartResolution(int width, int height);
    int getChartResolutionWidth();
    int getChartResolutionHeight();

    void setDrawText(bool draw);
    bool getDrawText();

    void setChartLineAA(TgChartLineAA aa);
    TgChartLineAA getChartLineAA();
private:
    TgItem2d *m_parent;
    TgImageAsset m_imageAsset;
    TgFontToImage m_fontToImageMaxY;
    TgFontToImage m_fontToImageMinY;
    TgFontToImage m_fontToImageMaxX;
    TgFontToImage m_fontToImageMinX;
    std::string m_fontFile;
    float m_fontSize = TG_CHART_DEFAULT_FONT_SIZE;

    bool m_initVerticesDone;
    bool m_initImageAssetDone;
    bool m_textureRequiresReset = true;
    uint32_t m_gridBottomMargin = 20;
    uint32_t m_gridTopMargin = 10;
    uint32_t m_gridLeftMargin = 20;
    uint32_t m_gridRightMargin = 10;
    uint32_t m_textToGridMarginX = 5;
    uint32_t m_textToGridMarginY = 5;
    uint32_t m_lineWidth = 1;

    TgMatrix4x4 m_transform;
    bool m_xyDrawIsMaxedToSize = true;

    unsigned char m_BackgroundR = 255, m_BackgroundG = 255, m_BackgroundB = 255, m_BackgroundA = 255;
    unsigned char m_gridLinesR = 0, m_gridLinesG = 0, m_gridLinesB = 0, m_gridLinesA = 255;
    unsigned char m_lineR = 0, m_lineG = 0, m_lineB = 0, m_lineA = 255;
    unsigned char m_textR = 0, m_textG = 0, m_textB = 0, m_textA = 255;

    int m_chartResolutionWidth;
    int m_chartResolutionHeight;
    bool m_newDrawText = true;
    bool m_drawingTextOnImage = true;

    std::vector<TgChartPosition> m_listPosition;
    std::mutex m_mutex;

    bool init();
    void setTranform(TgItem2d *currentItem);
    GLuint getTextureIndex();

    void drawGrid();
    void drawBackground();
    void drawLines();
    void drawText();

    static void getMinMaxPosition(const std::vector<TgChartPosition>&listPosition, double &minX, double &minY, double &maxX, double &maxY);
    void getMinMaxPositionForChart(double &minX, double &minY, double &maxX, double &maxY);
    std::string getChartNumber(const std::string &number);
};

#endif // TG_CHART_PRIVATE_H
