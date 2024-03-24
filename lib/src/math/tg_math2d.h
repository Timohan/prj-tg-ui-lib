/*!
 * \file
 * \brief file tg_math2d.h
 *
 * Math functions for 2d calc
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MATH_2D_H
#define TG_MATH_2D_H

class TgMath2d
{
public:
    static void rotateByAngle(const float x, const float y, float angle, float *outX, float *outY);
    static void rotateByAngle(const float fromX, const float fromY, const float toX, const float toY, float angleToRotate, float *outX, float *outY);
    static void rotateByAngle(const float fromX, const float fromY, const float toX, const float toY, const float angleToRotate, const float distance, float *outX, float *outY);
    static float calculateAngle(const float x, const float y);
    static bool moveDistance(const float fromX, const float fromY, const float toX, const float toY, const float distance, float *outX, float *outY);
    static bool calculateCrossPoint(const float fromX1, const float fromY1, const float toX1, const float toY1,
                                    const float fromX2, const float fromY2, const float toX2, const float toY2,
                                    float *crossPointX, float *crossPointY);
    static bool isInTriangle(const float pX, const float pY, const float x1, const float y1, const float x2, const float y2, const float x3, const float y3);
    static void isInQuadrilateralAA4(const float pX, const float pY,
                               const float x0, const float y0,
                               const float x1, const float y1,
                               const float x2, const float y2,
                               const float x3, const float y3,
                               bool *result);
    static void isInQuadrilateralAA8(const float pX, const float pY,
                               const float x0, const float y0,
                               const float x1, const float y1,
                               const float x2, const float y2,
                               const float x3, const float y3,
                               bool *result);
private:
    static float fixAngle(float angle);
    static void calculateLineRatio(const float fromX, const float fromY,
                                   const float toX, const float toY,
                                   float *ratioX, float *ratioY, float *ratioC);
    static float sign(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3);
};

#endif // TG_MATH_2D_H
