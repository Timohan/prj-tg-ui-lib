/*!
 * \file
 * \brief file tg_grid_view_private.h
 *
 * it holds general TgGridViewPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GRID_VIEW_PRIVATE_H
#define TG_GRID_VIEW_PRIVATE_H

#include <cstddef>
#include "../../tg_rectangle.h"
#include "../../tg_slider.h"
#include "../../tg_grid_view_cell.h"
class TgGridView;

class TgGridViewPrivate
{
public:
    TgGridViewPrivate(TgGridView *currentItem, size_t columnCount, size_t rowCount);
    ~TgGridViewPrivate();
    size_t getColumCount();
    size_t getRowCount();
    void setRowCount(size_t row);
    void setColumCount(size_t column);
    TgGridViewCell *getCell(size_t column, size_t row);
    void cellWidthTypeChanged(TgGridViewCellSizeType type, TgGridViewCell *cell);
    void widthCellChanged(float width, TgGridViewCell *cell);
    void heightCellChanged(float height, TgGridViewCell *cell);

    void setSliderVisibilityAndPosition();
private:
    TgGridView *m_currentItem;
    TgRectangle m_background;
    TgRectangle m_backgroundVerticalSlider;
    TgRectangle m_backgroundHorizontalSlider;
    TgSlider m_verticalSlider;
    TgSlider m_horizontalSlider;

    std::vector<TgGridViewCell *>m_listItems;
    size_t m_columnCount = 0;
    size_t m_rowCount = 0;
    size_t m_maxInnerAreaWidth = 0;
    size_t m_maxInnerAreaHeight = 0;
    uint64_t m_previousVerticalSliderPosition = 0;
    uint64_t m_previousHorizontalSliderPosition = 0;

    void setGridCellsPositions();
    void onVerticalSliderPositionChanged(uint64_t position);
    void onHorizontalSliderPositionChanged(uint64_t position);
    bool isSliderRequired(bool horizontal);
    bool setSliderVisibilityAndPositionHor();
    bool setSliderVisibilityAndPositionVer();
    void clear();
};

#endif // TG_GRID_VIEW_PRIVATE_H
