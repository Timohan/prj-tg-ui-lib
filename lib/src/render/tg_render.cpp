/*!
 * \file
 * \brief file tg_render.cpp
 *
 * Handles the rendering - common class for items
 * to simplify rendering
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_render.h"
#include "../global/tg_global_log.h"
#include "../shader/tg_shader_2d.h"

TgRender::TgRender() :
    m_vertexArrayObject(0),
    m_vertexBufferObject(0),
    m_verticesCount(0)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgRender::~TgRender()
{
    TG_FUNCTION_BEGIN();
    if (m_vertexArrayObject) {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }
    if (m_vertexBufferObject) {
        glDeleteBuffers(1, &m_vertexBufferObject);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgRender::init
 *
 * inits the vertices
 *
 * \param vertices pointer to vertices[]
 * \param verticesCount vertices count
 * \return true on success
 */
bool TgRender::init(Vertice *vertices, int verticesCount, bool staticDraw)
{
    TG_FUNCTION_BEGIN();
    if (m_vertexBufferObject && staticDraw == false) {
        TG_FUNCTION_END();
        return TgRender::reInit(vertices, verticesCount);
    }
    glGenVertexArrays(1, &m_vertexArrayObject);
    glGenBuffers(1, &m_vertexBufferObject);
    glBindVertexArray(m_vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    if (staticDraw) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice)*verticesCount, vertices, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice)*verticesCount, vertices, GL_DYNAMIC_DRAW);
    }

    glVertexAttribPointer(TgShader2d::m_shaderAttributeIndex[ShaderAttributes2d::AttribPosition], 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(TgShader2d::m_shaderAttributeIndex[ShaderAttributes2d::AttribPosition]);
    glVertexAttribPointer(TgShader2d::m_shaderAttributeIndex[ShaderAttributes2d::AttribTextCoord], 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(TgShader2d::m_shaderAttributeIndex[ShaderAttributes2d::AttribTextCoord]);
    glBindVertexArray(0);
    m_verticesCount = verticesCount;
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgRender::reInit
 *
 * re-inits the vertices
 *
 * \param vertices pointer to vertices[]
 * \param verticesCount vertices count
 * \return true on success
 */
bool TgRender::reInit(Vertice *vertices, int verticesCount)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertice)*verticesCount, vertices);
    m_verticesCount = verticesCount;
    return true;
}

/*!
 * \brief TgRender::render
 *
 * Renders the image
 * \param textureIndex
 */
void TgRender::render(int textureIndex)
{
    TG_FUNCTION_BEGIN();
    if (!m_vertexArrayObject) {
        TG_FUNCTION_END();
        return;
    }
    glBindVertexArray(m_vertexArrayObject);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_verticesCount);
    glBindVertexArray(0);
    TG_FUNCTION_END();
}

void TgRender::render(int textureIndex, int startIndex, int verticesCount)
{
    TG_FUNCTION_BEGIN();
    if (!m_vertexArrayObject) {
        TG_FUNCTION_END();
        return;
    }
    glBindVertexArray(m_vertexArrayObject);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glDrawArrays(GL_TRIANGLE_STRIP, startIndex, verticesCount);
    glBindVertexArray(0);
    TG_FUNCTION_END();
}

void TgRender::render(int textureIndex, int startIndex, int verticesCount, int blendSfactor, int blendDfactor) const
{
    TG_FUNCTION_BEGIN();
    if (!m_vertexArrayObject) {
        TG_FUNCTION_END();
        return;
    }
    glBindVertexArray(m_vertexArrayObject);
    glEnable(GL_BLEND);
    glBlendFunc(blendSfactor, blendDfactor);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glDrawArrays(GL_TRIANGLE_STRIP, startIndex, verticesCount);
    glBindVertexArray(0);
    TG_FUNCTION_END();
}
