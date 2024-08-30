/*!
 * \file
 * \brief file tg_pages_private.h
 *
 * it holds general TgPagesPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_PAGES_PRIVATE_H
#define TG_PAGES_PRIVATE_H

#include "../../math/tg_matrix4x4.h"
#include <vector>
#include "../tg_pages.h"
#include "../../common/time_difference.h"
#include "item2d/tg_item2d_position.h"

class TgPagesPrivate : public TgItem2dPositionInternalResize
{
public:
    explicit TgPagesPrivate(TgItem2d *currentItem);
    virtual ~TgPagesPrivate();

    void addPage(TgPage *page);

    size_t getCurrentPageIndex() const;
    size_t getPageCount() const;

    void setPage(size_t pageIndex);
    void setPageSwitchType(TgPagesPageSwitchType type);
    void checkPositionValues();
    void setPageSwitchTime(double pageSwitchTime);

    virtual void onInternalResize(float x, float y, float width, float height) override;

    void internalChangePage();
private:
    std::vector<TgPage *>m_listPage;
    size_t m_currentPageIndex;
    size_t m_toCurrentPageIndex;
    double m_pageSwitchMaxTime;
    TgPagesPageSwitchType m_pageSwitchType;
    TimeDifference m_pageSwitchStartTime;
};

#endif // TG_PAGES_PRIVATE_H
