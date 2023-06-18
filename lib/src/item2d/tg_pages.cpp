/*!
 * \file
 * \brief file tg_pages.cpp
 *
 * Inits and handles the pages and switching pages
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_pages.h"
#include "../global/tg_global_log.h"
#include "private/tg_pages_private.h"

/*!
 * \brief TgPages::TgPages
 * 
 * constructor to use AnchorFollowParentSize (check TgItem2D)
 *
 * \param parent
 */
TgPages::TgPages(TgItem2d *parent) :
    TgItem2d(parent),
    m_private(new TgPagesPrivate(this))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgPages::TgPages
 * 
 * constructor to use AnchorRelativeToParent (check TgItem2D)
 * position is relative to parent
 *
 * \param parent
 * \param x text field position x (relative to parent)
 * \param y text field position y (relative to parent)
 * \param width text field position width
 * \param height text field position height
 */
TgPages::TgPages(TgItem2d *parent, float x, float y, float width, float height) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgPagesPrivate(this))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgPages::~TgPages()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgPages::addPage
 *
 * add page to list of pages
 *
 * \param page add page
 */
void TgPages::addPage(TgPage *page)
{
    TG_FUNCTION_BEGIN();
    m_private->addPage(page);
    TG_FUNCTION_END();
}

/*!
 * \brief TgPages::getCurrentPageIndex
 *
 * get current page index
 *
 * \return current page index
 */
size_t TgPages::getCurrentPageIndex() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCurrentPageIndex();
}

/*!
 * \brief TgPages::getPageCount
 *
 * get page count
 *
 * \return page count
 */
size_t TgPages::getPageCount() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getPageCount();
}

/*!
 * \brief TgPages::setPage
 *
 * change page index
 *
 * \param pageIndex page index
 */
void TgPages::setPage(size_t pageIndex)
{
    TG_FUNCTION_BEGIN();
    m_private->setPage(pageIndex);
    TG_FUNCTION_END();
}

/*!
 * \brief TgPages::setPageSwitchType
 *
 * set page's switch style (type)
 *
 * \param type style of switching page
 */
void TgPages::setPageSwitchType(TgPagesPageSwitchType type)
{
    TG_FUNCTION_BEGIN();
    m_private->setPageSwitchType(type);
    TG_FUNCTION_END();
}

/*!
 * \brief TgPages::setPageSwitchTime
 *
 * sets "animation" time that takes when changing the page
 * in PageSwitchType_SlideFromTop, PageSwitchType_SlideFromBottom,
 * PageSwitchType_SlideFromLeft and PageSwitchType_SlideFromRight types
 *
 * type can be changed with function setPageSwitchType
 *
 * this page switch time is ignored if using PageSwitchType_Direct
 *
 * \param pageSwitchTime time that changing the page happens,
 * this value must be over 0
 */
void TgPages::setPageSwitchTime(double pageSwitchTime)
{
    TG_FUNCTION_BEGIN();
    if (pageSwitchTime <= 0) {
        TG_WARNING_LOG("setPageSwitchTime is ignoring pageSwitchTime, because it's under 0:", pageSwitchTime);
        TG_FUNCTION_END();
        return;
    }
    m_private->setPageSwitchTime(pageSwitchTime);
    TG_FUNCTION_END();
}

/*!
 * \brief TgPages::render
 *
 * Renders the pages
 * \param windowInfo
 */
bool TgPages::render(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgPages::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param windowInfo
 */
void TgPages::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    m_private->checkPositionValues();
    TG_FUNCTION_END();
}
