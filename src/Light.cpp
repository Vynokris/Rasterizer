#include "my_math.hpp"

#include "Light.hpp"

Light::Light() { params = { Vector3(), 0.f, 0.f, 0.f }; }

Light::Light(const LightParams& p) : params(p) {}

void Light::setParams(const LightParams& p)
{
    params = p;
}