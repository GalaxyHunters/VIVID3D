#ifndef VIVID_COLORDEFS_H
#define VIVID_COLORDEFS_H

#include <functional>
#include <map>
#include "NormalFloat.h"

using namespace std;

namespace vivid {
    typedef unsigned char rgb_t;
    typedef std::array<rgb_t, 3> color_t;

    constexpr rgb_t RGB_MAX = 255;

    inline color_t ToColor_T(const array<float, 3>& arNormalRGB) {
        return {(rgb_t) (Normalize(arNormalRGB[0]) * RGB_MAX), (rgb_t) (Normalize(arNormalRGB[1]) * RGB_MAX), (rgb_t) (Normalize(arNormalRGB[2]) * RGB_MAX) };
    }

    inline array<float, 3> ToNormalRGB(const color_t & arRGB) {
        return {(float)arRGB[0] / RGB_MAX, (float)arRGB[1] / RGB_MAX, (float)arRGB[2] / RGB_MAX};
    }

    constexpr color_t INDIANRED = {205, 92, 92};
    constexpr color_t LIGHTCORAL = {240, 128, 128};
    constexpr color_t SALMON = {250, 128, 114};
    constexpr color_t DARKSALMON = {233, 150, 122};
    constexpr color_t LIGHTSALMON = {255, 160, 122};
    constexpr color_t CRIMSON = {220, 20, 60};
    constexpr color_t RED = {255, 0, 0};
    constexpr color_t DARKRED = {139, 0, 0};
    constexpr color_t PINK = {255, 192, 203};
    constexpr color_t LIGHTPINK = {255, 182, 193};
    constexpr color_t HOTPINK = {255, 105, 180};
    constexpr color_t DEEPPINK = {255, 20, 147};
    constexpr color_t MEDIUMVIOLETRED = {199, 21, 133};
    constexpr color_t PALEVIOLETRED = {219, 112, 147};
    constexpr color_t CORAL = {255, 127, 80};
    constexpr color_t TOMATO = {255, 99, 71};
    constexpr color_t ORANGERED = {255, 69, 0};
    constexpr color_t DARKORANGE = {255, 140, 0};
    constexpr color_t ORANGE = {255, 165, 0};
    constexpr color_t GOLD = {255, 215, 0};
    constexpr color_t YELLOW = {255, 255, 0};
    constexpr color_t LIGHTYELLOW = {255, 255, 224};
    constexpr color_t LEMONCHIFFON = {255, 250, 205};
    constexpr color_t LIGHTGOLDENRODYELLOW = {250, 250, 210};
    constexpr color_t PAPAYAWHIP = {255, 239, 213};
    constexpr color_t MOCCASIN = {255, 228, 181};
    constexpr color_t PEACHPUFF = {255, 218, 185};
    constexpr color_t PALEGOLDENROD = {238, 232, 170};
    constexpr color_t KHAKI = {240, 230, 140};
    constexpr color_t DARKKHAKI = {189, 183, 107};
    constexpr color_t LAVENDER = {230, 230, 250};
    constexpr color_t THISTLE = {216, 191, 216};
    constexpr color_t PLUM = {221, 160, 221};
    constexpr color_t VIOLET = {238, 130, 238};
    constexpr color_t ORCHID = {218, 112, 214};
    constexpr color_t FUCHSIA = {255, 0, 255};
    constexpr color_t MAGENTA = {255, 0, 255};
    constexpr color_t MEDIUMORCHID = {186, 85, 211};
    constexpr color_t MEDIUMPURPLE = {147, 112, 219};
    constexpr color_t REBECCAPURPLE = {102, 51, 153};
    constexpr color_t BLUEVIOLET = {138, 43, 226};
    constexpr color_t DARKVIOLET = {148, 0, 211};
    constexpr color_t DARKORCHID = {153, 50, 204};
    constexpr color_t DARKMAGENTA = {139, 0, 139};
    constexpr color_t PURPLE = {128, 0, 128};
    constexpr color_t INDIGO = {75, 0, 130};
    constexpr color_t SLATEBLUE = {106, 90, 205};
    constexpr color_t DARKSLATEBLUE = {72, 61, 139};
    constexpr color_t MEDIUMSLATEBLUE = {123, 104, 238};
    constexpr color_t GREENYELLOW = {173, 255, 47};
    constexpr color_t CHARTREUSE = {127, 255, 0};
    constexpr color_t LAWNGREEN = {124, 252, 0};
    constexpr color_t LIME = {0, 255, 0};
    constexpr color_t LIMEGREEN = {50, 205, 50};
    constexpr color_t PALEGREEN = {152, 251, 152};
    constexpr color_t LIGHTGREEN = {144, 238, 144};
    constexpr color_t MEDIUMSPRINGGREEN = {0, 250, 154};
    constexpr color_t SPRINGGREEN = {0, 255, 127};
    constexpr color_t MEDIUMSEAGREEN = {60, 179, 113};
    constexpr color_t SEAGREEN = {46, 139, 87};
    constexpr color_t FORESTGREEN = {34, 139, 34};
    constexpr color_t GREEN = {0, 128, 0};
    constexpr color_t DARKGREEN = {0, 100, 0};
    constexpr color_t YELLOWGREEN = {154, 205, 50};
    constexpr color_t OLIVEDRAB = {107, 142, 35};
    constexpr color_t OLIVE = {128, 128, 0};
    constexpr color_t DARKOLIVEGREEN = {85, 107, 47};
    constexpr color_t MEDIUMAQUAMARINE = {102, 205, 170};
    constexpr color_t DARKSEAGREEN = {143, 188, 139};
    constexpr color_t LIGHTSEAGREEN = {32, 178, 170};
    constexpr color_t DARKCYAN = {0, 139, 139};
    constexpr color_t TEAL = {0, 128, 128};
    constexpr color_t AQUA = {0, 255, 255};
    constexpr color_t CYAN = {0, 255, 255};
    constexpr color_t LIGHTCYAN = {224, 255, 255};
    constexpr color_t PALETURQUOISE = {175, 238, 238};
    constexpr color_t AQUAMARINE = {127, 255, 212};
    constexpr color_t TURQUOISE = {64, 224, 208};
    constexpr color_t MEDIUMTURQUOISE = {72, 209, 204};
    constexpr color_t DARKTURQUOISE = {0, 206, 209};
    constexpr color_t CADETBLUE = {95, 158, 160};
    constexpr color_t STEELBLUE = {70, 130, 180};
    constexpr color_t LIGHTSTEELBLUE = {176, 196, 222};
    constexpr color_t POWDERBLUE = {176, 224, 230};
    constexpr color_t LIGHTBLUE = {173, 216, 230};
    constexpr color_t SKYBLUE = {135, 206, 235};
    constexpr color_t LIGHTSKYBLUE = {135, 206, 250};
    constexpr color_t DEEPSKYBLUE = {0, 191, 255};
    constexpr color_t DODGERBLUE = {30, 144, 255};
    constexpr color_t CORNFLOWERBLUE = {100, 149, 237};
    constexpr color_t ROYALBLUE = {65, 105, 225};
    constexpr color_t BLUE = {0, 0, 255};
    constexpr color_t MEDIUMBLUE = {0, 0, 205};
    constexpr color_t DARKBLUE = {0, 0, 139};
    constexpr color_t NAVY = {0, 0, 128};
    constexpr color_t MIDNIGHTBLUE = {25, 25, 112};
    constexpr color_t CORNSILK = {255, 248, 220};
    constexpr color_t BLANCHEDALMOND = {255, 235, 205};
    constexpr color_t BISQUE = {255, 228, 196};
    constexpr color_t NAVAJOWHITE = {255, 222, 173};
    constexpr color_t WHEAT = {245, 222, 179};
    constexpr color_t BURLYWOOD = {222, 184, 135};
    constexpr color_t TAN = {210, 180, 140};
    constexpr color_t ROSYBROWN = {188, 143, 143};
    constexpr color_t SANDYBROWN = {244, 164, 96};
    constexpr color_t GOLDENROD = {218, 165, 32};
    constexpr color_t DARKGOLDENROD = {184, 134, 11};
    constexpr color_t PERU = {205, 133, 63};
    constexpr color_t CHOCOLATE = {210, 105, 30};
    constexpr color_t SADDLEBROWN = {139, 69, 19};
    constexpr color_t SIENNA = {160, 82, 45};
    constexpr color_t BROWN = {165, 42, 42};
    constexpr color_t MAROON = {128, 0, 0};
    constexpr color_t WHITE = {255, 255, 255};
    constexpr color_t SNOW = {255, 250, 250};
    constexpr color_t HONEYDEW = {240, 255, 240};
    constexpr color_t MINTCREAM = {245, 255, 250};
    constexpr color_t AZURE = {240, 255, 255};
    constexpr color_t ALICEBLUE = {240, 248, 255};
    constexpr color_t GHOSTWHITE = {248, 248, 255};
    constexpr color_t WHITESMOKE = {245, 245, 245};
    constexpr color_t SEASHELL = {255, 245, 238};
    constexpr color_t BEIGE = {245, 245, 220};
    constexpr color_t OLDLACE = {253, 245, 230};
    constexpr color_t FLORALWHITE = {253, 245, 230};
    constexpr color_t IVORY = {255, 255, 240};
    constexpr color_t ANTIQUEWHITE = {250, 235, 215};
    constexpr color_t LINEN = {250, 240, 230};
    constexpr color_t LAVENDERBLUSH = {255, 240, 245};
    constexpr color_t MISTYROSE = {255, 228, 225};
    constexpr color_t GAINSBORO = {220, 220, 220};
    constexpr color_t LIGHTGRAY = {211, 211, 211};
    constexpr color_t SILVER = {192, 192, 192};
    constexpr color_t DARKGRAY = {169, 169, 169};
    constexpr color_t GRAY = {128, 128, 128};
    constexpr color_t DIMGRAY = {105, 105, 105};
    constexpr color_t LIGHTSLATEGRAY = {119, 136, 153};
    constexpr color_t SLATEGRAY = {112, 128, 144};
    constexpr color_t DARKSLATEGRAY = {47, 79, 79};
    constexpr color_t BLACK = {0, 0, 0};

    extern const std::map<std::string, color_t> COLORS;

}//namespace vivid
#endif //VIVID_COLORDEFS_H