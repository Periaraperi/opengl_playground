#pragma once

namespace peria::graphics {

struct Attenuation {
    // float constant {1.0f} // this is always one
    float linear {};
    float quadratic {};
};

constexpr Attenuation ATT_DISTANCE_7    {0.7f    ,1.8f};
constexpr Attenuation ATT_DISTANCE_13   {0.35f   ,0.44f};
constexpr Attenuation ATT_DISTANCE_20   {0.22f   ,0.20f};
constexpr Attenuation ATT_DISTANCE_32   {0.14f   ,0.07f};
constexpr Attenuation ATT_DISTANCE_50   {0.09f   ,0.032f};
constexpr Attenuation ATT_DISTANCE_65   {0.07f   ,0.017f};
constexpr Attenuation ATT_DISTANCE_100  {0.045f  ,0.0075f};
constexpr Attenuation ATT_DISTANCE_160  {0.027f  ,0.0028f};
constexpr Attenuation ATT_DISTANCE_200  {0.022f  ,0.0019f};
constexpr Attenuation ATT_DISTANCE_325  {0.014f  ,0.0007f};
constexpr Attenuation ATT_DISTANCE_600  {0.007f  ,0.0002f};
constexpr Attenuation ATT_DISTANCE_3250 {0.0014f ,0.000007f};

}
