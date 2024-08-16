/*!
 * \file
 * \brief file tg_image_assets.cpp
 *
 * it holds created textures
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_image_assets.h"
#include "../global/tg_global_log.h"
#include "tg_image_load.h"

TgImageAssets::TgImageAssets()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgImageAssets::~TgImageAssets()
{
    TG_FUNCTION_BEGIN();
    std::vector<TgImageAsset>::iterator it;
    for (it=m_listImages.begin();it!=m_listImages.end();it++) {
        if (it->m_textureIndex) {
            glDeleteTextures(1, &it->m_textureIndex);
        }
        clear(&(*it));
    }
    m_listImages.clear();
    TG_FUNCTION_END();
}

bool TgImageAssets::deleteImage(TgImageAsset &asset)
{
    std::vector<TgImageAsset>::iterator it;
    for (it=m_listImages.begin();it!=m_listImages.end();it++) {
        if (it->m_textureIndex == asset.m_textureIndex) {
            glDeleteTextures(1, &it->m_textureIndex);
            clear(&(*it));
            m_listImages.erase(it);
        }
        return true;
    }
    return false;
}

/**
 * @brief TgImageAssets::clear
 *
 * deletes (clears) all required pointers from asset
 *
 * @param asset
 */
void TgImageAssets::clear(TgImageAsset *asset)
{
    switch (asset->m_type) {
        case TgImageType::GeneratedImage:
            if (asset->m_imageData.m_generatedImage.m_imageData) {
                delete[] asset->m_imageData.m_generatedImage.m_imageData;
                asset->m_imageData.m_generatedImage.m_imageData = nullptr;
            }
            break;
        case TgImageType::LoadedImage:
        case TgImageType::PlainImage:
        case TgImageType::ImageTypeNA:
        default:
            break;
    }
}

/*!
 * \brief TgImageAssets::generatePlainImage
 *
 * generates image by image asset
 *
 * \param asset
 * \return generated image texture id
 */
GLuint TgImageAssets::generateImage(TgImageAsset &asset)
{
    switch (asset.m_type) {
        case TgImageType::PlainImage:
            return generatePlainImage(asset.m_imageData.m_plainImage.r,
                                      asset.m_imageData.m_plainImage.g,
                                      asset.m_imageData.m_plainImage.b,
                                      asset.m_imageData.m_plainImage.a);
        case TgImageType::LoadedImage:
            return loadImage(asset);
        case TgImageType::GeneratedImage:
            return TgImageAssets::setImageGenerated(asset);
        case TgImageType::ImageTypeNA:
        default:
            return 0;
    }
}

/*!
 * \brief TgImageAssets::generatePlainImage
 *
 * generates plain image
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 * \return texture index
 */
GLuint TgImageAssets::generatePlainImage(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgImageAsset>::const_iterator it;
    for (it=m_listImages.begin();it!=m_listImages.end();it++) {
        if (it->m_type == TgImageType::PlainImage
            && it->m_imageData.m_plainImage.r == r
            && it->m_imageData.m_plainImage.g == g
            && it->m_imageData.m_plainImage.b == b
            && it->m_imageData.m_plainImage.a == a) {
            return it->m_textureIndex;
        }
    }
    int x, y, i = 0;
    const int width = 2;
    const int height = 2;
    unsigned char* imageData = nullptr;
    TgImageAsset asset;
    asset.m_textureIndex = 0;
    asset.m_type = TgImageType::PlainImage;
    asset.m_imageData.m_plainImage.r = r;
    asset.m_imageData.m_plainImage.g = g;
    asset.m_imageData.m_plainImage.b = b;
    asset.m_imageData.m_plainImage.a = a;

    imageData = new unsigned char[width*height*4];
    for (x=0;x<width;x++) {
        for (y=0;y<height;y++) {
            imageData[i++] = r;
            imageData[i++] = g;
            imageData[i++] = b;
            imageData[i++] = a;
        }
    }

    asset.m_textureIndex = setImageDataToTexture(imageData, width, height);

    delete[]imageData;
    if (!asset.m_textureIndex) {
        return asset.m_textureIndex;
    }
    m_listImages.push_back(asset);
    TG_FUNCTION_END();
    return asset.m_textureIndex;
}

/*!
 * \brief TgImageAssets::loadImage
 *
 * loads image and sets it as texture
 *
 * \param asset image assets
 * \return texture index
 */
GLuint TgImageAssets::loadImage(TgImageAsset &asset)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgImageAsset>::const_iterator it;
    for (it=m_listImages.begin();it!=m_listImages.end();it++) {
        if (it->m_type == TgImageType::LoadedImage
            && it->m_textureIndex > 0
            && asset.m_filename.compare(it->m_filename) == 0) {
            asset.m_imageData.m_loadedImage.m_width = it->m_imageData.m_loadedImage.m_width;
            asset.m_imageData.m_loadedImage.m_height = it->m_imageData.m_loadedImage.m_height;
            return it->m_textureIndex;
        }
    }
    unsigned char *imageData = TgImageLoad::loadPng(asset.m_filename.c_str(),
                                                    asset.m_imageData.m_loadedImage.m_width,
                                                    asset.m_imageData.m_loadedImage.m_height);
    if (!imageData) {
        return 0;
    }

    TgImageAsset newAsset;
    newAsset.m_textureIndex = 0;
    newAsset.m_type = TgImageType::LoadedImage;
    newAsset.m_filename = asset.m_filename;
    newAsset.m_textureIndex = setImageDataToTexture(imageData, asset.m_imageData.m_loadedImage.m_width, asset.m_imageData.m_loadedImage.m_height);
    asset.m_textureIndex = newAsset.m_textureIndex;
    newAsset.m_imageData.m_loadedImage.m_width = asset.m_imageData.m_loadedImage.m_width;
    newAsset.m_imageData.m_loadedImage.m_height = asset.m_imageData.m_loadedImage.m_height;

    delete[] imageData;

    if (!newAsset.m_textureIndex) {
        return newAsset.m_textureIndex;
    }
    m_listImages.push_back(newAsset);

    TG_FUNCTION_END();
    return asset.m_textureIndex;
}

/*!
 * \brief TgImageAssets::setImageGenerated
 *
 * set image generated data and sets it as texture
 *
 * \param asset image assets
 * \return texture index
 */
GLuint TgImageAssets::setImageGenerated(TgImageAsset &asset)
{
    TgImageAsset newAsset;
    newAsset.m_textureIndex = 0;
    newAsset.m_type = TgImageType::GeneratedImage;
    newAsset.m_imageData.m_generatedImage.m_imageData = asset.m_imageData.m_generatedImage.m_imageData;
    newAsset.m_textureIndex = setImageDataToTexture(asset.m_imageData.m_generatedImage.m_imageData, asset.m_imageData.m_generatedImage.m_width, asset.m_imageData.m_generatedImage.m_height);
    asset.m_textureIndex = newAsset.m_textureIndex;
    newAsset.m_imageData.m_generatedImage.m_width = asset.m_imageData.m_generatedImage.m_width;
    newAsset.m_imageData.m_generatedImage.m_height = asset.m_imageData.m_generatedImage.m_height;

    if (!newAsset.m_textureIndex) {
        return newAsset.m_textureIndex;
    }
    m_listImages.push_back(newAsset);

    TG_FUNCTION_END();
    return asset.m_textureIndex;
}


/*!
 * \brief TgImageAssets::setImageDataToTexture
 *
 * set's image data to texture
 *
 * \param imageData image data (RGBA)
 * \param width width of image (imageData)
 * \param height height of image (imageData)
 */
GLuint TgImageAssets::setImageDataToTexture(const unsigned char *imageData, int width, int height)
{
    GLuint textureIndex;
    TG_FUNCTION_BEGIN();
    glGenTextures(1, &textureIndex);

    if (!textureIndex) {
        TG_ERROR_LOG("Failed to create texture");
        TG_FUNCTION_END();
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, textureIndex);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    TG_FUNCTION_END();
    return textureIndex;
}

/**
 * @brief TgImageAssets::modifyTexture
 *
 * modify texture data
 *
 * @param imageData new image data of texture
 * @param width new image width of texture
 * @param height new image height of texture
 * @param textureIndex texture index
 */
void TgImageAssets::modifyTexture(const unsigned char *imageData, int width, int height, GLuint textureIndex)
{
    TG_FUNCTION_BEGIN();
    glBindTexture(GL_TEXTURE_2D, textureIndex);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    TG_FUNCTION_END();
}

/**
 * @brief TgImageAssets::convertLoadedImageToGeneratedImage
 *
 * converts loaded image to generated image
 *
 * @param asset [in/out] TgImageAsset is modified from LoadedImage to GeneratedImage
 * and asset is also added to m_listImages
 * @return GLuint 0 if fails
 */
GLuint TgImageAssets::convertLoadedImageToGeneratedImage(TgImageAsset &asset)
{
    TG_FUNCTION_BEGIN();
    if (asset.m_type != TgImageType::LoadedImage) {
        TG_FUNCTION_END();
        return 0;
    }

    int width = asset.m_imageData.m_loadedImage.m_width;
    int height = asset.m_imageData.m_loadedImage.m_height;
    unsigned char *imageData = new unsigned char[width*height*4];

    glBindTexture(GL_TEXTURE_2D, asset.m_textureIndex);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    GLuint textureIndex = setImageDataToTexture(imageData, width, height);
    if (!textureIndex) {
        delete[]imageData;
        TG_FUNCTION_END();
        return 0;
    }
    m_listImages.push_back(TgImageAsset());
    m_listImages.back().m_textureIndex = textureIndex;
    m_listImages.back().m_type = TgImageType::GeneratedImage;
    m_listImages.back().m_imageData.m_generatedImage.m_imageData = imageData;
    m_listImages.back().m_imageData.m_generatedImage.m_width = width;
    m_listImages.back().m_imageData.m_generatedImage.m_height = height;

    asset.m_textureIndex = textureIndex;
    asset.m_type = TgImageType::GeneratedImage;
    asset.m_imageData.m_generatedImage.m_imageData = imageData;
    asset.m_imageData.m_generatedImage.m_width = width;
    asset.m_imageData.m_generatedImage.m_height = height;
    TG_FUNCTION_END();
    return asset.m_textureIndex;
}