#include "tg_math2d.h"
#include <math.h>
#include <numbers>

#define PI_VALUE static_cast<float>(M_PI)

/*!
 * \brief TgMath2d::calculateAngle
 * calculates angle of x/y
 * \param x
 * \param y
 * \return angle
 */
float TgMath2d::calculateAngle(const float x, const float y)
{
    if (std::fabs(x) <= std::numeric_limits<float>::epsilon()) {
        if (y >= 0) {
            return 0;
        }
        return PI_VALUE;
    }
    if (std::fabs(y) <= std::numeric_limits<float>::epsilon()) {
        if (x >= 0) {
            return PI_VALUE/2.0f;
        }
        return PI_VALUE + PI_VALUE/2.0f;
    }
    if (x >= 0 && y >= 0) {
        return atanf(x/y);
    }   
    if (x >= 0 && y <= 0) {
        return PI_VALUE + atanf(x/y);
    }
    if (x <= 0 && y <= 0) {
        return PI_VALUE + atanf(x/y);
    }
    return PI_VALUE*2 + atanf(x/y);
}

/*!
 * \brief TgMath2d::rotateByAngle
 * rotate (x/y) with angle
 * \param x
 * \param y
 * \param angle [in] angle to rotate
 * \param outX [out] rotated x
 * \param outY [out] rotated y
 */
void TgMath2d::rotateByAngle(const float x, const float y, float angle, float *outX, float *outY)
{
    float distance;
    angle += calculateAngle(x, y);
    distance = sqrtf(x*x + y*y);
    angle = fixAngle(angle);
    *outX = sinf(angle)*distance;
    *outY = cosf(angle)*distance;
}

/*!
 * \brief TgMath2d::rotateByAngle
 * rotate (toX-fromX) / (toY-fromY) with angle
 * \param fromX
 * \param fromY
 * \param toX
 * \param toY
 * \param angleToRotate [in] angle to rotate
 * \param outX [out] rotated x (related to fromX)
 * \param outY [out] rotated y (related to fromY)
 */
void TgMath2d::rotateByAngle(const float fromX, const float fromY, const float toX, const float toY, float angleToRotate, float *outX, float *outY)
{
    rotateByAngle(toX-fromX, toY-fromY, angleToRotate, outX, outY);
    *outX = *outX + fromX;
    *outY = *outY + fromY;
}

/*!
 * \brief TgMath2d::rotateByAngle
 * rotate (toX-fromX) / (toY-fromY) with angle
 * \param fromX
 * \param fromY
 * \param toX
 * \param toY
 * \param angleToRotate [in] angle to rotate
 * \param distance [in] distance of rotated line
 * \param outX [out] rotated x (related to fromX)
 * \param outY [out] rotated y (related to fromY)
 */
void TgMath2d::rotateByAngle(const float fromX, const float fromY, const float toX, const float toY, const float angleToRotate, const float distance, float *outX, float *outY)
{
    rotateByAngle(fromX, fromY, toX, toY, angleToRotate, outX, outY);
    moveDistance(fromX, fromY, *outX, *outY, distance, outX, outY);
}



/*!
 * \brief TgMath2d::fixAngle
 * if angle value is < 0 or >= PI*2
 * it will be fixed to >= 0 && <= PI*2
 * \return fixed angle
 */
float TgMath2d::fixAngle(float angle)
{
    while (1) {
        if (angle >= 0 && angle < PI_VALUE*2) {
            return angle;
        }
        if (angle < 0) {
            angle += PI_VALUE*2;
        } else {
            angle -= PI_VALUE*2;
        }
    }
}

/*!
 * \brief TgMath2d::moveDistance
 *
 * move distance from fromX/fromY to toX/toY direction with distance
 *
 * \param fromX from this X
 * \param fromY from this Y
 * \param toX to this X direction
 * \param toY to this Y direction
 * \param distance distance to move
 * \param outX [out] moved to this X
 * \param outY [out] moved to this Y
 */
bool TgMath2d::moveDistance(const float fromX, const float fromY, const float toX, const float toY, const float distance, float *outX, float *outY)
{
    float diffX = toX - fromX;
    float diffY = toY - fromY;

    if (std::fabs(diffX + diffY) <= std::numeric_limits<float>::epsilon()) {
        *outX = fromX;
        *outY = fromY;
        return false;
    }

    float fullDistance = sqrtf(diffX*diffX + diffY*diffY);
    *outX = diffX*distance/fullDistance + fromX;
    *outY = diffY*distance/fullDistance + fromY;
    return true;
}


/*!
 * \brief TgMath2d::calculateLineRatio
 *
 * calculate line ratio (x, y, c) from the line (from to to)
 *
 * \param fromX from this X
 * \param fromY from this Y
 * \param toX to this X direction
 * \param toY to this Y direction
 * \param ratioX [out] ratio X, also calculated as "A"
 * \param ratioY [out] ratio Y, also calculated as "B"
 * \param ratioC [out] ratio C
 */
void TgMath2d::calculateLineRatio(const float fromX, const float fromY,
                                  const float toX, const float toY,
                                  float *ratioX, float *ratioY, float *ratioC)
{
    float fromX2;
    float fromY2;
    float toX2;
    float toY2;

    if (fromX < toX) {
        fromX2 = fromX;
        fromY2 = fromY;
        toX2 = toX;
        toY2 = toY;
    } else {
        toX2 = fromX;
        toY2 = fromY;
        fromX2 = toX;
        fromY2 = toY;
    }

    const float pointRatioX = toX2 - fromX2;
    const float pointRatioY = toY2 - fromY2;
    *ratioY = 1; 
    *ratioX = pointRatioY/pointRatioX;
    if (std::fabs(pointRatioX) <= 0 || std::fabs(pointRatioY) <= 0) {
        *ratioC = 0;
    } else {
        *ratioC = (fromY - pointRatioY*fromX/pointRatioX);
    }
}

/*!
 * \brief TgMath2d::calculateCrossPoint
 *
 * calculating cross point between line
 *
 * \param fromX1 from this X (line1)
 * \param fromY1 from this Y (line1)
 * \param toX1 to this X direction (line1)
 * \param toY1 to this Y direction (line1)
 * \param fromX2 from this X (line2)
 * \param fromY2 from this Y (line2)
 * \param toX2 to this X direction (line2)
 * \param toY2 to this Y direction (line2)
 * \param crossPointX [out] cross point (X)
 * \param crossPointY [out] cross point (Y)
 * \return true if cross point calc was possible
 */
bool TgMath2d::calculateCrossPoint(const float fromX1, const float fromY1, const float toX1, const float toY1,
                                   const float fromX2, const float fromY2, const float toX2, const float toY2,
                                   float *crossPointX, float *crossPointY)
{
    float ratioX1, ratioY1, ratioC1;
    float ratioX2, ratioY2, ratioC2;
    bool okX;
    bool okY;
    float tmpX, tmpY;

    calculateLineRatio(fromX1, fromY1, toX1, toY1, &ratioX1, &ratioY1, &ratioC1);
    calculateLineRatio(fromX2, fromY2, toX2, toY2, &ratioX2, &ratioY2, &ratioC2);

    float dx = ratioC1*ratioY2 - ratioC2*ratioY1;
    float dy = ratioX1*ratioC2 - ratioX2*ratioC1;
    float d = ratioX1*ratioY2 - ratioX2*ratioY1;

    if ((fromX1 > toX1 || fromX1 < toX1) &&
        (fromY1 > toY1 || fromY1 < toY1) &&
        (fromX2 > toX2 || fromX2 < toX2) &&
        (fromY2 > toY2 || fromY2 < toY2)) {
        if (d > 0 || d < 0) {
            *crossPointX = -dx/d;
            *crossPointY = dy/d;
            return true;
        }
        return false;
    } else {
        okX = false;
        if (fromX1 > toX1 || fromX1 < toX1) {
            if (fromX2 > toX2 || fromX2 < toX2) {
                tmpX = dx;
            } else {
                tmpX = toX2;
                okX = true;
            }
        } else {
            tmpX = toX1;
            okX = true;
        }
        okY = false;
        if (fromY1 > toY1 || fromY1 < toY1) {
            if (fromY2 > toY2 || fromY2 < toY2) {
                tmpY = dy;
            } else {
                tmpY = toY2;
                okY = true;
            }
        } else {
            tmpY = toY1;
            okY = true;
        }

        if (okX != okY) {
            if (okY == false) {
                if (fromX1 > toX1 || fromX1 < toX1) {
                    dx = fromX1 - toX1;
                    dy = fromY1 - toY1;
                    d = fromX1 - toX2;
                    tmpY = -d*dy/dx + fromY1;
                } else {
                    dx = fromX2 - toX2;
                    if (dx > 0 || dx < 0) {
                        dy = fromY2 - toY2;
                        d = fromX2 - toX1;
                        tmpY = - d*dy/dx + fromY2;
                    } else {
                        return false;
                    }
                }
            } else {
                if (fromY1 < toY1 || fromY1 > toY1) { 
                    dx = fromY1 - toY1;
                    dy = fromX1 - toX1;
                    d = fromY1 - toY2;
                    tmpX = -d*dy/dx + fromX1;
                } else {
                    dx = fromY2 - toY2;
                    if (dx > 0 || dx < 0) {
                        dy = fromX2 - toX2;
                        d = fromY2 - toY1;
                        tmpX = - d*dy/dx + fromX2;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    *crossPointX = tmpX;
    *crossPointY = tmpY;
    return true;
}

float TgMath2d::sign(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
{
    return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

/*!
 * \brief TgMath2d::isInTriangle
 *
 * \return true if pX/pY is in the triangle corners x1/y1, x2/y2, x3/y3
 */
bool TgMath2d::isInTriangle(const float pX, const float pY, const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
{
    const float d1 = sign(pX, pY, x1, y1, x2, y2);
    const float d2 = sign(pX, pY, x2, y2, x3, y3);
    const float d3 = sign(pX, pY, x3, y3, x1, y1);

    if ((d1 < 0 || d2 < 0 || d3 < 0) && (d1 > 0 || d2 > 0 || d3 > 0)) {
        return false;
    }

    return true;
}

/*!
 * \brief TgMath2d::isInQuadrilateralAA4
 * is pX/pY in the quadrilateral (x0/y0, x1/y1, x2/y2, x3/y3)
 *
 * x0/y0 -> next corner to clock wise must be x1/y1
 * x1/y1 -> next corner to clock wise must be x2/y2
 * x2/y2 -> next corner to clock wise must be x3/y3
 * x3/y3 -> next corner to clock wise must be x0/y0
 * 
 * \param result (must be at least size 4),
 * result[0] is top-left
 * result[1] is top-right
 * result[2] is bottom-left
 * result[3] is bottom-right
 */
void TgMath2d::isInQuadrilateralAA4(const float pX, const float pY,
                               const float x0, const float y0,
                               const float x1, const float y1,
                               const float x2, const float y2,
                               const float x3, const float y3,
                               bool *result)
{
    if (isInTriangle(pX+0.24f, pY+0.26f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.24f, pY+0.26f, x0, y0, x2, y2, x3, y3)) {
        result[0] = true;
    } else {
        result[0] = false;
    }

    if (isInTriangle(pX+0.74f, pY+0.24f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.74f, pY+0.24f, x0, y0, x2, y2, x3, y3)) {
        result[1] = true;
    } else {
        result[1] = false;
    }

    if (isInTriangle(pX+0.26f, pY+0.76f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.26f, pY+0.76f, x0, y0, x2, y2, x3, y3)) {
        result[2] = true;
    } else {
        result[2] = false;
    }

    if (isInTriangle(pX+0.76f, pY+0.74f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.76f, pY+0.74f, x0, y0, x2, y2, x3, y3)) {
        result[3] = true;
    } else {
        result[3] = false;
    }
}

/*!
 * \brief TgMath2d::isInQuadrilateralAA8
 * is pX/pY in the quadrilateral (x0/y0, x1/y1, x2/y2, x3/y3)
 *
 * x0/y0 -> next corner to clock wise must be x1/y1
 * x1/y1 -> next corner to clock wise must be x2/y2
 * x2/y2 -> next corner to clock wise must be x3/y3
 * x3/y3 -> next corner to clock wise must be x0/y0
 * 
 * \param result (must be at least size 8),
 * result[0] is top-left
 * result[1] is middle-top
 * result[2] is top-right
 * result[3] is bottom-left
 * result[4] is middle-middle
 * result[5] is middle-right
 * result[6] is bottom-left
 * result[7] is bottom-right
 */
void TgMath2d::isInQuadrilateralAA8(const float pX, const float pY,
                               const float x0, const float y0,
                               const float x1, const float y1,
                               const float x2, const float y2,
                               const float x3, const float y3,
                               bool *result)
{
    if (isInTriangle(pX+0.2f, pY+0.2f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.2f, pY+0.2f, x0, y0, x2, y2, x3, y3)) {
        result[0] = true;
    } else {
        result[0] = false;
    }

    if (isInTriangle(pX+0.55f, pY+0.1f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.55f, pY+0.1f, x0, y0, x2, y2, x3, y3)) {
        result[1] = true;
    } else {
        result[1] = false;
    }

    if (isInTriangle(pX+0.9f, pY+0.3f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.9f, pY+0.3f, x0, y0, x2, y2, x3, y3)) {
        result[2] = true;
    } else {
        result[2] = false;
    }

    if (isInTriangle(pX+0.1f, pY+0.7f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.1f, pY+0.7f, x0, y0, x2, y2, x3, y3)) {
        result[3] = true;
    } else {
        result[3] = false;
    }
    if (isInTriangle(pX+0.4f, pY+0.4f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.4f, pY+0.4f, x0, y0, x2, y2, x3, y3)) {
        result[4] = true;
    } else {
        result[4] = false;
    }
    if (isInTriangle(pX+0.6f, pY+0.55f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.6f, pY+0.55f, x0, y0, x2, y2, x3, y3)) {
        result[5] = true;
    } else {
        result[5] = false;
    }
    if (isInTriangle(pX+0.35f, pY+0.9f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.35f, pY+0.9f, x0, y0, x2, y2, x3, y3)) {
        result[6] = true;
    } else {
        result[6] = false;
    }
    if (isInTriangle(pX+0.66f, pY+0.81f, x0, y0, x1, y1, x2, y2)
        || isInTriangle(pX+0.66f, pY+0.81f, x0, y0, x2, y2, x3, y3)) {
        result[7] = true;
    } else {
        result[7] = false;
    }
}