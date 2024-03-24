#include "PositronEngineCore/LightTypeCounter.hpp"

namespace PositronEngine
{

    int LightTypeCounter::_num_point_lights = 0;
    int LightTypeCounter::_num_spot_lights = 0;

    int LightTypeCounter::getNumberOfPointLights()
    {
        return _num_point_lights;
    }

    int LightTypeCounter::getNumberOfSpotLights()
    {
        return _num_spot_lights;
    }

    void LightTypeCounter::incrementPointLightCount()
    {
        _num_point_lights++;
    }

    void LightTypeCounter::incrementSpotLightCount()
    {
        _num_spot_lights++;
    }

    void LightTypeCounter::decrementPointLightCount()
    {
        _num_point_lights--;
    }

    void LightTypeCounter::decrementSpotLightCount()
    {
        _num_spot_lights--;
    }
}
