/*!
 * \file
 * \brief file tg_rectangle.h
 *
 * Inits and draws rectangle
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_RECTANGLE_H
#define TG_RECTANGLE_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

class TgRectanglePrivate;
struct TgWindowInfo;

/*!
 * \brief TgRectangle
 * draws rectangle
 */
class TG_MAINWINDOW_EXPORT TgRectangle : public TgItem2d
{
public:
    explicit TgRectangle(TgItem2d *parent, const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    explicit TgRectangle(TgItem2d *parent, float x, float y, float width, float height,
                         const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    ~TgRectangle();
    void setColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);

protected:
    virtual void render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;

private:
    TgRectanglePrivate *m_private;

    friend class TgMainWindowMenu;
};

#endif // TG_RECTANGLE_H
