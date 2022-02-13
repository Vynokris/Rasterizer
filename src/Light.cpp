#include "my_math.hpp"

#include "light.hpp"

Light::Light() { params = { Vector3(), 0.f, 0.f, 0.f }; }

Light:Light(const LightParams& p) : params(p) {}

void Light::setParams(LightParams& p)
{
    params = p;
}