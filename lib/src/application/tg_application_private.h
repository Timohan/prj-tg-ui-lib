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

class TgApplicationPrivate
{
public:
    TgApplicationPrivate();
    ~TgApplicationPrivate();

    int exec();
    GLuint getTextureIndex(const char *filename);

    const char *getDefaultFont();
    void setDefaultFont(const char *fullFilePathFont);
private:
};

#endif // TG_APPLICATION_PRIVATE_H
