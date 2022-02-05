/*!
 * \file
 * \brief file tg_background_private.h
 *
 * Inits and draws background that can be TgRectangle
 * or TgImage (private)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_BACKGROUND_PRIVATE_H
#define TG_BACKGROUND_PRIVATE_H

#include "../../math/tg_matrix4x4.h"
#include "../../global/private/tg_global_defines.h"
#include <string>

class TgItem2d;
class TgImage;
class TgRectangle;
struct TgWindowInfo;

class TgBackgroundPrivate
{
public:
    explicit TgBackgroundPrivate(TgItem2d *currentItem, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    explicit TgBackgroundPrivate(TgItem2d *currentItem, const char *imageFilename);
    ~TgBackgroundPrivate();

    void setColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void setImage(const char *imageFilename);
private:
    TgItem2d *m_currentItem;
    TgRectangle *m_rectangle;
    TgImage *m_image;

};

#endif // TG_BACKGROUND_PRIVATE_H
