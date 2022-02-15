#include "my_math.hpp"

#include "Light.hpp"

Light::Light(const LightParams& _p) : params(_p) {}

void Light::setParams(const LightParams& _p)
{
    params = _p;
}

Color Light::computePhong(const Material& mat, const Vector3& pixel, const Vector3& surfaceNormal, const Vector3& viewVector)
{
    // Compute light direction vector.
    params.dir = params.pos - pixel;

    // Compute reflection.
    Vector3 reflexion = surfaceNormal * 2 * (surfaceNormal & params.dir) - params.dir;

    // Compute result color of Phong's model.
    Vector3 result    = params.ambientComponent * mat.ambient + params.dir * 
                       (mat.diffuse * (surfaceNormal & params.dir) + mat.specular * (reflexion & viewVector)) * mat.attenuation;

    return { result.x, result.y, result.z, 1 };
}