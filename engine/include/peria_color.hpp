#pragma once

#include <type_traits>

#include "peria_types.hpp"

template <typename T>
concept color_type = std::is_same_v<T, peria::u8> || std::is_same_v<T, float>;

template<typename T> requires color_type<T>
struct peria_color {
    constexpr peria_color()
    {
        static_assert(std::is_same_v<T, peria::u8> || std::is_same_v<T, float>);

        if constexpr (std::is_same_v<T, peria::u8>) {
            r = 255;
            g = 255;
            b = 255;
            a = 255;
        }
        else if constexpr (std::is_same_v<T, float>) {
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            a = 1.0f;
        }
    }

    constexpr peria_color(T r_, T g_, T b_, T a_)
        :r{r_}, g{g_}, b{b_}, a{a_}
    {}

    static constexpr peria_color<float> to_gl_color(const peria_color<peria::u8>& c)
    {
        return {
            c.r / 255.0f,
            c.g / 255.0f,
            c.b / 255.0f,
            c.a / 255.0f,
        };
    }
    
    T r{};
    T g{};
    T b{};
    T a{};
};

// the following colors are taken from this site:
// https://web.archive.org/web/20180301041827/https://prideout.net/archive/colors.php 
namespace peria::color {
constexpr peria_color<float> ALICEBLUE{0.941f, 0.973f, 1.000f, 1.0f};
constexpr peria_color<float> ANTIQUEWHITE {0.980f, 0.922f, 0.843f, 1.0f};
constexpr peria_color<float> AQUA {0.000f, 1.000f, 1.000f, 1.0f};
constexpr peria_color<float> AQUAMARINE {0.498f, 1.000f, 0.831f, 1.0f};
constexpr peria_color<float> AZURE {0.941f, 1.000f, 1.000f, 1.0f};
constexpr peria_color<float> BEIGE {0.961f, 0.961f, 0.863f, 1.0f};
constexpr peria_color<float> BISQUE {1.000f, 0.894f, 0.769f, 1.0f};
constexpr peria_color<float> BLACK {0.000f, 0.000f, 0.000f, 1.0f};
constexpr peria_color<float> BLANCHEDALMOND {1.000f, 0.922f, 0.804f, 1.0f};
constexpr peria_color<float> BLUE {0.000f, 0.000f, 1.000f, 1.0f};
constexpr peria_color<float> BLUEVIOLET {0.541f, 0.169f, 0.886f, 1.0f};
constexpr peria_color<float> BROWN {0.647f, 0.165f, 0.165f, 1.0f};
constexpr peria_color<float> BURLYWOOD {0.871f, 0.722f, 0.529f, 1.0f};
constexpr peria_color<float> CADETBLUE {0.373f, 0.620f, 0.627f, 1.0f};
constexpr peria_color<float> CHARTREUSE {0.498f, 1.000f, 0.000f, 1.0f};
constexpr peria_color<float> CHOCOLATE {0.824f, 0.412f, 0.118f, 1.0f};
constexpr peria_color<float> CORAL {1.000f, 0.498f, 0.314f, 1.0f};
constexpr peria_color<float> CORNFLOWERBLUE {0.392f, 0.584f, 0.929f, 1.0f};
constexpr peria_color<float> CORNSILK {1.000f, 0.973f, 0.863f, 1.0f};
constexpr peria_color<float> CRIMSON {0.863f, 0.078f, 0.235f, 1.0f};
constexpr peria_color<float> CYAN {0.000f, 1.000f, 1.000f, 1.0f};
constexpr peria_color<float> DARKBLUE {0.000f, 0.000f, 0.545f, 1.0f};
constexpr peria_color<float> DARKCYAN {0.000f, 0.545f, 0.545f, 1.0f};
constexpr peria_color<float> DARKGOLDENROD {0.722f, 0.525f, 0.043f, 1.0f};
constexpr peria_color<float> DARKGRAY {0.663f, 0.663f, 0.663f, 1.0f};
constexpr peria_color<float> DARKGREEN {0.000f, 0.392f, 0.000f, 1.0f};
constexpr peria_color<float> DARKGREY {0.663f, 0.663f, 0.663f, 1.0f};
constexpr peria_color<float> DARKKHAKI {0.741f, 0.718f, 0.420f, 1.0f};
constexpr peria_color<float> DARKMAGENTA {0.545f, 0.000f, 0.545f, 1.0f};
constexpr peria_color<float> DARKOLIVEGREEN {0.333f, 0.420f, 0.184f, 1.0f};
constexpr peria_color<float> DARKORANGE {1.000f, 0.549f, 0.000f, 1.0f};
constexpr peria_color<float> DARKORCHID {0.600f, 0.196f, 0.800f, 1.0f};
constexpr peria_color<float> DARKRED {0.545f, 0.000f, 0.000f, 1.0f};
constexpr peria_color<float> DARKSALMON {0.914f, 0.588f, 0.478f, 1.0f};
constexpr peria_color<float> DARKSEAGREEN {0.561f, 0.737f, 0.561f, 1.0f};
constexpr peria_color<float> DARKSLATEBLUE {0.282f, 0.239f, 0.545f, 1.0f};
constexpr peria_color<float> DARKSLATEGRAY {0.184f, 0.310f, 0.310f, 1.0f};
constexpr peria_color<float> DARKSLATEGREY {0.184f, 0.310f, 0.310f, 1.0f};
constexpr peria_color<float> DARKTURQUOISE {0.000f, 0.808f, 0.820f, 1.0f};
constexpr peria_color<float> DARKVIOLET {0.580f, 0.000f, 0.827f, 1.0f};
constexpr peria_color<float> DEEPPINK {1.000f, 0.078f, 0.576f, 1.0f};
constexpr peria_color<float> DEEPSKYBLUE {0.000f, 0.749f, 1.000f, 1.0f};
constexpr peria_color<float> DIMGRAY {0.412f, 0.412f, 0.412f, 1.0f};
constexpr peria_color<float> DIMGREY {0.412f, 0.412f, 0.412f, 1.0f};
constexpr peria_color<float> DODGERBLUE {0.118f, 0.565f, 1.000f, 1.0f};
constexpr peria_color<float> FIREBRICK {0.698f, 0.133f, 0.133f, 1.0f};
constexpr peria_color<float> FLORALWHITE {1.000f, 0.980f, 0.941f, 1.0f};
constexpr peria_color<float> FORESTGREEN {0.133f, 0.545f, 0.133f, 1.0f};
constexpr peria_color<float> FUCHSIA {1.000f, 0.000f, 1.000f, 1.0f};
constexpr peria_color<float> GAINSBORO {0.863f, 0.863f, 0.863f, 1.0f};
constexpr peria_color<float> GHOSTWHITE {0.973f, 0.973f, 1.000f, 1.0f};
constexpr peria_color<float> GOLD {1.000f, 0.843f, 0.000f, 1.0f};
constexpr peria_color<float> GOLDENROD {0.855f, 0.647f, 0.125f, 1.0f};
constexpr peria_color<float> GRAY {0.502f, 0.502f, 0.502f, 1.0f};
constexpr peria_color<float> GREEN {0.000f, 0.502f, 0.000f, 1.0f};
constexpr peria_color<float> GREENYELLOW {0.678f, 1.000f, 0.184f, 1.0f};
constexpr peria_color<float> GREY {0.502f, 0.502f, 0.502f, 1.0f};
constexpr peria_color<float> HONEYDEW {0.941f, 1.000f, 0.941f, 1.0f};
constexpr peria_color<float> HOTPINK {1.000f, 0.412f, 0.706f, 1.0f};
constexpr peria_color<float> INDIANRED {0.804f, 0.361f, 0.361f, 1.0f};
constexpr peria_color<float> INDIGO {0.294f, 0.000f, 0.510f, 1.0f};
constexpr peria_color<float> IVORY {1.000f, 1.000f, 0.941f, 1.0f};
constexpr peria_color<float> KHAKI {0.941f, 0.902f, 0.549f, 1.0f};
constexpr peria_color<float> LAVENDER {0.902f, 0.902f, 0.980f, 1.0f};
constexpr peria_color<float> LAVENDERBLUSH {1.000f, 0.941f, 0.961f, 1.0f};
constexpr peria_color<float> LAWNGREEN {0.486f, 0.988f, 0.000f, 1.0f};
constexpr peria_color<float> LEMONCHIFFON {1.000f, 0.980f, 0.804f, 1.0f};
constexpr peria_color<float> LIGHTBLUE {0.678f, 0.847f, 0.902f, 1.0f};
constexpr peria_color<float> LIGHTCORAL {0.941f, 0.502f, 0.502f, 1.0f};
constexpr peria_color<float> LIGHTCYAN {0.878f, 1.000f, 1.000f, 1.0f};
constexpr peria_color<float> LIGHTGOLDENRODYELLOW {0.980f, 0.980f, 0.824f, 1.0f};
constexpr peria_color<float> LIGHTGRAY {0.827f, 0.827f, 0.827f, 1.0f};
constexpr peria_color<float> LIGHTGREEN {0.565f, 0.933f, 0.565f, 1.0f};
constexpr peria_color<float> LIGHTGREY {0.827f, 0.827f, 0.827f, 1.0f};
constexpr peria_color<float> LIGHTPINK {1.000f, 0.714f, 0.757f, 1.0f};
constexpr peria_color<float> LIGHTSALMON {1.000f, 0.627f, 0.478f, 1.0f};
constexpr peria_color<float> LIGHTSEAGREEN {0.125f, 0.698f, 0.667f, 1.0f};
constexpr peria_color<float> LIGHTSKYBLUE {0.529f, 0.808f, 0.980f, 1.0f};
constexpr peria_color<float> LIGHTSLATEGRAY {0.467f, 0.533f, 0.600f, 1.0f};
constexpr peria_color<float> LIGHTSLATEGREY {0.467f, 0.533f, 0.600f, 1.0f};
constexpr peria_color<float> LIGHTSTEELBLUE {0.690f, 0.769f, 0.871f, 1.0f};
constexpr peria_color<float> LIGHTYELLOW {1.000f, 1.000f, 0.878f, 1.0f};
constexpr peria_color<float> LIME {0.000f, 1.000f, 0.000f, 1.0f};
constexpr peria_color<float> LIMEGREEN {0.196f, 0.804f, 0.196f, 1.0f};
constexpr peria_color<float> LINEN {0.980f, 0.941f, 0.902f, 1.0f};
constexpr peria_color<float> MAGENTA {1.000f, 0.000f, 1.000f, 1.0f};
constexpr peria_color<float> MAROON {0.502f, 0.000f, 0.000f, 1.0f};
constexpr peria_color<float> MEDIUMAQUAMARINE {0.400f, 0.804f, 0.667f, 1.0f};
constexpr peria_color<float> MEDIUMBLUE {0.000f, 0.000f, 0.804f, 1.0f};
constexpr peria_color<float> MEDIUMORCHID {0.729f, 0.333f, 0.827f, 1.0f};
constexpr peria_color<float> MEDIUMPURPLE {0.576f, 0.439f, 0.859f, 1.0f};
constexpr peria_color<float> MEDIUMSEAGREEN {0.235f, 0.702f, 0.443f, 1.0f};
constexpr peria_color<float> MEDIUMSLATEBLUE {0.482f, 0.408f, 0.933f, 1.0f};
constexpr peria_color<float> MEDIUMSPRINGGREEN {0.000f, 0.980f, 0.604f, 1.0f};
constexpr peria_color<float> MEDIUMTURQUOISE {0.282f, 0.820f, 0.800f, 1.0f};
constexpr peria_color<float> MEDIUMVIOLETRED {0.780f, 0.082f, 0.522f, 1.0f};
constexpr peria_color<float> MIDNIGHTBLUE {0.098f, 0.098f, 0.439f, 1.0f};
constexpr peria_color<float> MINTCREAM {0.961f, 1.000f, 0.980f, 1.0f};
constexpr peria_color<float> MISTYROSE {1.000f, 0.894f, 0.882f, 1.0f};
constexpr peria_color<float> MOCCASIN {1.000f, 0.894f, 0.710f, 1.0f};
constexpr peria_color<float> NAVAJOWHITE {1.000f, 0.871f, 0.678f, 1.0f};
constexpr peria_color<float> NAVY {0.000f, 0.000f, 0.502f, 1.0f};
constexpr peria_color<float> OLDLACE {0.992f, 0.961f, 0.902f, 1.0f};
constexpr peria_color<float> OLIVE {0.502f, 0.502f, 0.000f, 1.0f};
constexpr peria_color<float> OLIVEDRAB {0.420f, 0.557f, 0.137f, 1.0f};
constexpr peria_color<float> ORANGE {1.000f, 0.647f, 0.000f, 1.0f};
constexpr peria_color<float> ORANGERED {1.000f, 0.271f, 0.000f, 1.0f};
constexpr peria_color<float> ORCHID {0.855f, 0.439f, 0.839f, 1.0f};
constexpr peria_color<float> PALEGOLDENROD {0.933f, 0.910f, 0.667f, 1.0f};
constexpr peria_color<float> PALEGREEN {0.596f, 0.984f, 0.596f, 1.0f};
constexpr peria_color<float> PALETURQUOISE {0.686f, 0.933f, 0.933f, 1.0f};
constexpr peria_color<float> PALEVIOLETRED {0.859f, 0.439f, 0.576f, 1.0f};
constexpr peria_color<float> PAPAYAWHIP {1.000f, 0.937f, 0.835f, 1.0f};
constexpr peria_color<float> PEACHPUFF {1.000f, 0.855f, 0.725f, 1.0f};
constexpr peria_color<float> PERU {0.804f, 0.522f, 0.247f, 1.0f};
constexpr peria_color<float> PINK {1.000f, 0.753f, 0.796f, 1.0f};
constexpr peria_color<float> PLUM {0.867f, 0.627f, 0.867f, 1.0f};
constexpr peria_color<float> POWDERBLUE {0.690f, 0.878f, 0.902f, 1.0f};
constexpr peria_color<float> PURPLE {0.502f, 0.000f, 0.502f, 1.0f};
constexpr peria_color<float> RED {1.000f, 0.000f, 0.000f, 1.0f};
constexpr peria_color<float> ROSYBROWN {0.737f, 0.561f, 0.561f, 1.0f};
constexpr peria_color<float> ROYALBLUE {0.255f, 0.412f, 0.882f, 1.0f};
constexpr peria_color<float> SADDLEBROWN {0.545f, 0.271f, 0.075f, 1.0f};
constexpr peria_color<float> SALMON {0.980f, 0.502f, 0.447f, 1.0f};
constexpr peria_color<float> SANDYBROWN {0.957f, 0.643f, 0.376f, 1.0f};
constexpr peria_color<float> SEAGREEN {0.180f, 0.545f, 0.341f, 1.0f};
constexpr peria_color<float> SEASHELL {1.000f, 0.961f, 0.933f, 1.0f};
constexpr peria_color<float> SIENNA {0.627f, 0.322f, 0.176f, 1.0f};
constexpr peria_color<float> SILVER {0.753f, 0.753f, 0.753f, 1.0f};
constexpr peria_color<float> SKYBLUE {0.529f, 0.808f, 0.922f, 1.0f};
constexpr peria_color<float> SLATEBLUE {0.416f, 0.353f, 0.804f, 1.0f};
constexpr peria_color<float> SLATEGRAY {0.439f, 0.502f, 0.565f, 1.0f};
constexpr peria_color<float> SLATEGREY {0.439f, 0.502f, 0.565f, 1.0f};
constexpr peria_color<float> SNOW {1.000f, 0.980f, 0.980f, 1.0f};
constexpr peria_color<float> SPRINGGREEN {0.000f, 1.000f, 0.498f, 1.0f};
constexpr peria_color<float> STEELBLUE {0.275f, 0.510f, 0.706f, 1.0f};
constexpr peria_color<float> TAN {0.824f, 0.706f, 0.549f, 1.0f};
constexpr peria_color<float> TEAL {0.000f, 0.502f, 0.502f, 1.0f};
constexpr peria_color<float> THISTLE {0.847f, 0.749f, 0.847f, 1.0f};
constexpr peria_color<float> TOMATO {1.000f, 0.388f, 0.278f, 1.0f};
constexpr peria_color<float> TURQUOISE {0.251f, 0.878f, 0.816f, 1.0f};
constexpr peria_color<float> VIOLET {0.933f, 0.510f, 0.933f, 1.0f};
constexpr peria_color<float> WHEAT {0.961f, 0.871f, 0.702f, 1.0f};
constexpr peria_color<float> WHITE {1.000f, 1.000f, 1.000f, 1.0f};
constexpr peria_color<float> WHITESMOKE {0.961f, 0.961f, 0.961f, 1.0f};
constexpr peria_color<float> YELLOW {1.000f, 1.000f, 0.000f, 1.0f};
constexpr peria_color<float> YELLOWGREEN {0.604f, 0.804f, 0.196f, 1.0f};
}
