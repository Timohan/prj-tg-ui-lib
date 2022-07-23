/*!
 * \file
 * \brief file tg_image_load.h
 *
 * it loads image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#ifndef TG_IMAGE_LOAD_H
#define TG_IMAGE_LOAD_H

#include <png.h>

class TgImageLoad
{
public:
    static unsigned char *loadPng(const char *filename, int &width, int &height);

private:
    static unsigned char *generateImageData(const png_bytep *rowPointers, int colorType, int width, int height);

};

#endif // TG_IMAGE_LOAD_H
