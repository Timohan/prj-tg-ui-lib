/*!
 * \file
 * \brief file tg_background.h
 *
 * Inits and draws background that can be TgRectangle
 * or TgImage
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_BACKGROUND_H
#define TG_BACKGROUND_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

class TgBackgroundPrivate;
struct TgWindowInfo;

/*!
 * \brief TgBackground
 * Inits and draws background that can be TgRectangle
 * or TgImage
 */
class TG_MAINWINDOW_EXPORT TgBackground : public TgItem2d
{
public:
    explicit TgBackground(TgItem2d *parent, const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    explicit TgBackground(TgItem2d *parent, float x, float y, float width, float height,
                          const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    explicit TgBackground(TgItem2d *parent, const char *imageFilename);
    explicit TgBackground(TgItem2d *parent, float x, float y, float width, float height, const char *imageFilename);

    ~TgBackground();

     void setColor(const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
     void setImage(const char *imageFilename);

protected:
    virtual void checkPositionValues() override;

private:
    TgBackgroundPrivate *m_private;
};

#endif // TG_BACKGROUND_H
