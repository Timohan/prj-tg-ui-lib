/*!
 * \file
 * \brief file tg_matrix4x4.cpp
 *
 * Simple 4x4 matrix
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_matrix4x4.h"
#include <string.h>

/*!
 * \brief TgMatrix4x4::TgMatrix4x4
 *
 * Constructor
 */
TgMatrix4x4::TgMatrix4x4()
{
    clear();
}

/*!
 * \brief TgMatrix4x4::getMatrixTable
 * \return matrix table
 */
const TgMatrix4x4Table *TgMatrix4x4::getMatrixTable() const
{
    return &m_matrixTable;
}

/*!
 * \brief TgMatrix4x4::set
 *
 * \param m matrix table to set here
 */
void TgMatrix4x4::set(const TgMatrix4x4Table *m)
{
    memcpy(&m_matrixTable, m, sizeof(TgMatrix4x4Table));
}

/*!
 * \brief TgMatrix4x4::clear
 *
 * clears the matrix 4x4
 */
void TgMatrix4x4::clear()
{
    memset(&m_matrixTable, 0, sizeof(TgMatrix4x4Table));
    m_matrixTable.matrix4x4Values.m00 = 1.0;
    m_matrixTable.matrix4x4Values.m11 = 1.0;
    m_matrixTable.matrix4x4Values.m22 = 1.0;
    m_matrixTable.matrix4x4Values.m33 = 1.0;
}

/*!
 * \brief TgMatrix4x4::setTransform
 *
 * sets transform position to matrix 4x4
 * \param x
 * \param y
 * \param z
 */
void TgMatrix4x4::setTransform(float x, float y, float z)
{
    clear();
    m_matrixTable.matrix4x4Values.m30 = x;
    m_matrixTable.matrix4x4Values.m31 = y;
    m_matrixTable.matrix4x4Values.m32 = z;
}

/*!
 * \brief TgMatrix4x4::setScale
 *
 * sets scaling to matrix 4x4
 *
 * \param x
 * \param y
 * \param z
 */
void TgMatrix4x4::setScale(float x, float y, float z)
{
    clear();
    m_matrixTable.matrix4x4Values.m00 = x;
    m_matrixTable.matrix4x4Values.m11 = y;
    m_matrixTable.matrix4x4Values.m22 = z;
}


/*!
 * \brief TgMatrix4x4::setOrth
 *
 * set orthographic matrix
 *
 * \param left
 * \param right
 * \param bottom
 * \param top
 * \param znear
 * \param zfar
 */
void TgMatrix4x4::setOrth(float left, float right, float top, float bottom, float znear, float zfar)
{
    memset(&m_matrixTable, 0, sizeof(TgMatrix4x4Table));

    m_matrixTable.matrix4x4Values.m00 = +2.0f/(right-left);
    m_matrixTable.matrix4x4Values.m30 = -(right+left)/(right-left);
    m_matrixTable.matrix4x4Values.m11 = +2.0f/(top-bottom);
    m_matrixTable.matrix4x4Values.m31 = -(top+bottom)/(top-bottom);
    m_matrixTable.matrix4x4Values.m22 = -2.0f/(zfar-znear);
    m_matrixTable.matrix4x4Values.m32 = -(zfar+znear)/(zfar-znear);
    m_matrixTable.matrix4x4Values.m33 = 1.0f;
}

/*!
 * \brief TgMatrix4x4::mul
 *
 * inits the window
 *
 * puts this matrix table a*b
 *
 * \param a
 * \param b
 */
void TgMatrix4x4::mul(const TgMatrix4x4Table *a, const TgMatrix4x4Table *b)
{
    m_matrixTable.data[0] = a->data[0]*b->data[0] + a->data[1]*b->data[4] + a->data[2]*b->data[8] + a->data[3]*b->data[12];
    m_matrixTable.data[1] = a->data[0]*b->data[1] + a->data[1]*b->data[5] + a->data[2]*b->data[9] + a->data[3]*b->data[13];
    m_matrixTable.data[2] = a->data[0]*b->data[2] + a->data[1]*b->data[6] + a->data[2]*b->data[10] + a->data[3]*b->data[14];
    m_matrixTable.data[3] = a->data[0]*b->data[3] + a->data[1]*b->data[7] + a->data[2]*b->data[11] + a->data[3]*b->data[15];

    m_matrixTable.data[4] = a->data[4]*b->data[0] + a->data[5]*b->data[4] + a->data[6]*b->data[8] + a->data[7]*b->data[12];
    m_matrixTable.data[5] = a->data[4]*b->data[1] + a->data[5]*b->data[5] + a->data[6]*b->data[9] + a->data[7]*b->data[13];
    m_matrixTable.data[6] = a->data[4]*b->data[2] + a->data[5]*b->data[6] + a->data[6]*b->data[10] + a->data[7]*b->data[14];
    m_matrixTable.data[7] = a->data[4]*b->data[3] + a->data[5]*b->data[7] + a->data[6]*b->data[11] + a->data[7]*b->data[15];

    m_matrixTable.data[8] = a->data[8]*b->data[0] + a->data[9]*b->data[4] + a->data[10]*b->data[8] + a->data[11]*b->data[12];
    m_matrixTable.data[9] = a->data[8]*b->data[1] + a->data[9]*b->data[5] + a->data[10]*b->data[9] + a->data[11]*b->data[13];
    m_matrixTable.data[10] = a->data[8]*b->data[2] + a->data[9]*b->data[6] + a->data[10]*b->data[10] + a->data[11]*b->data[14];
    m_matrixTable.data[11] = a->data[8]*b->data[3] + a->data[9]*b->data[7] + a->data[10]*b->data[11] + a->data[11]*b->data[15];

    m_matrixTable.data[12] = a->data[12]*b->data[0] + a->data[13]*b->data[4] + a->data[14]*b->data[8] + a->data[15]*b->data[12];
    m_matrixTable.data[13] = a->data[12]*b->data[1] + a->data[13]*b->data[5] + a->data[14]*b->data[9] + a->data[15]*b->data[13];
    m_matrixTable.data[14] = a->data[12]*b->data[2] + a->data[13]*b->data[6] + a->data[14]*b->data[10] + a->data[15]*b->data[14];
    m_matrixTable.data[15] = a->data[12]*b->data[3] + a->data[13]*b->data[7] + a->data[14]*b->data[11] + a->data[15]*b->data[15];
}
