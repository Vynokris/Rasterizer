#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>

using namespace std;
using namespace arithmetic;

Color computePhong(const vector<Light>& _lights, const Material& _mat, Vector3 _pixel, const Vector3& _normal, const Vector3& _view)
{
    Color lightIntensity = { 0, 0, 0, 0 };

    for (Light light : _lights)
    {
        // Create vectors.
        Vector3 camToPixel     = Vector3(_pixel, _view).getNormalized();
        Vector3 lightToPixel   = Vector3(_pixel, light.pos).getNormalized();
        Vector3 lightReflexion = (_normal * 2 * (lightToPixel & _normal) - lightToPixel).getNormalized();

        // Compute the intensity of diffuse and specular light.
        float diffuseFactor  = clamp(_normal & lightToPixel, 0, 1);
        float reflectionDot  = lightReflexion & camToPixel;
        float specularFactor = 0;
        if (reflectionDot > 0 && _mat.shiness > 0)
            specularFactor = pow(reflectionDot, _mat.shiness * 64);

        // Compute the colors of diffuse and specular light.
        float diffuse  = _mat.diffuse  * diffuseFactor;
        float specular = _mat.specular * specularFactor;

        //! Debug pannel.
        if (ImGui::Begin("Lighting rendering info"))
        {
            ImGui::Text("Light position: %.2f, %.2f, %.2f\n", light.pos.x, light.pos.y, light.pos.z);
            ImGui::Text("Pixel position (relative to world): %.2f, %.2f, %.2f\n", _pixel.x, _pixel.y, _pixel.z);
            ImGui::Text("Light direction (normalized): %.2f, %.2f, %.2f\n", lightToPixel.x, lightToPixel.y, lightToPixel.z);
            ImGui::Text("Light diffuse: %.2f", diffuseFactor);
            ImGui::Text("Light specular: %.2f\n\n", specularFactor);
        }
        ImGui::End();

        // Compute the output color.
        lightIntensity += light.color * (diffuse + specular);
    }
    lightIntensity.a = 1;

    if (ImGui::Begin("Lighting rendering info"))
        ImGui::Text("Light color: %2.f, %.2f, %.2f", lightIntensity.r, lightIntensity.g, lightIntensity.b);
    ImGui::End();

    return lightIntensity;
}
