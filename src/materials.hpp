#pragma once

#include <array>

namespace peria::graphics::materials {

struct Material {
    std::array<float, 3> ambient {0.1f, 0.1f, 0.1f};
    std::array<float, 3> diffuse {0.5f, 0.5f, 0.5f};
    std::array<float, 3> specular {1.0f, 1.0f, 1.0f};
    float shininess {32.0f};
};

// The following material values are taken from this website
// http://devernay.free.fr/cours/opengl/materials.html 

constexpr Material EMERALD {
    .ambient {0.0215, 0.1745, 0.0215f,}, 
    .diffuse {0.07568, 0.61424f, 0.07568f}, 
    .specular {0.633f, 0.727811, 0.633f}, 
    .shininess = 0.6f * 128.0f
};
constexpr Material JADE {
    .ambient {0.135f, 0.2225f, 0.1575f},
    .diffuse {0.54f, 0.89f, 0.63f},
    .specular {0.316228f, 0.316228f, 0.316228f},
    .shininess = 0.1f * 128.0f
};
constexpr Material OBSIDIAN {
    .ambient {0.05375f, 0.05f, 0.06625f},
    .diffuse {0.18275f, 0.17f, 0.22525f},
    .specular {0.332741f, 0.328634f, 0.346435f},
    .shininess = 0.3f * 128.0f
};
constexpr Material PEARL {
    .ambient {0.25f, 0.20725f, 0.20725f},
    .diffuse {1.0f, 0.829f, 0.829f},
    .specular {0.296648f, 0.296648f, 0.296648f},
    .shininess = 0.088f * 128.0f
};
constexpr Material RUBY {
    .ambient {0.1745f, 0.01175f, 0.01175f},
    .diffuse {0.61424f, 0.04136f, 0.04136f},
    .specular {0.727811f, 0.626959f, 0.626959f},
    .shininess = 0.6f * 128.0f
};
constexpr Material TURQUOISE {
    .ambient {0.1f, 0.18725f, 0.1745f},
    .diffuse {0.396f, 0.74151f, 0.69102f},
    .specular {0.297254f, 0.30829f, 0.306678f},
    .shininess = 0.1f * 128.0f
};
constexpr Material BRASS {
    .ambient {0.329412f, 0.223529f, 0.027451f},
    .diffuse {0.780392f, 0.568627f, 0.113725f},
    .specular {0.992157f, 0.941176f, 0.807843f},
    .shininess = 0.21794872f * 128.0f
};
constexpr Material BRONZE {
    .ambient {0.2125f, 0.1275f, 0.054f},
    .diffuse {0.714f, 0.4284f, 0.18144f},
    .specular {0.393548f, 0.271906f, 0.166721f},
    .shininess = 0.2f * 128.0f
};
constexpr Material CHROME {
    .ambient {0.25f, 0.25f, 0.25f},
    .diffuse {0.4f, 0.4f, 0.4f},
    .specular {0.774597f, 0.774597f, 0.774597f},
    .shininess = 0.6f * 128.0f
};
constexpr Material COPPER {
    .ambient {0.19125f, 0.0735f, 0.0225f},
    .diffuse {0.7038f, 0.27048f, 0.0828f},
    .specular {0.256777f, 0.137622f, 0.086014f},
    .shininess = 0.1f * 128.0f
};
constexpr Material GOLD {
    .ambient {0.24725f, 0.1995f, 0.0745f},
    .diffuse {0.75164f, 0.60648f, 0.22648f},
    .specular {0.628281f, 0.555802f, 0.366065f},
    .shininess = 0.4f * 128.0f
};
constexpr Material SILVER {
    .ambient {0.19225f, 0.19225f, 0.19225f},
    .diffuse {0.50754f, 0.50754f, 0.50754f},
    .specular {0.508273f, 0.508273f, 0.508273f},
    .shininess = 0.4f * 128.0f
};
constexpr Material BLACK_PLASTIC {
    .ambient {0.0f, 0.0f, 0.0f},
    .diffuse {0.01f, 0.01f, 0.01f},
    .specular {0.50f, 0.50f, 0.50f},
    .shininess = 0.25f * 128.0f
};
constexpr Material CYAN_PLASTIC {
    .ambient {0.0f, 0.1f, 0.06f},
    .diffuse {0.0f, 0.50980392f, 0.50980392f},
    .specular {0.50196078f, 0.50196078f, 0.50196078f},
    .shininess =0.25f * 128.0f
};
constexpr Material GREEN_PLASTIC {
    .ambient {0.0f, 0.0f, 0.0f},
    .diffuse {0.1f, 0.35f, 0.1f},
    .specular {0.45f, 0.55f, 0.45f},
    .shininess = 0.25f * 128.0f
};
constexpr Material RED_PLASTIC {
    .ambient {0.0f, 0.0f, 0.0f},
    .diffuse {0.5f, 0.0f, 0.0f},
    .specular {0.7f, 0.6f, 0.6f},
    .shininess = 0.25f * 128.0f
};
constexpr Material WHITE_PLASTIC {
    .ambient {0.0f, 0.0f, 0.0f},
    .diffuse {0.55f, 0.55f, 0.55f},
    .specular {0.70f, 0.70f, 0.70f},
    .shininess = 0.25f * 128.0f
};
constexpr Material YELLOW_PLASTIC {
    .ambient {0.0f, 0.0f, 0.0f},
    .diffuse {0.5f, 0.5f, 0.0f},
    .specular {0.60f, 0.60f, 0.50f},
    .shininess = 0.25f * 128.0f
};
constexpr Material BLACK_RUBBER {
    .ambient {0.02f, 0.02f, 0.02f},
    .diffuse {0.01f, 0.01f, 0.01f},
    .specular {0.4f, 0.4f, 0.4f},
    .shininess = 0.078125f * 128.0f
};
constexpr Material CYAN_RUBBER {
    .ambient {0.0f, 0.05f, 0.05f},
    .diffuse {0.4f, 0.5f, 0.5f},
    .specular {0.04f, 0.7f, 0.7f},
    .shininess = 0.078125f * 128.0f
};
constexpr Material GREEN_RUBBER {
    .ambient {0.0f, 0.05f, 0.0f},
    .diffuse {0.4f, 0.5f, 0.4f},
    .specular {0.04f, 0.7f, 0.04f},
    .shininess = 0.078125f * 128.0f
};
constexpr Material RED_RUBBER {
    .ambient {0.05f, 0.0f, 0.0f},
    .diffuse {0.5f, 0.4f, 0.4f},
    .specular {0.7f, 0.04f, 0.04f},
    .shininess = 0.078125f * 128.0f
};
constexpr Material WHITE_RUBBER {
    .ambient {0.05f, 0.05f, 0.05f},
    .diffuse {0.5f, 0.5f, 0.5f},
    .specular {0.7f, 0.7f, 0.7f},
    .shininess = 0.078125f * 128.0f
};
constexpr Material YELLOW_RUBBER {
    .ambient {0.05f, 0.05f, 0.0f},
    .diffuse {0.5f, 0.5f, 0.4f},
    .specular {0.7f, 0.7f, 0.04f},
    .shininess = 0.078125f * 128.0f
};

} 
