/*!
 * \file
 * \brief file tg_page.h
 *
 * Handles individual page and it can be used with TgPages
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_PAGE_H
#define TG_PAGE_H

#include "tg_background.h"
#include "../global/tg_global_macros.h"

struct TgWindowInfo;
class TgPagePrivate;
class TgPages;

/*!
 * \brief TgPage
 * Handles individual page and it can be used with TgPages
 */
class TG_MAINWINDOW_EXPORT TgPage : public TgBackground
{
public:
    explicit TgPage(TgPages *parent, const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    explicit TgPage(TgPages *parent, const char *imageFilename);
    virtual ~TgPage();

protected:
    virtual void render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;

private:
    TgPagePrivate *m_private;

    friend class TgPagesPrivate;
};

#endif // TG_PAGE_H
