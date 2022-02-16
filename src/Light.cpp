#include "my_math.hpp"

#include "Light.hpp"

Color computePhong(Light _light, const Material& _mat, Vector3 _pixel, const Vector3& _normal, const Vector3& _view)
{
    // Compute light direction vector.
    _light.params.dir = _light.params.pos - _pixel;
    _light.params.dir = _light.params.dir.getNormalized();

    // Compute result color of Phong's model.
    float result = _normal & _light.params.dir;

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
