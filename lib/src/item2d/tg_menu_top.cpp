/*!
 * \file
 * \brief file tg_menu_top.cpp
 *
 * Top menu
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_menu_top.h"
#include "private/tg_menu_top_private.h"
#include "../global/tg_global_log.h"
#include "private/item2d/tg_item2d_private.h"
#include "../global/private/tg_global_defines.h"

TgMenuTop::TgMenuTop(TgItem2d *parent) :
     TgItem2d(parent, 0, 0, parent->getWidth(), TG_MENU_DEFAULT_HEIGHT, true),
     m_private(new TgMenuTopPrivate(this, parent))
{
}

TgMenuTop::~TgMenuTop()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*! \brief TgMenuTop::addMenu
 * \param text
 * \return pointer of created menu item, nullptr if create is failed
 */
TgMenuItem *TgMenuTop::addMenu(const char *text)
{
     TgShortCut shortCut;
     return addMenu(text, &shortCut);
}

/*! \brief TgMenuTop::addMenu
 * \param text
 * \param shortCut
 * \return pointer of created menu item, nullptr if create is failed
 */
TgMenuItem *TgMenuTop::addMenu(const char *text, const TgShortCut *shortCut)
{
     return m_private->addMenu(text, shortCut);
}

/*!
 * \brief TgMenuTop::checkOnResizeChanged
 *
 * checks if position or size is changed, and sends
 * possible resizeChanged callback
 */
void TgMenuTop::checkOnResizeChanged()
{
     m_private->checkOnResizeChanged();
     TgItem2d::checkOnResizeChanged();
}

/*!
 * \brief TgMenuTop::getMenuCount
 *
 * \return menu count
 */
size_t TgMenuTop::getMenuCount()
{
     return m_private->getMenuCount();
}

/*!
 * \brief TgMenuTop::removeMenu
 *
 * \param i index of menu
 * \return true if menu was removed
 */
bool TgMenuTop::removeMenu(size_t i)
{
     return m_private->removeMenu(i);
}

/*!
 * \brief TgMenuTop::checkPositionValuesChildren
 *
 * check position values to children
 * \param windowInfo
 */
void TgMenuTop::checkPositionValuesChildren(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    TgItem2d::checkPositionValuesChildren(windowInfo);
    m_private->checkPositionValuesMenu();
    TG_FUNCTION_END();
}