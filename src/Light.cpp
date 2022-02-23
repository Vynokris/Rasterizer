#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>

using namespace std;
using namespace arithmetic;

Color computePhong(const vector<Light>& _lights, const Material& _mat, const Vector3& _pixel, const Vector3& _normal, const Vector3& _view)
{
    Color lightIntensity = { 0, 0, 0, 0 };

    for (Light light : _lights)
    {
        // Create vectors.
        Vector3 camToPixel       = Vector3(_pixel, _view).getNormalized();
        Vector3 lightToPixel     = Vector3(_pixel, light.pos).getNormalized();
        Vector3 lightReflexion   = (_normal * 2 * (lightToPixel & _normal) - lightToPixel).getNormalized();
        float   lightToPixelDist = Vector3(_pixel, light.pos).getLength();

        // Compute the intensity of diffuse and specular light.
        float diffuseFactor  = clamp(_normal & lightToPixel, 0, 1);
        float reflectionDot  = lightReflexion & camToPixel;
        float specularFactor = 0;
        if (reflectionDot > 0 && _mat.shininess > 0)
            specularFactor = pow(reflectionDot, _mat.shininess * 64);

        // Compute the attenuation factor.
        float attenuation = 0;
        if (lightToPixelDist < light.range)
        {
            attenuation = 1 / (light.constantAttenuation                           + 
                               light.linearAttenuation    *       lightToPixelDist + 
                               light.quadraticAttenuation * sqpow(lightToPixelDist));
        }

        // Compute the colors of diffuse and specular light.
        float diffuse  = _mat.diffuse  * diffuseFactor;
        float specular = _mat.specular * specularFactor;

        // Compute the output color.
        lightIntensity += light.color * (diffuse + specular) * attenuation;
    }
    lightIntensity.a = 1;

    return lightIntensity;
}
