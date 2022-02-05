/*!
 * \file
 * \brief file tg_matrix4x4.h
 *
 * Simple 4x4 matrix
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MATRIX4X4_H
#define TG_MATRIX4X4_H

typedef union
{
    float data[16];    // to get all matrix values
    struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    } matrix4x4Values;
} TgMatrix4x4Table;

class TgMatrix4x4
{
public:
    TgMatrix4x4();
    const TgMatrix4x4Table *getMatrixTable() const;
    void set(const TgMatrix4x4Table *m);

    void setTransform(float x, float y, float z = 0);
    void setScale(float x, float y, float z = 1);
    void clear();
    void setOrth(float left, float right, float top, float bottom, float znear, float zfar);

    void mul(const TgMatrix4x4Table *a, const TgMatrix4x4Table *b);
private:
    TgMatrix4x4Table m_matrixTable;
};

#endif // TG_MATRIX4X4_H
