#pragma once
#include "../utils/vector.h"
#include <math.h>

struct Matrix3x4 {
public:
    float matrix[3][4];

    Vector3 GetPosition() const {
        return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
    }

    Vector3 GetPosition2() const {
        return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
    }
};

struct ViewMatrix {
public:
    float matrix[4][4];

    Vector3 Transform(const Vector3 vector) const {
        Vector3 transformed;

        transformed.x = vector.y * matrix[0][1] + vector.x * matrix[0][0] + vector.z * matrix[0][2] + matrix[0][3];
        transformed.y = vector.y * matrix[1][1] + vector.x * matrix[1][0] + vector.z * matrix[1][2] + matrix[1][3];
        transformed.z = vector.y * matrix[3][1] + vector.x * matrix[3][0] + vector.z * matrix[3][2] + matrix[3][3];

        return transformed;
    }
};

#define Assert( _exp ) ((void)0)
struct matrix3x4_t
{
    matrix3x4_t() {}
    matrix3x4_t(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23)
    {
        m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
        m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
        m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
    }

    float* operator[](int i) { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
    const float* operator[](int i) const { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
    float* Base() { return &m_flMatVal[0][0]; }
    const float* Base() const { return &m_flMatVal[0][0]; }

    float m_flMatVal[3][4];
};


namespace math
{
    double distanceToMeters(float distance)
    {
        const float INCHES_TO_METER_RATE = 39.3701;
        return distance / INCHES_TO_METER_RATE;
    }
    double calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        float dx = (x1 - x2);
        float dy = (y1 - y2);
        float dz = (z1 - z2);
        float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
        return distance;
    }

    double calculateDistanceInMeters(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        return distanceToMeters(calculateDistance(x1, y1, z1, x2, y2, z2));
    }

    double calculateDistance2D(float x1, float y1, float x2, float y2)
    {
        float dx = (x1 - x2);
        float dy = (y1 - y2);
        float distance = sqrt(pow(dx, 2) + pow(dy, 2));
        return distance;
    }

};