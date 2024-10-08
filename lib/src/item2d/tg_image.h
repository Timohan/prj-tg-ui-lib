/*!
 * \file
 * \brief file tg_image.h
 *
 * Inits and draws image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_IMAGE_H
#define TG_IMAGE_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

class TgImagePrivate;
struct TgWindowInfo;

/*!
 * \brief TgImage
 * normal single image
 */
class TG_MAINWINDOW_EXPORT TgImage : public TgItem2d
{
public:
    explicit TgImage(TgItem2d *parent, const char *filename);
    explicit TgImage(TgItem2d *parent, float x, float y, float width, float height, const char *filename);
    ~TgImage();
    void setTextureST(float topLeftS, float topLeftT,
                      float topRightS, float topRightT,
                      float bottomRightS, float bottomRightT,
                      float bottomLeftS, float bottomLeftT);
    void setImage(const char *filename);
    bool setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    uint32_t getImageWidth();
    uint32_t getImageHeight();

protected:
    virtual bool render(const TgWindowInfo *windowInfo, float parentOpacity) override;
    virtual void checkPositionValues() override;

private:
    TgImagePrivate *m_private;
};

#endif // TG_IMAGE_H
