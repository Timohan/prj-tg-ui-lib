/*!
 * \file
 * \brief file tg_application.cpp
 *
 * application's private functionality
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#include "tg_application.h"
#include "tg_application_private.h"
#include "../global/tg_global_log.h"

TgApplication::TgApplication() :
    m_private(new TgApplicationPrivate())
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgApplication::~TgApplication()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgApplicationPrivate::exec
 *
 * starts this whole application running, this can be called only once
 *
 * \return 0 on success after all rendering is ended
 */
int TgApplication::exec()
{
    return m_private->exec();
}

/*!
 * \brief TgApplication::getTextureIndex
 *
 * get texture index for filename
 * if texture doesn't already exists, then this
 * generates texture index for filename (texture)
 *
 * \return texture index for filename (texture)
 * and if return 0, then generating texture is failed
 */
GLuint TgApplication::getTextureIndex(const char *filename)
{
    return m_private->getTextureIndex(filename);
}

/*!
 * \brief TgApplication::getDefaultFont
 *
 * get font default (full filepath name)
 *
 * \return font default (full filepath)
 */
const char *TgApplication::getDefaultFont()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getDefaultFont();
}

/*!
 * \brief TgApplication::setDefaultFont
 *
 * set font default (full filepath name)
 *
 * \param fullFilePathFont font default (full filepath)
 */
void TgApplication::setDefaultFont(const char *fullFilePathFont)
{
    TG_FUNCTION_BEGIN();
    m_private->setDefaultFont(fullFilePathFont);
    TG_FUNCTION_END();
}

/*!
 * \brief TgApplication::getFontCount
 *
 * get font count
 *
 * \return font count
 */
size_t TgApplication::getFontCount()
{
    return m_private->getFontCount();
}

/*!
 * \brief TgApplication::setFont
 *
 * set/add font (full filepath) to font list into position
 *
 * \param fullFilePathFont add/set this font name
 * \param position font to this position, this position is larger
 * than current index (for example UINT64_MAX), then it adds/set this
 * font to last of the list
 * \return index position that font will be set/added
 */
size_t TgApplication::setFont(const std::string &fullFilePathFont, size_t position)
{
    return m_private->setFont(fullFilePathFont, position);
}

/*!
 * \brief TgApplication::getFont
 *
 * get font by index
 *
 * \param i index of the font
 * \return font (file path) by the index
 * if index is larger than font list size, then it returns ""
 */
std::string TgApplication::getFont(size_t i)
{
    return m_private->getFont(i);
}