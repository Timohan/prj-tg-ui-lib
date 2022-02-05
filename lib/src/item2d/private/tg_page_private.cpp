/*!
 * \file
 * \brief file tg_page_private.cpp
 *
 * it holds general TgPagePrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_page_private.h"
#include "../../global/tg_global_log.h"
#include "../tg_page.h"

TgPagePrivate::TgPagePrivate(TgPage *currentPage) :
    m_currentPage(currentPage),
    m_pageSliding(false)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgPagePrivate::~TgPagePrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgPagesPrivate::setPageOutsideParent
 *
 * set page outside of parent
 */
void TgPagePrivate::setPageOutsideParent()
{
    TG_FUNCTION_BEGIN();
    m_currentPage->setAnchorState(TgItem2dAnchor::AnchorRelativeToParent);
    m_currentPage->setX(-m_currentPage->getWidth() - 10);
    m_currentPage->setY(0);
    m_currentPage->setWidth(m_currentPage->getWidth());
    m_currentPage->setHeight(m_currentPage->getHeight());
    m_currentPage->setVisible(false);
    m_pageSliding = false;
    TG_FUNCTION_END();
}

/*!
 * \brief TgPagesPrivate::setPageOnTopOfParent
 *
 * set page on top of pages
 */
void TgPagePrivate::setPageOnTopOfParent()
{
    TG_FUNCTION_BEGIN();
    m_currentPage->setAnchorState(TgItem2dAnchor::AnchorRelativeToParent);
    m_currentPage->setX(0);
    m_currentPage->setY(0);
    m_currentPage->setWidth(m_currentPage->getWidth());
    m_currentPage->setHeight(m_currentPage->getHeight());
    m_currentPage->setVisible(true);
    m_pageSliding = false;
    TG_FUNCTION_END();
}

void TgPagePrivate::setPageToMoving(TgPagesPageSwitchType type)
{
    TG_FUNCTION_BEGIN();
    m_currentPage->setAnchorState(TgItem2dAnchor::AnchorRelativeToParent);
    switch (type) {
        case TgPagesPageSwitchType::PageSwitchType_Direct:
        default:
            setPageOnTopOfParent();
            return;
        case TgPagesPageSwitchType::PageSwitchType_SlideFromTop:
            m_slidingStartX = 0;
            m_slidingStartY = -m_currentPage->getHeight();
            break;
        case TgPagesPageSwitchType::PageSwitchType_SlideFromBottom:
            m_slidingStartX = 0;
            m_slidingStartY = m_currentPage->getHeight();
            break;
        case TgPagesPageSwitchType::PageSwitchType_SlideFromLeft:
            m_slidingStartX = -m_currentPage->getWidth();
            m_slidingStartY = 0;
            break;
        case TgPagesPageSwitchType::PageSwitchType_SlideFromRight:
            m_slidingStartX = m_currentPage->getWidth();
            m_slidingStartY = 0;
            break;
    }
    m_currentPage->setX(m_slidingStartX);
    m_currentPage->setY(m_slidingStartY);
    m_currentPage->setWidth(m_currentPage->getWidth());
    m_currentPage->setHeight(m_currentPage->getHeight());
    m_currentPage->setVisible(true);
    m_pageSliding = true;
    TG_FUNCTION_END();
}

TgPageMovingState TgPagePrivate::pageMoving(double elapsedTime, double maxTime)
{
    TG_FUNCTION_BEGIN();
    if (!m_pageSliding) {
        TG_FUNCTION_END();
        return TgPageMovingState::TgPageMovinState_NotMoving;
    }
    if (elapsedTime >= maxTime) {
        m_currentPage->setX(0);
        m_currentPage->setY(0);
        m_pageSliding = false;
        TG_FUNCTION_END();
        return TgPageMovingState::TgPageMovinState_StopMoving;
    }

    m_currentPage->setX(static_cast<float>(maxTime-elapsedTime)*m_slidingStartX/static_cast<float>(maxTime));
    m_currentPage->setY(static_cast<float>(maxTime-elapsedTime)*m_slidingStartY/static_cast<float>(maxTime));
    TG_FUNCTION_END();
    return TgPageMovingState::TgPageMovinState_Moving;
}

