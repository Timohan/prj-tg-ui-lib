/*!
 * \file
 * \brief file tg_image_assets.h
 *
 * it holds created textures
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_IMAGE_ASSETS_H
#define TG_IMAGE_ASSETS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>

enum TgImageType
{
    ImageTypeNA = 0,
    PlainImage,
    LoadedImage,
    GeneratedImage,
};

struct TgImageAsset
{
    GLuint m_textureIndex;
    TgImageType m_type;
    std::string m_filename;

    union {
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        } m_plainImage;
        struct {
            int m_width;
            int m_height;
        } m_loadedImage;
        struct {
            uint8_t *m_imageData;
            int m_width;
            int m_height;
        } m_generatedImage;
    } m_imageData;
};

class TgImageAssets
{
public:
    explicit TgImageAssets();
    ~TgImageAssets();

    GLuint generateImage(TgImageAsset &asset);
    bool deleteImage(TgImageAsset &asset);
    GLuint convertLoadedImageToGeneratedImage(TgImageAsset &asset);
    void modifyTexture(const unsigned char *imageData, int width, int height, GLuint textureIndex);

private:
    std::vector<TgImageAsset>m_listImages;

    GLuint generatePlainImage(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    GLuint loadImage(TgImageAsset &asset);
    GLuint setImageDataToTexture(const unsigned char *imageData, int width, int height);
    GLuint setImageGenerated(TgImageAsset &asset);

    static void clear(TgImageAsset *asset);
};

#endif // TG_IMAGE_ASSETS_H
