/*!
 * \file
 * \brief file tg_render.h
 *
 * Handles the rendering - common class for items
 * to simplify rendering
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_RENDER_H
#define TG_RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "../global/private/tg_global_defines.h"

class TgRender
{
public:
    explicit TgRender();
    ~TgRender();
    bool init(Vertice *vertices, int verticesCount, bool staticDraw = false);
    void render(int textureIndex);
    void render(int textureIndex, int startIndex, int verticesCount);
    void render(int textureIndex, int startIndex, int verticesCount, int blendSfactor, int blendDfactor) const;

private:
    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
    int m_verticesCount;

    bool reInit(Vertice *vertices, int verticesCount);
};

#endif // TG_RENDER_H
