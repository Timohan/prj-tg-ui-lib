/*!
 * \file
 * \brief file tg_page.cpp
 *
 * Handles individual page and it can be used with TgPages
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_page.h"
#include "tg_pages.h"
#include "../global/tg_global_log.h"
#include "private/tg_page_private.h"

/*!
 * \brief TgPage::TgPage
 *
 * page, background is image background
 *
 * \param parent pointer to TgPages class
 * \param imageFilename with full file name of image (background)
 */
TgPage::TgPage(TgPages *parent, const char *imageFilename) :
    TgBackground(parent, imageFilename),
    m_private(new TgPagePrivate(this))
{
    TG_FUNCTION_BEGIN();
    parent->addPage(this);
    setAnchorState(TgItem2dAnchor::AnchorRelativeToParentXyFollowParentSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgPage::TgPage
 *
 * page, background is single color background (rgb)
 *
 * \param parent pointer to TgPages class
 * \param r red color of background
 * \param g green color of background
 * \param b blue color of background
 * \param a alpha color of background
 */
TgPage::TgPage(TgPages *parent, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgBackground(parent, r, g, b, a),
    m_private(new TgPagePrivate(this))
{
    TG_FUNCTION_BEGIN();
    parent->addPage(this);
    setAnchorState(TgItem2dAnchor::AnchorRelativeToParentXyFollowParentSize);
    TG_FUNCTION_END();
}


TgPage::~TgPage()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgPage::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgPage::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}
