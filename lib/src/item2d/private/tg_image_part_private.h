/*!
 * \file
 * \brief file tg_image_part_private.h
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_IMAGE_PART_PRIVATE_H
#define TG_IMAGE_PART_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../global/private/tg_global_defines.h"
#include "../../math/tg_matrix4x4.h"
#include "../tg_image_part.h"
#include "../../render/tg_render.h"

class TgItem2d;
struct TgWindowInfo;

#define IMAGE_PARTS_MAX_COUNT           9
#define IMAGE_PARTS_VERTICES_MAX_COUNT  36

class TgImagePartPrivate : protected TgRender
{
public:
    explicit TgImagePartPrivate(TgItem2d *currentItem, const char *imageFilename, TgImagePartType type);
    ~TgImagePartPrivate();

    void setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom);
    void setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize);

    void setImageCropPosition3LeftToRight(float imageCropLeft, float imageCropRight);
    void setImageAreaSize3LeftToRight(float leftAreaSize, float rightAreaSize);

    void setImageCropPosition3TopToBottom(float imageCropTop, float imageCropBottom);
    void setImageAreaSize3TopToBottom(float topAreaSize, float bottomAreaSize);

    TgImagePartType getType() const;
    void setType(TgImagePartType type);

    void render(const TgWindowInfo *windowInfo, TgItem2d *currentItem);
    void checkPositionValues(TgItem2d *currentItem);

    void setImage(const char *filename);
private:
    enum VerticeIndexPositionParts9
    {
        TopLeft = 0,
        TopMiddle = 4,
        TopRight = 8,
        MiddleLeft = 12,
        MiddleMiddle = 16,
        MiddleRight = 20,
        BottomLeft = 24,
        BottomMiddle = 28,
        BottomRight = 32
    };

    TgImagePartType m_type;
    bool m_initImageAssetDone;
    bool m_initVerticesDone;
    TgItem2d *m_currentItem;
    TgImageAsset m_imageAsset;
    TgMatrix4x4 m_transform;

    float m_imageCropLeft, m_imageCropTop, m_imageCropRight, m_imageCropBottom;
    float m_leftAreaSize, m_topAreaSize, m_rightAreaSize, m_bottomAreaSize;

    float m_imageCrop3Left, m_imageCrop3Right;
    float m_leftArea3Size, m_rightArea3Size;

    float m_imageCrop3Top, m_imageCrop3Bottom;
    float m_topArea3Size, m_bottomArea3Size;

    void generateVertices(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT]);
    void generateVertices3LeftToRight(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT]);
    void generateVertices3TopToBottom(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT]);
    GLuint getTextureIndex();
    bool init();
    void setTranform(TgItem2d *currentItem);
    static void setTexturePosition(Vertice vertices[IMAGE_PARTS_VERTICES_MAX_COUNT],
                                   VerticeIndexPositionParts9 part,
                                   float textureX,
                                   float textureY,
                                   float textureWidth,
                                   float textureHeight,
                                   float x, float y, float width, float height);
};

#endif // TG_IMAGE_PART_PRIVATE_H
