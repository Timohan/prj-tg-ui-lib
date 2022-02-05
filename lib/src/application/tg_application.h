/*!
 * \file
 * \brief file tg_application.h
 *
 * application's private functionality
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_APPLICATION_H
#define TG_APPLICATION_H

#include <GL/glew.h>
#include "../global/tg_global_macros.h"

struct TgApplicationPrivate;

/*!
 * \brief TgApplication
 * This is application functionalities
 * You should only init one (1) TgApplication
 */
class TG_MAINWINDOW_EXPORT TgApplication
{
public:
    explicit TgApplication();
    ~TgApplication();
    int exec();
    GLuint getTextureIndex(const char *filename);

    const char *getDefaultFont();
    void setDefaultFont(const char *fullFilePathFont);
private:
    TgApplicationPrivate *m_private;
};

#endif // TG_APPLICATION_H
