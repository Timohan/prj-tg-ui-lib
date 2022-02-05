/*!
 * \file
 * \brief file tg_page_private.h
 *
 * it holds general TgPagePrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_PAGE_PRIVATE_H
#define TG_PAGE_PRIVATE_H

#include "../../math/tg_matrix4x4.h"
#include "../../global/private/tg_global_defines.h"
#include <string>
#include "../tg_pages.h"

class TgPage;

enum TgPageMovingState
{
    TgPageMovinState_Moving = 0,
    TgPageMovinState_NotMoving,
    TgPageMovinState_StopMoving
};

class TgPagePrivate
{
public:
    explicit TgPagePrivate(TgPage *parent);
    ~TgPagePrivate();

    void setPageOutsideParent();
    void setPageOnTopOfParent();
    void setPageToMoving(TgPagesPageSwitchType type);
    TgPageMovingState pageMoving(double elapsedTime, double maxTime);

private:
    TgPage *m_currentPage;
    bool m_pageSliding;
    float m_slidingStartX;
    float m_slidingStartY;
};

#endif // TG_PAGE_PRIVATE_H
