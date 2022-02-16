#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>

Color computePhong(Light _light, const Material& _mat, Vector3 _pixel, const Vector3& _normal, const Vector3& _view)
{
    // Compute light direction vector.
    _light.params.dir = _pixel - _light.params.pos;
    _light.params.dir = _light.params.dir.getNormalized();

    // Compute result color of Phong's model.
    float result = (_normal & _light.params.dir);

    //! DEBUG PANNEL
    ImGui::Begin("Lighting rendering info");
    {
       ImGui::Text("Light position: %.2f, %.2f, %.2f\n", _light.params.pos.x, _light.params.pos.y, _light.params.pos.z);
       ImGui::Text("Pixel position (relative to world): %.2f, %.2f, %.2f\n", _pixel.x, _pixel.y, _pixel.z);
       ImGui::Text("Light direction (normalized): %.2f, %.2f, %.2f\n", _light.params.dir.x, _light.params.dir.y, _light.params.dir.z);
       ImGui::Text("Phong intensity: %.2f\n\n", result);
    }
    ImGui::End();

    /* TO IMPLEMENT IF BASIC LIGHT WORKED

    // Compute reflexion.
    Vector3 reflexion = _normal * 2 * (_normal & _light.params.dir) - _light.params.dir;

    // Compute result color of Phong's model.
    Vector3 result = _light.params.ambientComponent * _mat.ambient + _light.params.dir * 
                     (_mat.diffuse * (_normal & _light.params.dir) + _mat.specular     *
                     ((reflexion & _view) ^ _mat.shiness)) * _mat.attenuation;
    */

    return { result, result, result, 1 };
}
