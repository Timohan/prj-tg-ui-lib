/*!
 * \file
 * \brief file tg_application_private.cpp
 *
 * application's private functionality
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#include "tg_application_private.h"
#include "../global/tg_global_log.h"
#include "../global/tg_global_application.h"
#ifdef USE_GLFW
#include "../window/glfw/tg_glfw_input.h"
#else

#endif
#include "../image/tg_image_assets.h"
#include <string>

TgApplicationPrivate::TgApplicationPrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgApplicationPrivate::~TgApplicationPrivate()
{
    TG_FUNCTION_BEGIN();
    TgGlobalApplication::getInstance()->clear();
    TG_FUNCTION_END();
}

/*!
 * \brief TgApplicationPrivate::exec
 *
 * starts this whole application running, this can be called only once
 *
 * \return 0 on success after all rendering is ended
 */
int TgApplicationPrivate::exec()
{
    TG_FUNCTION_BEGIN();
    int ret = TgGlobalApplication::getInstance()->init();
    if (ret != EXIT_SUCCESS) {
        TG_FUNCTION_END();
        return ret;
    }
    while (!TgGlobalApplication::getInstance()->getExit()) {
        TgGlobalApplication::getInstance()->render();
    }
#ifdef USE_GLFW
    TgGlfwInput::clear();
#else

#endif
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgApplicationPrivate::getTextureIndex
 *
 * get texture index for filename
 * if texture doesn't already exists, then this
 * generates texture index for filename (texture)
 *
 * \return texture index for filename (texture)
 * and if return 0, then generating texture is failed
 */
GLuint TgApplicationPrivate::getTextureIndex(const char *filename)
{
    TG_FUNCTION_BEGIN();
    TgImageAsset imageAsset;
    imageAsset.m_textureIndex = 0;
    imageAsset.m_type = TgImageType::LoadedImage;
    imageAsset.m_filename = filename;
    TG_FUNCTION_END();
    return TgGlobalApplication::getInstance()->getImageAssets()->generateImage(imageAsset);
}

/*!
 * \brief TgApplicationPrivate::getDefaultFont
 *
 * get font default (full filepath name)
 *
 * \return font default (full filepath)
 */
const char *TgApplicationPrivate::getDefaultFont()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return TgGlobalApplication::getInstance()->getFontDefault()->getDefaultFont().c_str();
}

/*!
 * \brief TgApplicationPrivate::setDefaultFont
 *
 * set font default (full filepath name)
 *
 * \param fullFilePathFont font default (full filepath)
 */
void TgApplicationPrivate::setDefaultFont(const char *fullFilePathFont)
{
    TG_FUNCTION_BEGIN();
    TgGlobalApplication::getInstance()->getFontDefault()->setDefaultFont(fullFilePathFont);
    TG_FUNCTION_END();
}

/*!
 * \brief TgApplicationPrivate::getFontCount
 *
 * get font count
 *
 * \return font count
 */
size_t TgApplicationPrivate::getFontCount()
{
    return TgGlobalApplication::getInstance()->getFontDefault()->getFontCount();
}

/*!
 * \brief TgApplicationPrivate::setFont
 *
 * set/add font (full filepath) to font list into position
 *
 * \param fullFilePathFont add/set this font name
 * \param position font to this position, this position is larger
 * than current index (for example UINT64_MAX), then it adds/set this
 * font to last of the list
 * \return index position that font will be set/added
 */
size_t TgApplicationPrivate::setFont(const std::string &fullFilePathFont, size_t position)
{
    return TgGlobalApplication::getInstance()->getFontDefault()->setFont(fullFilePathFont, position);
}

/*!
 * \brief TgApplicationPrivate::getFont
 *
 * get font by index
 *
 * \param i index of the font
 * \return font (file path) by the index
 * if index is larger than font list size, then it returns ""
 */
std::string TgApplicationPrivate::getFont(size_t i)
{
    return TgGlobalApplication::getInstance()->getFontDefault()->getFont(i);
}