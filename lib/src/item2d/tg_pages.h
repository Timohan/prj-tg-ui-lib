/*!
 * \file
 * \brief file tg_pages.h
 *
 * Inits and handles the pages and switching pages
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_PAGES_H
#define TG_PAGES_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

struct TgWindowInfo;
class TgPagesPrivate;
class TgPage;

/*!
 * \brief TgPagesPageSwitchType
 * page change type
 */
enum TgPagesPageSwitchType
{
    PageSwitchType_Direct = 0,          /*!< page change directly without sliding animation */
    PageSwitchType_SlideFromTop,        /*!< page change from top */
    PageSwitchType_SlideFromBottom,     /*!< page change from bottom */
    PageSwitchType_SlideFromLeft,       /*!< page change from left */
    PageSwitchType_SlideFromRight,      /*!< page change from right */
};

/*!
 * \brief TgPages
 * handles the pages and switching pages
 * individual pares are TgPage
 */
class TG_MAINWINDOW_EXPORT TgPages : public TgItem2d
{
public:
    explicit TgPages(TgItem2d *parent);
    explicit TgPages(TgItem2d *parent, float x, float y, float width, float height);
    virtual ~TgPages();

    size_t getCurrentPageIndex() const;
    size_t getPageCount() const;

    void setPage(size_t pageIndex);
    void setPageSwitchType(TgPagesPageSwitchType type);
    void setPageSwitchTime(double pageSwitchTime);

protected:
    virtual void render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;

private:
    TgPagesPrivate *m_private;

    void addPage(TgPage *page);

    friend class TgPage;
};

#endif // TG_PAGES_H
