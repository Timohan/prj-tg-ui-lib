/*!
 * \file
 * \brief file tg_shader_2d.cpp
 *
 * Compile shader
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_shader_2d.h"
#include <stdio.h>
#include "../global/tg_global_log.h"

GLuint TgShader2d::m_shaderAttributeIndex[2] = {0,0};

/*!
 * \brief TgShader2d::m_vertShader
 *
 * simple 2D vertex shader
 */
const char *TgShader2d::m_vertShader = "attribute vec2 vertex;" \
        "attribute vec2 tex_coord;" \
        "uniform mat4 model;" \
        "uniform mat4 view;" \
        "uniform mat4 projection;" \
        "uniform mat4 vertex_transform;" \
        "varying vec4 currentPosition;" \
        "void main()" \
        "{" \
        "    gl_TexCoord[0].xy       = tex_coord.xy;" \
        "    currentPosition         = vertex_transform*vec4(vertex, 1.0f, 1.0f);" \
        "    gl_Position             = projection*(view*(model*vertex_transform*vec4(vertex, 1.0f, 1.0f)));" \
        "}";
/*!
 * \brief TgShader2d::m_fragShader
 *
 * fragment shader
 */
const char *TgShader2d::m_fragShader = "" \
        "uniform int render_type;" \
        "uniform vec4 color;" \
        "uniform sampler2D texture;" \
        "varying vec4 currentPosition;" \
        "uniform vec4 maxRenderValues;" \
        "uniform float opacity;" \
        "void main()" \
        "{" \
        "   if (currentPosition.x < maxRenderValues.x || currentPosition.y < maxRenderValues.y || currentPosition.x > maxRenderValues.z || currentPosition.y > maxRenderValues.w) {" \
        "       discard;" \
        "   }" \
        "   if (render_type == 1) {" \
        "       if (texture2D(texture, gl_TexCoord[0].xy).r <= 0.0f) {" \
        "           discard;" \
        "       }" \
        "       gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);" \
        "       gl_FragColor.a = gl_FragColor.b*color.w*opacity;"
        "       gl_FragColor.r = gl_FragColor.r*color.x;"
        "       gl_FragColor.g = gl_FragColor.g*color.y;"
        "       gl_FragColor.b = gl_FragColor.b*color.z;"
        "    } else {" \
        "       gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);" \
        "       gl_FragColor.r = gl_FragColor.r*color.x;"
        "       gl_FragColor.g = gl_FragColor.g*color.y;"
        "       gl_FragColor.b = gl_FragColor.b*color.z;"
        "       gl_FragColor.a = gl_FragColor.a*color.w*opacity;"
        "    }" \
        "}";
/*!
 * \brief TgShader2d::TgShader2d
 *
 * constructor
 */
TgShader2d::TgShader2d() :
    m_generalShader(0)
{
}

/*!
 * \brief TgShader2d::~TgShader2d
 *
 * destructor
 */
TgShader2d::~TgShader2d()
{
    if (m_generalShader) {
        glDeleteProgram(m_generalShader);
    }
}

/*!
 * \brief TgShader2d::generalShader
 * \return general shader
 */
GLuint TgShader2d::generalShader()
{
    return m_generalShader;
}

/*!
 * \brief TgShader2d::init
 *
 * loads the shaders
 *
 * \return 0 on success
 */
int TgShader2d::init()
{
    GLuint shaderHandle = glCreateProgram();
    GLint status;
    GLuint fragShader;
    GLuint vertShader;


    if (compileShader(m_vertShader, GL_VERTEX_SHADER, &vertShader) != 0) {
        TG_ERROR_LOG("Compile shader failed vert");
        return -1;
    }
    glAttachShader(shaderHandle, vertShader);
    glDeleteShader(vertShader);

    if (compileShader(m_fragShader, GL_FRAGMENT_SHADER, &fragShader) != 0) {
        TG_ERROR_LOG("Compile shader failed frag");
        return -1;
    }
    glAttachShader(shaderHandle, fragShader);
    glDeleteShader(fragShader);

    glLinkProgram(shaderHandle);

    glGetProgramiv(shaderHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        TG_ERROR_LOG("The link operation is failed");
        return -1;
    }
    m_generalShader = shaderHandle;
    m_shaderAttributeIndex[ShaderAttributes2d::AttribPosition] = glGetAttribLocation(shaderHandle, "vertex");
    m_shaderAttributeIndex[ShaderAttributes2d::AttribTextCoord] = glGetAttribLocation(shaderHandle, "tex_coord");
    return 0;
}

/*!
 * \brief TgShader2d::compileShader
 *
 * compile the shader
 *
 * \param source, source text of the shader
 * \param type, type of the shader (vertex or frag)
 * \param shader, returns shader here, if return is 0
 * \return 0 on success
 */
int TgShader2d::compileShader(const char *source, const GLenum type, GLuint *shader)
{
    GLint status;
    GLuint shaderHandle = glCreateShader(type);

    if (shaderHandle == 0) {
        TG_ERROR_LOG("Creating the shader failed");
        return -1;
    }

    glShaderSource(shaderHandle, 1, &source, nullptr);
    glCompileShader(shaderHandle);

    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

    if(status == GL_FALSE) {
        glDeleteShader(shaderHandle);
        TG_ERROR_LOG("Compiling shader failed - compile status is false");
        return -1;
    }

    *shader = shaderHandle;
    return 0;
}
