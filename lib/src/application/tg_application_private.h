/*!
 * \file
 * \brief file tg_application_private.h
 *
 * application's private functionality
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_APPLICATION_PRIVATE_H
#define TG_APPLICATION_PRIVATE_H

#include <GL/glew.h>
#include <string>
#include <vector>

class TgApplicationPrivate
{
public:
    TgApplicationPrivate();
    ~TgApplicationPrivate();

    int exec();
    GLuint getTextureIndex(const char *filename);

    const char *getDefaultFont();
    void setDefaultFont(const char *fullFilePathFont);
    size_t getFontCount();
    size_t setFont(const std::string &fullFilePathFont, size_t position = UINT64_MAX);
    std::string getFont(size_t i);
    bool loadCharactersToCache(const char *filename, const std::vector<uint32_t> &listCharacters, const float &fontSize);
private:
};

#endif // TG_APPLICATION_PRIVATE_H
