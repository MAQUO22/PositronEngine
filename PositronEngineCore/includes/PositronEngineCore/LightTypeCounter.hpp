#pragma once

namespace PositronEngine
{
    class LightTypeCounter
    {
        public:
            static int getNumberOfPointLights();
            static int getNumberOfSpotLights();

            static void incrementPointLightCount();
            static void incrementSpotLightCount();

            static void decrementPointLightCount();
            static void decrementSpotLightCount();

        private:
            static int _num_point_lights;
            static int _num_spot_lights;
    };
}
