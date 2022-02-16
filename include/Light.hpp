#pragma once

#include "my_math.hpp"

struct Material
{
    Color ambient;
    Color diffuse;
    Color specular;
    float shiness;
    float attenuation;
};

struct LightParams
{
    Vector3 pos;
    Vector3 dir;
    Vector3 ambientComponent;
    Vector3 diffuseComponent;
    Vector3 specularComponent;
};

struct Light
{
    LightParams params;

    Light(const LightParams& _p) : params(_p) {}
};

Color computePhong(Light    _light,
                   const Material& _mat,
                   Vector3  _pixel,
                   const Vector3&  _surfaceNormal,
                   const Vector3&  _viewVector);