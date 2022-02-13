#pragma once

#include "my_math.hpp"

struct LightParams
{
    Vector3 pos;
    float ambientComponent;
    float diffuseComponent;
    float specularComponent;
};

class Light
{
private:
    LightParams params;

public:
    Light();
    Light(const LightParams& p);
    ~Light();

    void setParams(const LightParams& p);
};