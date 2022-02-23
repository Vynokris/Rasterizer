#pragma once

#include "my_math.hpp"

struct Material { float diffuse, specular, shiness; };

struct Light
{
    Vector3 pos;
    Color color;
};

Color computePhong(const std::vector<Light>& _light,
                   const Material&           _mat,
                         Vector3             _pixel,
                   const Vector3&            _surfaceNormal,
                   const Vector3&            _viewVector);