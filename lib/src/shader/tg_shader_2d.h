/*!
 * \file
 * \brief file tg_shader_2d.h
 *
 * Compile shader
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_SHADER_2D_H
#define TG_SHADER_2D_H

#include <GL/glew.h>
#include <GL/gl.h>

enum ShaderAttributes2d
{
    AttribPosition = 0,
    AttribTextCoord,
};

class TgShader2d
{
public:
    TgShader2d();
    ~TgShader2d();

    int init();

    GLuint generalShader();

    static GLuint m_shaderAttributeIndex[2];
private:
    static const char *m_vertShader;
    static const char *m_fragShader;
    GLuint m_generalShader;

    static int compileShader(const char *source, const GLenum type, GLuint *shader);
};

#endif // TG_SHADER_2D_H
