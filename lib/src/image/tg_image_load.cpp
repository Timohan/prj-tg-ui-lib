/*!
 * \file
 * \brief file tg_image_load.cpp
 *
 * it loads image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_image_load.h"
#include <png.h>
#include <cstring>
#include "../global/tg_global_log.h"

/*!
 * \brief TgImageLoad::loadPng
 *
 * creates image data from rowPointers
 *
 * \param filename png filename
 * \param width [out} width of image
 * \param height [out} height of image
 * \return pointer of image data that is ready to go into glTexImage2D
 * if fails, return nullptr
 */
unsigned char *TgImageLoad::loadPng(const char *filename, int &width, int &height)
{
    png_structp png;
    png_infop info;
    png_bytep *rowPointers;
    unsigned char header[8];    // 8 is the maximum size that can be checked
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        TG_ERROR_LOG("File could not open: ", filename);
        return nullptr;
    }


    if (fread(header, 1, 8, fp) != 8 ||
        png_sig_cmp(header, 0, 8)) {
        TG_ERROR_LOG("File is not png image: ", filename);
        fclose(fp);
        return nullptr;
    }

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png) {
        TG_ERROR_LOG("png_create_read_struct failed");
        fclose(fp);
        return nullptr;
    }

    info = png_create_info_struct(png);
    if (!info) {
        TG_ERROR_LOG("png_create_info_struct failed");
        fclose(fp);
        png_destroy_read_struct(&png, nullptr, nullptr);
        return nullptr;
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);
    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    int colorType = png_get_color_type(png, info);
    png_read_update_info(png, info);


    if (setjmp(png_jmpbuf(png))) {
        TG_ERROR_LOG("setjmp failed");
        fclose(fp);
        png_destroy_read_struct(&png, &info, nullptr);
        return nullptr;
    }

    rowPointers = new png_bytep[height]; //reinterpret_cast<png_bytep *>(malloc(sizeof(png_bytep) * height);
    for (int y=0;y<height;y++) {
        rowPointers[y] = new png_byte[png_get_rowbytes(png, info)]; // (png_byte*) malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, rowPointers);
    unsigned char *imageData = generateImageData(rowPointers, colorType, width, height);
    png_destroy_read_struct(&png, &info, nullptr);
    for (int y=0;y<height;y++) {
        delete[] rowPointers[y];
    }
    delete[] rowPointers;
    fclose(fp);
    return imageData;
}

/*!
 * \brief TgImageLoad::generateImageData
 *
 * creates image data from rowPointers
 *
 * \param rowPointers from png lib
 * \param colorType type of color
 * \param width width of image
 * \param height height of image
 * \return pointer of image data that is ready to go into glTexImage2D
 */
unsigned char *TgImageLoad::generateImageData(const png_bytep *rowPointers, int colorType, int width, int height)
{
    if (colorType != PNG_COLOR_TYPE_RGBA
        && colorType != PNG_COLOR_TYPE_RGB) {
        TG_ERROR_LOG("Png color type is not PNG_COLOR_TYPE_RGBA or PNG_COLOR_TYPE_RGB");
        return nullptr;
    }
    int x, y;
    png_byte *row;
    png_byte *ptr;
    unsigned char *ret = new unsigned char[width*height*4];
    if (colorType == PNG_COLOR_TYPE_RGBA) {
        for (y=0;y<height;y++) {
            row = rowPointers[y];
            for (x=0;x<width; x++) {
                ptr = &(row[x*4]);
                ret[y*width*4+x*4+0] = ptr[0];
                ret[y*width*4+x*4+1] = ptr[1];
                ret[y*width*4+x*4+2] = ptr[2];
                ret[y*width*4+x*4+3] = ptr[3];
            }
        }
        return ret;
    }
    for (y=0;y<height;y++) {
        row = rowPointers[y];
        for (x=0;x<width; x++) {
            ptr = &(row[x*3]);
            ret[y*width*4+x*4+0] = ptr[0];
            ret[y*width*4+x*4+1] = ptr[1];
            ret[y*width*4+x*4+2] = ptr[2];
            ret[y*width*4+x*4+3] = 255;
        }
    }
    return ret;
}
