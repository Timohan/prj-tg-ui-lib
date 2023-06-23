/*!
 * \file
 * \brief file tg_grid_view_private.cpp
 *
 * it holds general TgGridViewPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_grid_view_private.h"
#include "../../../global/tg_global_log.h"
#include "../../tg_grid_view.h"
#include "../../tg_grid_view_cell.h"
#include "../../../global/private/tg_global_deleter.h"
#include "../../../global/private/tg_global_wait_renderer.h"
#include "../item2d/tg_item2d_private.h"
#include "tg_grid_view_cell_private.h"

#define DEFAULT_GRID_CELL_WIDTH     100
#define DEFAULT_GRID_CELL_HEIGHT    25
#define DEFAULT_GRID_CELL_BORDER    1
#define DEFAULT_GRID_VIEW_SLIDER    15

TgGridViewPrivate::TgGridViewPrivate(TgGridView *currentItem, size_t columnCount, size_t rowCount) :
    m_currentItem(currentItem),
    m_background(currentItem, 128, 128, 128),
    m_backgroundVerticalSlider(currentItem, 0, 0, DEFAULT_GRID_VIEW_SLIDER, DEFAULT_GRID_VIEW_SLIDER, 128, 128, 128),
    m_backgroundHorizontalSlider(currentItem, 0, 0, DEFAULT_GRID_VIEW_SLIDER, DEFAULT_GRID_VIEW_SLIDER, 128, 128, 128),
    m_verticalSlider(currentItem, 0, 0, DEFAULT_GRID_VIEW_SLIDER, 100, TgSliderType::SliderType_Vertical),
    m_horizontalSlider(currentItem, 0, 0, 100, DEFAULT_GRID_VIEW_SLIDER, TgSliderType::SliderType_Horizontal),
    m_columnCount(columnCount),
    m_rowCount(rowCount)
{
    TG_FUNCTION_BEGIN();
    size_t x, y;
    for (y=0;y<rowCount;y++) {
        for (x=0;x<columnCount;x++) {
            m_listItems.push_back(new TgGridViewCell(currentItem,
                static_cast<float>(x*(DEFAULT_GRID_CELL_WIDTH+DEFAULT_GRID_CELL_BORDER)+DEFAULT_GRID_CELL_BORDER),
                static_cast<float>(y*(DEFAULT_GRID_CELL_HEIGHT+DEFAULT_GRID_CELL_BORDER)+DEFAULT_GRID_CELL_BORDER),
                static_cast<float>(DEFAULT_GRID_CELL_WIDTH),
                static_cast<float>(DEFAULT_GRID_CELL_HEIGHT) ));
        }
    }
    setSliderVisibilityAndPosition();
    m_verticalSlider.connectOnSliderPositionChanged( std::bind(&TgGridViewPrivate::onVerticalSliderPositionChanged, this, std::placeholders::_1) );
    m_horizontalSlider.connectOnSliderPositionChanged( std::bind(&TgGridViewPrivate::onHorizontalSliderPositionChanged, this, std::placeholders::_1) );
    TG_FUNCTION_END();
}

TgGridViewPrivate::~TgGridViewPrivate()
{
    std::vector<TgGridViewCell *>::iterator it;
    for (it=m_listItems.begin();it!=m_listItems.end();it++) {
        if (*it) {
            delete *it;
        }
    }
    m_listItems.clear();
}

/*! \brief TgGridViewPrivate::onVerticalSliderPositionChanged
 * this is callback from vertical slider, when it's position is changed
 * \param position new position
 */
void TgGridViewPrivate::onVerticalSliderPositionChanged(uint64_t position)
{
    TG_FUNCTION_BEGIN();
    if (m_previousVerticalSliderPosition == position) {
        TG_FUNCTION_END();
        return;
    }
    setGridCellsPositions();
    TgGlobalWaitRenderer::getInstance()->release();
    m_previousVerticalSliderPosition = position;
    TG_FUNCTION_END();
}

/*! \brief TgGridViewPrivate::onHorizontalSliderPositionChanged
 * this is callback from horizontal slider, when it's position is changed
 * \param position new position
 */
void TgGridViewPrivate::onHorizontalSliderPositionChanged(uint64_t position)
{
    TG_FUNCTION_BEGIN();
    if (m_previousHorizontalSliderPosition == position) {
        TG_FUNCTION_END();
        return;
    }
    setGridCellsPositions();
    TgGlobalWaitRenderer::getInstance()->release();
    m_previousHorizontalSliderPosition = position;
    TG_FUNCTION_END();
}

/*! \brief TgGridViewPrivate::setSliderVisibilityAndPosition
 * sets position and visiblity for sliders
 */
void TgGridViewPrivate::setSliderVisibilityAndPosition()
{
    TG_FUNCTION_BEGIN();
    if (m_columnCount == 0 || m_rowCount == 0) {
        m_verticalSlider.setSliderMaxPosition(0);
        m_horizontalSlider.setSliderMaxPosition(0);
        m_verticalSlider.setVisible(false);
        m_horizontalSlider.setVisible(false);
        TG_FUNCTION_END();
        return;
    }
    TgGridViewCell *tmp = getCell(m_columnCount-1, 0);
    m_maxInnerAreaWidth = static_cast<size_t>(tmp->getX() + tmp->getWidth() + DEFAULT_GRID_CELL_BORDER) - static_cast<size_t>(getCell(0, 0)->getX() - DEFAULT_GRID_CELL_BORDER);
    tmp = getCell(0, m_rowCount-1);
    m_maxInnerAreaHeight = static_cast<size_t>(tmp->getY() + tmp->getHeight() + DEFAULT_GRID_CELL_BORDER) - static_cast<size_t>(getCell(0, 0)->getY() - DEFAULT_GRID_CELL_BORDER);

    bool hor = setSliderVisibilityAndPositionHor();
    bool ver = setSliderVisibilityAndPositionVer();

    if (hor && ver) {
        m_backgroundVerticalSlider.setToTop();
        m_backgroundHorizontalSlider.setToTop();
        m_verticalSlider.setToTop();
        m_horizontalSlider.setToTop();
    } else if (hor) {
        m_backgroundHorizontalSlider.setToTop();
        m_horizontalSlider.setToTop();
        m_verticalSlider.setSliderMaxPosition(0);
    } else if (ver) {
        m_backgroundVerticalSlider.setToTop();
        m_verticalSlider.setToTop();
        m_horizontalSlider.setSliderMaxPosition(0);
    }
    TG_FUNCTION_END();
}

void TgGridViewPrivate::setGridCellsPositions()
{
    TgGridViewCell *tmp;
    size_t x, y;
    float w, h = DEFAULT_GRID_CELL_BORDER;
    for (y=0;y<m_rowCount;y++) {
        w = DEFAULT_GRID_CELL_BORDER;
        for (x=0;x<m_columnCount;x++) {
            tmp = getCell(x, y);
            tmp->TgItem2d::setX( w - static_cast<float>(m_horizontalSlider.getSliderCurrentPosition() ) );
            tmp->TgItem2d::setY( h - static_cast<float>(  m_verticalSlider.getSliderCurrentPosition() ) );
            w += tmp->getWidth() + DEFAULT_GRID_CELL_BORDER;
        }
        tmp = getCell(x-1, y);
        if (tmp) {
            h += tmp->getHeight() + DEFAULT_GRID_CELL_BORDER;
        }
    }
}

bool TgGridViewPrivate::isSliderRequired(bool horizontal)
{
    if (horizontal) {
        return m_currentItem->getWidth() < static_cast<float>(m_maxInnerAreaWidth);
    }
    return m_currentItem->getHeight() < static_cast<float>(m_maxInnerAreaHeight);
}

bool TgGridViewPrivate::setSliderVisibilityAndPositionHor()
{
    if (!isSliderRequired(true)) {
        m_horizontalSlider.setSliderMaxPosition(0);
        m_horizontalSlider.setVisible(false);
        m_backgroundHorizontalSlider.setVisible(false);
        return false;
    }
    float w = m_currentItem->getWidth();
    float h = m_currentItem->getHeight();

    m_backgroundHorizontalSlider.TgItem2d::setY(h-DEFAULT_GRID_VIEW_SLIDER);
    m_backgroundHorizontalSlider.setWidth(w);
    m_horizontalSlider.TgItem2d::setY(h-DEFAULT_GRID_VIEW_SLIDER);
    m_horizontalSlider.setWidth(w-DEFAULT_GRID_VIEW_SLIDER);

    if (isSliderRequired(false)) {
        m_horizontalSlider.setSliderMaxPosition(m_maxInnerAreaWidth+DEFAULT_GRID_VIEW_SLIDER-static_cast<uint64_t>(w));
    } else {
        m_horizontalSlider.setSliderMaxPosition(m_maxInnerAreaWidth-static_cast<uint64_t>(w));
    }
    m_horizontalSlider.setVisible(true);
    m_backgroundHorizontalSlider.setVisible(true);
    return true;
}

bool TgGridViewPrivate::setSliderVisibilityAndPositionVer()
{
    if (!isSliderRequired(false)) {
        m_verticalSlider.setSliderMaxPosition(0);
        m_verticalSlider.setVisible(false);
        m_backgroundVerticalSlider.setVisible(false);
        return false;
    }

    float w = m_currentItem->getWidth();
    float h = m_currentItem->getHeight();

    m_backgroundVerticalSlider.TgItem2d::setX(w-DEFAULT_GRID_VIEW_SLIDER);
    m_backgroundVerticalSlider.TgItem2d::setHeight(h);
    m_verticalSlider.TgItem2d::setX(w-DEFAULT_GRID_VIEW_SLIDER);
    m_verticalSlider.setHeight(h-DEFAULT_GRID_VIEW_SLIDER);

    if (isSliderRequired(true)) {
        m_verticalSlider.setSliderMaxPosition(m_maxInnerAreaHeight+DEFAULT_GRID_VIEW_SLIDER-static_cast<uint64_t>(h));
    } else {
        m_verticalSlider.setSliderMaxPosition(m_maxInnerAreaHeight-static_cast<uint64_t>(h));
    }
    m_verticalSlider.setVisible(true);
    m_backgroundVerticalSlider.setVisible(true);
    return true;
}

/*! \brief TgGridViewPrivate::getColumCount
 * \return column count
 */
size_t TgGridViewPrivate::getColumCount()
{
    return m_columnCount;
}

/*! \brief TgGridViewPrivate::getRowCount
 * \return row count
 */
size_t TgGridViewPrivate::getRowCount()
{
    return m_rowCount;
}

/*! \brief TgGridViewPrivate::getCell
 *
 * get pointer of item
 *
 * \param column
 * \param row
 * \return nullptr if item doesn't exists (row or column is too large)
 * otherwise returns pointer of item
 */
TgGridViewCell *TgGridViewPrivate::getCell(size_t column, size_t row)
{
    TG_FUNCTION_BEGIN();
    if (column >= m_columnCount || row >= m_rowCount) {
        TG_FUNCTION_END();
        return nullptr;
    }
    TG_FUNCTION_END();
    return m_listItems[row*m_columnCount+column];
}

/*! \brief TgGridViewPrivate::setRowCount
 * \param row
 */
void TgGridViewPrivate::setRowCount(size_t row)
{
    TG_FUNCTION_BEGIN();
    if (row == m_rowCount) {
        TG_FUNCTION_END();
        return;
    }
    if (row == 0) {
        clear();
        m_rowCount = row;
        setGridCellsPositions();
        setSliderVisibilityAndPosition();
        TG_FUNCTION_END();
        return;
    }
    std::vector<TgGridViewCell *>tmpList;
    for (size_t i=0;i<m_listItems.size();i++) {
        tmpList.push_back(m_listItems.at(i));
    }
    size_t x, y;
    float posX, posY, newWidth;
    TgGridViewCell *cell;
    m_listItems.clear();
    if (row > m_rowCount) {
        for (y=0;y<row;y++) {
            for (x=0;x<m_columnCount;x++) {
                if (y < m_rowCount) {
                    m_listItems.push_back(tmpList[y*m_columnCount+x]);
                    continue;
                }
                if (x == 0) {
                    posX = DEFAULT_GRID_CELL_BORDER;
                } else {
                    cell = m_listItems[y*m_columnCount+x-1];
                    posX = cell->getX() + cell->getWidth() + DEFAULT_GRID_CELL_BORDER;
                }
                if (y == 0) {
                    posY = DEFAULT_GRID_CELL_BORDER;
                    newWidth = DEFAULT_GRID_CELL_WIDTH;
                } else {
                    cell = m_listItems[ (y-1)*m_columnCount+x];
                    posY = cell->getY() + cell->getHeight() + DEFAULT_GRID_CELL_BORDER;
                    newWidth = cell->getWidth();
                }
                m_listItems.push_back(new TgGridViewCell(m_currentItem,
                        posX, posY,
                        newWidth, static_cast<float>(DEFAULT_GRID_CELL_HEIGHT) ));
            }
        }
    } else {
        for (y=0;y<row;y++) {
            for (x=0;x<m_columnCount;x++) {
                m_listItems.push_back(tmpList[y*m_columnCount+x]);
            }
        }
        for (y=row;y<m_rowCount;y++) {
            for (x=0;x<m_columnCount;x++) {
                tmpList[y*m_columnCount+x]->setVisible(false);
                TgGlobalDeleter::getInstance()->add(tmpList[y*m_columnCount+x]);
            }
        }
    }
    tmpList.clear();
    m_rowCount = row;
    setGridCellsPositions();
    setSliderVisibilityAndPosition();
    static_cast<TgItem2d *>(m_currentItem)->setPositionChanged(true);
    TG_FUNCTION_END();
}

void TgGridViewPrivate::clear()
{
    TG_FUNCTION_BEGIN();
    for (size_t i=0;i<m_listItems.size();i++) {
        m_listItems[i]->setVisible(false);
        TgGlobalDeleter::getInstance()->add(m_listItems[i]);
    }
    m_listItems.clear();
    TG_FUNCTION_END();
}

/*! \brief TgGridViewPrivate::setColumCount
 * \param column count
 */
void TgGridViewPrivate::setColumCount(size_t column)
{
    TG_FUNCTION_BEGIN();
    if (column == m_columnCount) {
        TG_FUNCTION_END();
        return;
    }
    if (column == 0) {
        clear();
        m_columnCount = column;
        setGridCellsPositions();
        setSliderVisibilityAndPosition();
        TG_FUNCTION_END();
        return;
    }
    std::vector<TgGridViewCell *>tmpList;
    for (size_t i=0;i<m_listItems.size();i++) {
        tmpList.push_back(m_listItems.at(i));
    }
    size_t x, y;
    float posX, posY;
    float newHeight;
    TgGridViewCell *cell;
    m_listItems.clear();
    if (column > m_columnCount) {
        for (y=0;y<m_rowCount;y++) {
            for (x=0;x<column;x++) {
                if (x < m_columnCount) {
                    m_listItems.push_back(tmpList[y*m_columnCount+x]);
                    continue;
                }
                if (x == 0) {
                    posX = DEFAULT_GRID_CELL_BORDER;
                    newHeight = DEFAULT_GRID_CELL_HEIGHT;
                } else {
                    cell = m_listItems[y*column+x-1];
                    posX = cell->getX() + cell->getWidth() + DEFAULT_GRID_CELL_BORDER;
                    newHeight = cell->getHeight();
                }
                if (y == 0) {
                    posY = DEFAULT_GRID_CELL_BORDER;
                } else {
                    cell = m_listItems[ (y-1)*column+x];
                    posY = cell->getY() + cell->getHeight() + DEFAULT_GRID_CELL_BORDER;
                }
                m_listItems.push_back(new TgGridViewCell(m_currentItem,
                        posX, posY,
                        static_cast<float>(DEFAULT_GRID_CELL_WIDTH),
                        newHeight ));
            }
        }
    } else {
        for (y=0;y<m_rowCount;y++) {
            for (x=0;x<column;x++) {
                m_listItems.push_back(tmpList[y*m_columnCount+x]);
            }
        }
        for (y=0;y<m_rowCount;y++) {
            for (x=column;x<m_columnCount;x++) {
                tmpList[y*m_columnCount+x]->setVisible(false);
                TgGlobalDeleter::getInstance()->add(tmpList[y*m_columnCount+x]);
            }
        }
    }
    tmpList.clear();
    m_columnCount = column;
    setGridCellsPositions();
    setSliderVisibilityAndPosition();
    static_cast<TgItem2d *>(m_currentItem)->setPositionChanged(true);
    TG_FUNCTION_END();
}

/*! \brief TgGridViewPrivate::cellWidthTypeChanged
 * when cell type is changed, this is called
 * \param type new widtht of the cell
 * \param cell
 */
void TgGridViewPrivate::cellWidthTypeChanged(TgGridViewCellSizeType type, TgGridViewCell *cell)
{
    TG_FUNCTION_BEGIN();
    size_t x, y, y2;
    float fRequiredWidth, w;
    for (y=0;y<m_rowCount;y++) {
        for (x=0;x<m_columnCount;x++) {
            if (getCell(x, y) == cell) {
                fRequiredWidth = cell->m_private->getCellRequiredWidth();
                if (type == TgGridViewCellSizeType::TgGridViewCellSize_SizeFollowTextSize) {
                    for (y2=0;y2<m_rowCount;y2++) {
                        if (y != y2) {
                            w = getCell(x, y2)->m_private->getCellRequiredWidth();
                            if (w > fRequiredWidth) {
                                fRequiredWidth = w;
                            }
                        }
                    }
                }
                for (y2=0;y2<m_rowCount;y2++) {
                    reinterpret_cast<TgItem2d *>(getCell(x, y2))->m_private->setWidth(fRequiredWidth, false);
                    getCell(x, y2)->m_private->setWidthType(type, false);
                }
                setGridCellsPositions();
                m_currentItem->setPositionChanged(true);
                TG_FUNCTION_END();
                return;
            }
        }
    }
    TG_FUNCTION_END();
}

/*! \brief TgGridViewPrivate::widthCellChanged
 * when cell width is changed, this is called
 * \param width new widtht of the cell
 * \param cell
 */
void TgGridViewPrivate::widthCellChanged(float width, TgGridViewCell *cell)
{
    TG_FUNCTION_BEGIN();
    size_t x, y, y2;
    for (y=0;y<m_rowCount;y++) {
        for (x=0;x<m_columnCount;x++) {
            if (getCell(x, y) == cell) {
                for (y2=0;y2<m_rowCount;y2++) {
                    if (y != y2) {
                        reinterpret_cast<TgItem2d *>(getCell(x, y2))->m_private->setWidth(width, false);
                    }
                }
                setGridCellsPositions();
                m_currentItem->setPositionChanged(true);
                TG_FUNCTION_END();
                return;
            }
        }
    }
    TG_FUNCTION_END();
}

/*! \brief TgGridViewPrivate::heightCellChanged
 * when cell width is changed, this is called
 * \param width new widtht of the cell
 * \param cell
 */
void TgGridViewPrivate::heightCellChanged(float height, TgGridViewCell *cell)
{
    TG_FUNCTION_BEGIN();
    size_t x, y, x2;
    for (y=0;y<m_rowCount;y++) {
        for (x=0;x<m_columnCount;x++) {
            if (getCell(x, y) == cell) {
                for (x2=0;x2<m_columnCount;x2++) {
                    if (x != x2) {
                        reinterpret_cast<TgItem2d *>(getCell(x2, y))->m_private->setHeight(height, false);
                    }
                }
                setGridCellsPositions();
                m_currentItem->setPositionChanged(true);
                TG_FUNCTION_END();
                return;
            }
        }
    }
    TG_FUNCTION_END();
}
