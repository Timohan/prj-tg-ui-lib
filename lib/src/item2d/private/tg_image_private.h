/*!
 * \file
 * \brief file tg_image_private.h
 *
 * it holds general TgImagePrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_IMAGE_PRIVATE_H
#define TG_IMAGE_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../math/tg_matrix4x4.h"
#include "../../render/tg_render.h"
#include "../../global/private/tg_global_defines.h"
#include <string>

class TgItem2d;
struct TgWindowInfo;
class TgItem2dPosition;

class TgImagePrivate : protected TgRender
{
public:
    explicit TgImagePrivate(const char *filename);
    ~TgImagePrivate();
    bool render(const TgWindowInfo *windowInfo, TgItem2d *currentItem, TgItem2dPosition *itemPosition);
    void checkPositionValues(TgItem2d *currentItem);
    void setTextureST(float topLeftS, float topLeftT,
                      float topRightS, float topRightT,
                      float bottomRightS, float bottomRightT,
                      float bottomLeftS, float bottomLeftT);
    void setImage(const char *filename);

private:
    float m_topLeftS, m_topLeftT;
    float m_topRightS, m_topRightT;
    float m_bottomRightS, m_bottomRightT;
    float m_bottomLeftS, m_bottomLeftT;

    TgImageAsset m_imageAsset;

    bool m_initVerticesDone;
    bool m_initImageAssetDone;

    TgMatrix4x4 m_transform;

    bool init();
    void setTranform(TgItem2d *currentItem);
    GLuint getTextureIndex();
    void generateVertices(Vertice vertices[4]);
};

#endif // TG_IMAGE_PRIVATE_H
