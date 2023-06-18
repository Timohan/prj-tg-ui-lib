/*!
 * \file
 * \brief file tg_pages_private.cpp
 *
 * it holds general TgPagesPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_pages_private.h"
#include "../../global/tg_global_log.h"
#include "../tg_page.h"
#include "tg_page_private.h"
#include "../tg_item2d.h"
#include "item2d/tg_item2d_private.h"

TgPagesPrivate::TgPagesPrivate(TgItem2d *currentItem) :
    m_currentPageIndex(0),
    m_pageSwitchMaxTime(0.25),
    m_pageSwitchType(TgPagesPageSwitchType::PageSwitchType_Direct)
{
    TG_FUNCTION_BEGIN();
    currentItem->m_private->setInternalResize(this);
    TG_FUNCTION_END();
}

TgPagesPrivate::~TgPagesPrivate()
{
    TG_FUNCTION_BEGIN();
    m_listPage.clear();
    TG_FUNCTION_END();
}

/*!
 * \brief TgPagesPrivate::addPage
 *
 * add page to list of pages
 *
 * \param page add page
 */
void TgPagesPrivate::addPage(TgPage *page)
{
    TG_FUNCTION_BEGIN();
    if (m_listPage.size()) {
        page->m_private->setPageOutsideParent();
    } else {
        page->m_private->setPageOnTopOfParent();
    }
    m_listPage.push_back(page);
    TG_FUNCTION_END();
}

/*!
 * \brief TgPagesPrivate::getCurrentPageIndex
 *
 * get current page index
 *
 * \return current page index
 */
size_t TgPagesPrivate::getCurrentPageIndex() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_currentPageIndex;
}

/*!
 * \brief TgPagesPrivate::getPageCount
 *
 * get page count
 *
 * \return page count
 */
size_t TgPagesPrivate::getPageCount() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_listPage.size();
}

/*!
 * \brief TgPagesPrivate::setPage
 *
 * change page index
 *
 * \param pageIndex page index
 */
void TgPagesPrivate::setPage(size_t pageIndex)
{
    TG_FUNCTION_BEGIN();
    if (pageIndex >= m_listPage.size()) {
        TG_WARNING_LOG(std::string("Page index ("), pageIndex, std::string(") is larger than list of pages:"), m_listPage.size());
        TG_FUNCTION_END();
        return;
    }
    if (pageIndex == m_currentPageIndex) {
        TG_FUNCTION_END();
        return;
    }

    for (size_t i=0;i<m_listPage.size();i++) {
        if (m_pageSwitchType == TgPagesPageSwitchType::PageSwitchType_Direct) {
            if (i == pageIndex) {
                m_listPage[i]->m_private->setPageOnTopOfParent();
            } else {
                m_listPage[i]->m_private->setPageOutsideParent();
            }
        } else {
            if (i == pageIndex) {
                m_listPage[i]->m_private->setPageToMoving(m_pageSwitchType);
                m_listPage[i]->setToTop();
            } else if (i == m_currentPageIndex) {
                m_listPage[i]->m_private->setPageOnTopOfParent();
            } else {
                m_listPage[i]->m_private->setPageOutsideParent();
            }
        }
    }
    m_currentPageIndex = pageIndex;
    m_pageSwitchStartTime.resetTimer();
    TG_FUNCTION_END();
}

/*!
 * \brief TgPagesPrivate::setPageSwitchType
 *
 * set page's switch style (type)
 *
 * \param type style of switching page
 */
void TgPagesPrivate::setPageSwitchType(TgPagesPageSwitchType type)
{
    m_pageSwitchType = type;
}

/*!
 * \brief TgPagesPrivate::setPageSwitchTime
 *
 * sets "animation" time that takes when changing the page
 * in PageSwitchType_SlideFromTop, PageSwitchType_SlideFromBottom,
 * PageSwitchType_SlideFromLeft and PageSwitchType_SlideFromRight types
 *
 * type can be changed with function setPageSwitchType
 *
 * this page switch time is ignored if using PageSwitchType_Direct
 *
 * \param pageSwitchTime time that changing the page happens
 * this value must be over 0
 */
void TgPagesPrivate::setPageSwitchTime(double pageSwitchTime)
{
    TG_FUNCTION_BEGIN();
    m_pageSwitchMaxTime = pageSwitchTime;
    TG_FUNCTION_END();
}

void TgPagesPrivate::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    bool setVisibleValues = false;
    size_t i;
    TgPageMovingState state;
    double elapsedTime = m_pageSwitchStartTime.elapsedTimeFromBegin();
    for (i=0;i<m_listPage.size();i++) {
        state = m_listPage[i]->m_private->pageMoving(elapsedTime, m_pageSwitchMaxTime);
        if (state == TgPageMovingState::TgPageMovinState_StopMoving
            && m_currentPageIndex == i) {
            setVisibleValues = true;
        }
    }
    if (setVisibleValues) {
        for (i=0;i<m_listPage.size();i++) {
            if (m_currentPageIndex != i) {
                m_listPage[i]->setVisible(false);
            }
        }
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgPagesPrivate::onInternalResize
 *
 * This is internal resize call to set that pages are correctly set
 * by x, y, width, heigth
 */
void TgPagesPrivate::onInternalResize(float, float, float, float)
{
    TG_FUNCTION_BEGIN();
    for (size_t i=0;i<m_listPage.size();i++) {
        if (i == m_currentPageIndex) {
            m_listPage[i]->m_private->setPageOnTopOfParent();
        } else {
            m_listPage[i]->m_private->setPageOutsideParent();
        }
    }
    TG_FUNCTION_END();
}
