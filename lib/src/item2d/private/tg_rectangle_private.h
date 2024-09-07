/*!
 * \file
 * \brief file tg_rectangle_private.h
 *
 * it holds general TgRectanglePrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_RECTANGLE_PRIVATE_H
#define TG_RECTANGLE_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../math/tg_matrix4x4.h"
#include "../../render/tg_render.h"

class TgItem2d;
struct TgWindowInfo;
class TgItem2dPosition;

class TgRectanglePrivate : protected TgRender
{
public:
    explicit TgRectanglePrivate(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    ~TgRectanglePrivate();
    bool render(const TgWindowInfo *windowInfo, TgItem2d *currentItem, TgItem2dPosition *itemPosition, float opacity);
    void setColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void checkPositionValues(TgItem2d *currentItem);

private:
    TgImageAsset m_imageAsset;

    bool m_initVerticesDone;
    bool m_initImageAssetDone;

    TgMatrix4x4 m_transform;

    float m_r;
    float m_g;
    float m_b;
    float m_a;

    bool init();
    void setTranform(TgItem2d *currentItem);
    GLuint getTextureIndex();
};

#endif // TG_RECTANGLE_PRIVATE_H
