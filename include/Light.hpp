#pragma once

#include "my_math.hpp"

struct Material
{
    float   ambient;
    float   diffuse;
    float   specular;
    float   attenuation;
};

struct LightParams
{
    Vector3 pos;
    Vector3 dir;
    Vector3 ambientComponent;
    Vector3 diffuseComponent;
    Vector3 specularComponent;
};

class Light
{
private:
    LightParams params;

public:
    Light() {}
    Light(const LightParams& _p);
    ~Light() {}

    void setParams(const LightParams& _p);

    Color computePhong(const Material& mat,
                       const Vector3& pixel,
                       const Vector3& surfaceNormal,
                       const Vector3& viewVector);
};