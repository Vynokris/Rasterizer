#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>


using namespace arithmetic;

Color computePhong(Light _light, const Material& _mat, Vector3 _pixel, const Vector3& _normal, const Vector3& _view)
{
    // Create vectors.
    Vector3 camToPixel     = Vector3(_pixel, _view).getNormalized();
    Vector3 lightToPixel   = Vector3(_pixel, _light.params.pos).getNormalized();
    Vector3 lightReflexion = (_normal * 2 * (lightToPixel & _normal) - lightToPixel).getNormalized();

    // Compute the intensity of diffuse and specular light.
    float diffuseFactor  = clamp(_normal & lightToPixel, 0, 1);
    float reflectionDot = lightReflexion & camToPixel;
    float specularFactor = 0;
    if (reflectionDot > 0)
        specularFactor = pow(reflectionDot, acos(reflectionDot));

    // Compute the colors of diffuse and specular light.
    Vector3 diffuse  = _light.params.diffuseComponent  * diffuseFactor;
    Vector3 specular = _light.params.specularComponent * specularFactor;

    //! Debug pannel.
    ImGui::Begin("Lighting rendering info");
    {
    ImGui::Text("Light position: %.2f, %.2f, %.2f\n", _light.params.pos.x, _light.params.pos.y, _light.params.pos.z);
    ImGui::Text("Pixel position (relative to world): %.2f, %.2f, %.2f\n", _pixel.x, _pixel.y, _pixel.z);
    ImGui::Text("Light direction (normalized): %.2f, %.2f, %.2f\n", lightToPixel.x, lightToPixel.y, lightToPixel.z);
    ImGui::Text("Light diffuse: %.2f", diffuseFactor);
    ImGui::Text("Light specular: %.2f\n\n", specularFactor);
    }
    ImGui::End();

    // Compute the output color.
    // TODO: specular is broken idk why.
    Vector3 lightIntensity = diffuse/* + specular*/;

    return { lightIntensity.x, lightIntensity.y, lightIntensity.z, 1 };
}
