/*!
 * \file
 * \brief file tg_textfield_private.h
 *
 * it holds general TgTextfieldPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_TEXTFIELD_PRIVATE_H
#define TG_TEXTFIELD_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../math/tg_matrix4x4.h"
#include "../../font/tg_character_positions.h"
#include <string>
#include <mutex>
#include "../tg_textfield.h"

struct TgFontInfo;
class TgItem2d;
struct TgWindowInfo;

class TgTextfieldPrivate
{
public:
    explicit TgTextfieldPrivate(const char *text, const char *fontFile, float fontSize, uint8_t r, uint8_t g, uint8_t b);
    ~TgTextfieldPrivate();
    void render(const TgWindowInfo *windowInfo, TgItem2d *current);
    void checkPositionValues(const TgWindowInfo *windowInfo, TgItem2d *currentItem);
    void setHorizontalAlign(TgItem2d *currentItem, TgTextfieldHorizontalAlign align);
    void setVerticalAlign(TgItem2d *currentItem, TgTextfieldVerticalAlign align);
    TgTextfieldHorizontalAlign getAlignHorizontal() const;
    TgTextfieldVerticalAlign getAlignVertical() const;
    void setText(const char *text, TgItem2d *currentItem);
    void setFontSize(float fontSize);

private:
    TgFontInfo *m_fontInfo;
    float m_r, m_g, m_b;
    std::string m_text;
    std::string m_fontFile;
    float m_fontSize;
    bool m_initDone;
    std::vector<TgMatrix4x4>m_listTransform;
    TgCharPosition m_characterPositions;
    TgTextfieldHorizontalAlign m_alignHorizontal;
    TgTextfieldVerticalAlign m_alignVertical;
    std::recursive_mutex m_mutex;

    void generateTransform(const TgWindowInfo *windowInfo, TgItem2d *currentItem);
};

#endif // TG_TEXTFIELD_PRIVATE_H
