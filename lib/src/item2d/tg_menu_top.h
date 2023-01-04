/*!
 * \file
 * \brief file tg_menu_top.h
 *
 * Top menu
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MENU_TOP_H
#define TG_MENU_TOP_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"
#include "../event/tg_event_data.h"
#include "tg_menu_item.h"

class TgMenuTopPrivate;

/*!
 * \brief TgMenuTop
 * Top menu for TgItem2d, normally contains File, Edit and such
 */
class TG_MAINWINDOW_EXPORT TgMenuTop : public TgItem2d
{
public:
    explicit TgMenuTop(TgItem2d *parent);
    ~TgMenuTop();

    TgMenuItem *addMenu(const char *text);
    TgMenuItem *addMenu(const char *text, const TgShortCut *shortCut);

    virtual size_t getMenuCount() override;
    virtual bool removeMenu(size_t i) override;

protected:
    virtual void checkOnResizeChanged();
    virtual void checkPositionValuesChildren(const TgWindowInfo *windowInfo) override;

private:
    TgMenuTopPrivate *m_private;
};

#endif // TG_MENU_TOP_H
