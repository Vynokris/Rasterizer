#pragma once

#include <cmath>

#include "types.hpp"

//Remplacer par vos maths


// Constant and common maths functions
namespace maths
{
    const float TAU = 6.283185307179586476925f;

    inline float cos(float x) { return cosf(x); }
    inline float sin(float x) { return sinf(x); }
    inline float tan(float x) { return tanf(x); }
}

namespace mat4
{
    inline mat4x4 identity()
    {
        return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
        };
    }

    //mat4x4 frustum(float left, float right, float bottom, float top, float near, float far);
    //mat4x4 perspective(float fovY, float aspect, float near, float far);
    //mat4x4 rotateY(float angleRadians);
}

//inline float4 operator*(const mat4x4& m, float4 v)
//{
//}
//
//inline mat4x4 operator*(const mat4x4& a, const mat4x4& b)
//{
//}
//
//inline float3 operator/(float3 v, float a)
//{
//}
