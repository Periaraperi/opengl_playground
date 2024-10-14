#pragma once

#include "peria_types.hpp"

namespace peria::engine {
// the following Peria_Key values are copied from SDL2/SDL_scancode.h.
// I just wanted my own enum with my own names for dependent applications to use them.
enum peria_key {
    PERIA_KEY_UNKNOWN = 0,
    PERIA_KEY_A = 4,
    PERIA_KEY_B = 5,
    PERIA_KEY_C = 6,
    PERIA_KEY_D = 7,
    PERIA_KEY_E = 8,
    PERIA_KEY_F = 9,
    PERIA_KEY_G = 10,
    PERIA_KEY_H = 11,
    PERIA_KEY_I = 12,
    PERIA_KEY_J = 13,
    PERIA_KEY_K = 14,
    PERIA_KEY_L = 15,
    PERIA_KEY_M = 16,
    PERIA_KEY_N = 17,
    PERIA_KEY_O = 18,
    PERIA_KEY_P = 19,
    PERIA_KEY_Q = 20,
    PERIA_KEY_R = 21,
    PERIA_KEY_S = 22,
    PERIA_KEY_T = 23,
    PERIA_KEY_U = 24,
    PERIA_KEY_V = 25,
    PERIA_KEY_W = 26,
    PERIA_KEY_X = 27,
    PERIA_KEY_Y = 28,
    PERIA_KEY_Z = 29,

    PERIA_KEY_1 = 30,
    PERIA_KEY_2 = 31,
    PERIA_KEY_3 = 32,
    PERIA_KEY_4 = 33,
    PERIA_KEY_5 = 34,
    PERIA_KEY_6 = 35,
    PERIA_KEY_7 = 36,
    PERIA_KEY_8 = 37,
    PERIA_KEY_9 = 38,
    PERIA_KEY_0 = 39,

    PERIA_KEY_RETURN = 40,
    PERIA_KEY_ESCAPE = 41,
    PERIA_KEY_BACKSPACE = 42,
    PERIA_KEY_TAB = 43,
    PERIA_KEY_SPACE = 44,

    PERIA_KEY_MINUS = 45,
    PERIA_KEY_EQUALS = 46,
    PERIA_KEY_LEFTBRACKET = 47,
    PERIA_KEY_RIGHTBRACKET = 48,
    PERIA_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    PERIA_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate PERIA_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    PERIA_KEY_SEMICOLON = 51,
    PERIA_KEY_APOSTROPHE = 52,
    PERIA_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    PERIA_KEY_COMMA = 54,
    PERIA_KEY_PERIOD = 55,
    PERIA_KEY_SLASH = 56,

    PERIA_KEY_CAPSLOCK = 57,

    PERIA_KEY_F1 = 58,
    PERIA_KEY_F2 = 59,
    PERIA_KEY_F3 = 60,
    PERIA_KEY_F4 = 61,
    PERIA_KEY_F5 = 62,
    PERIA_KEY_F6 = 63,
    PERIA_KEY_F7 = 64,
    PERIA_KEY_F8 = 65,
    PERIA_KEY_F9 = 66,
    PERIA_KEY_F10 = 67,
    PERIA_KEY_F11 = 68,
    PERIA_KEY_F12 = 69,

    PERIA_KEY_PRINTSCREEN = 70,
    PERIA_KEY_SCROLLLOCK = 71,
    PERIA_KEY_PAUSE = 72,
    PERIA_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    PERIA_KEY_HOME = 74,
    PERIA_KEY_PAGEUP = 75,
    PERIA_KEY_DELETE = 76,
    PERIA_KEY_END = 77,
    PERIA_KEY_PAGEDOWN = 78,
    PERIA_KEY_RIGHT = 79,
    PERIA_KEY_LEFT = 80,
    PERIA_KEY_DOWN = 81,
    PERIA_KEY_UP = 82,

    PERIA_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    PERIA_KEY_KP_DIVIDE = 84,
    PERIA_KEY_KP_MULTIPLY = 85,
    PERIA_KEY_KP_MINUS = 86,
    PERIA_KEY_KP_PLUS = 87,
    PERIA_KEY_KP_ENTER = 88,
    PERIA_KEY_KP_1 = 89,
    PERIA_KEY_KP_2 = 90,
    PERIA_KEY_KP_3 = 91,
    PERIA_KEY_KP_4 = 92,
    PERIA_KEY_KP_5 = 93,
    PERIA_KEY_KP_6 = 94,
    PERIA_KEY_KP_7 = 95,
    PERIA_KEY_KP_8 = 96,
    PERIA_KEY_KP_9 = 97,
    PERIA_KEY_KP_0 = 98,
    PERIA_KEY_KP_PERIOD = 99,

    PERIA_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    PERIA_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
    PERIA_KEY_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    PERIA_KEY_KP_EQUALS = 103,
    PERIA_KEY_F13 = 104,
    PERIA_KEY_F14 = 105,
    PERIA_KEY_F15 = 106,
    PERIA_KEY_F16 = 107,
    PERIA_KEY_F17 = 108,
    PERIA_KEY_F18 = 109,
    PERIA_KEY_F19 = 110,
    PERIA_KEY_F20 = 111,
    PERIA_KEY_F21 = 112,
    PERIA_KEY_F22 = 113,
    PERIA_KEY_F23 = 114,
    PERIA_KEY_F24 = 115,
    PERIA_KEY_EXECUTE = 116,
    PERIA_KEY_HELP = 117,    /**< AL Integrated Help Center */
    PERIA_KEY_MENU = 118,    /**< Menu (show menu) */
    PERIA_KEY_SELECT = 119,
    PERIA_KEY_STOP = 120,    /**< AC Stop */
    PERIA_KEY_AGAIN = 121,   /**< AC Redo/Repeat */
    PERIA_KEY_UNDO = 122,    /**< AC Undo */
    PERIA_KEY_CUT = 123,     /**< AC Cut */
    PERIA_KEY_COPY = 124,    /**< AC Copy */
    PERIA_KEY_PASTE = 125,   /**< AC Paste */
    PERIA_KEY_FIND = 126,    /**< AC Find */
    PERIA_KEY_MUTE = 127,
    PERIA_KEY_VOLUMEUP = 128,
    PERIA_KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     SDL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     SDL_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     SDL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    PERIA_KEY_KP_COMMA = 133,
    PERIA_KEY_KP_EQUALSAS400 = 134,

    PERIA_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    PERIA_KEY_INTERNATIONAL2 = 136,
    PERIA_KEY_INTERNATIONAL3 = 137, /**< Yen */
    PERIA_KEY_INTERNATIONAL4 = 138,
    PERIA_KEY_INTERNATIONAL5 = 139,
    PERIA_KEY_INTERNATIONAL6 = 140,
    PERIA_KEY_INTERNATIONAL7 = 141,
    PERIA_KEY_INTERNATIONAL8 = 142,
    PERIA_KEY_INTERNATIONAL9 = 143,
    PERIA_KEY_LANG1 = 144, /**< Hangul/English toggle */
    PERIA_KEY_LANG2 = 145, /**< Hanja conversion */
    PERIA_KEY_LANG3 = 146, /**< Katakana */
    PERIA_KEY_LANG4 = 147, /**< Hiragana */
    PERIA_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    PERIA_KEY_LANG6 = 149, /**< reserved */
    PERIA_KEY_LANG7 = 150, /**< reserved */
    PERIA_KEY_LANG8 = 151, /**< reserved */
    PERIA_KEY_LANG9 = 152, /**< reserved */

    PERIA_KEY_ALTERASE = 153,    /**< Erase-Eaze */
    PERIA_KEY_SYSREQ = 154,
    PERIA_KEY_CANCEL = 155,      /**< AC Cancel */
    PERIA_KEY_CLEAR = 156,
    PERIA_KEY_PRIOR = 157,
    PERIA_KEY_RETURN2 = 158,
    PERIA_KEY_SEPARATOR = 159,
    PERIA_KEY_OUT = 160,
    PERIA_KEY_OPER = 161,
    PERIA_KEY_CLEARAGAIN = 162,
    PERIA_KEY_CRSEL = 163,
    PERIA_KEY_EXSEL = 164,

    PERIA_KEY_KP_00 = 176,
    PERIA_KEY_KP_000 = 177,
    PERIA_KEY_THOUSANDSSEPARATOR = 178,
    PERIA_KEY_DECIMALSEPARATOR = 179,
    PERIA_KEY_CURRENCYUNIT = 180,
    PERIA_KEY_CURRENCYSUBUNIT = 181,
    PERIA_KEY_KP_LEFTPAREN = 182,
    PERIA_KEY_KP_RIGHTPAREN = 183,
    PERIA_KEY_KP_LEFTBRACE = 184,
    PERIA_KEY_KP_RIGHTBRACE = 185,
    PERIA_KEY_KP_TAB = 186,
    PERIA_KEY_KP_BACKSPACE = 187,
    PERIA_KEY_KP_A = 188,
    PERIA_KEY_KP_B = 189,
    PERIA_KEY_KP_C = 190,
    PERIA_KEY_KP_D = 191,
    PERIA_KEY_KP_E = 192,
    PERIA_KEY_KP_F = 193,
    PERIA_KEY_KP_XOR = 194,
    PERIA_KEY_KP_POWER = 195,
    PERIA_KEY_KP_PERCENT = 196,
    PERIA_KEY_KP_LESS = 197,
    PERIA_KEY_KP_GREATER = 198,
    PERIA_KEY_KP_AMPERSAND = 199,
    PERIA_KEY_KP_DBLAMPERSAND = 200,
    PERIA_KEY_KP_VERTICALBAR = 201,
    PERIA_KEY_KP_DBLVERTICALBAR = 202,
    PERIA_KEY_KP_COLON = 203,
    PERIA_KEY_KP_HASH = 204,
    PERIA_KEY_KP_SPACE = 205,
    PERIA_KEY_KP_AT = 206,
    PERIA_KEY_KP_EXCLAM = 207,
    PERIA_KEY_KP_MEMSTORE = 208,
    PERIA_KEY_KP_MEMRECALL = 209,
    PERIA_KEY_KP_MEMCLEAR = 210,
    PERIA_KEY_KP_MEMADD = 211,
    PERIA_KEY_KP_MEMSUBTRACT = 212,
    PERIA_KEY_KP_MEMMULTIPLY = 213,
    PERIA_KEY_KP_MEMDIVIDE = 214,
    PERIA_KEY_KP_PLUSMINUS = 215,
    PERIA_KEY_KP_CLEAR = 216,
    PERIA_KEY_KP_CLEARENTRY = 217,
    PERIA_KEY_KP_BINARY = 218,
    PERIA_KEY_KP_OCTAL = 219,
    PERIA_KEY_KP_DECIMAL = 220,
    PERIA_KEY_KP_HEXADECIMAL = 221,

    PERIA_KEY_LCTRL = 224,
    PERIA_KEY_LSHIFT = 225,
    PERIA_KEY_LALT = 226, /**< alt, option */
    PERIA_KEY_LGUI = 227, /**< windows, command (apple), meta */
    PERIA_KEY_RCTRL = 228,
    PERIA_KEY_RSHIFT = 229,
    PERIA_KEY_RALT = 230, /**< alt gr, option */
    PERIA_KEY_RGUI = 231, /**< windows, command (apple), meta */

    PERIA_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *  See https://usb.org/sites/default/files/hut1_2.pdf
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    PERIA_KEY_AUDIONEXT = 258,
    PERIA_KEY_AUDIOPREV = 259,
    PERIA_KEY_AUDIOSTOP = 260,
    PERIA_KEY_AUDIOPLAY = 261,
    PERIA_KEY_AUDIOMUTE = 262,
    PERIA_KEY_MEDIASELECT = 263,
    PERIA_KEY_WWW = 264,             /**< AL Internet Browser */
    PERIA_KEY_MAIL = 265,
    PERIA_KEY_CALCULATOR = 266,      /**< AL Calculator */
    PERIA_KEY_COMPUTER = 267,
    PERIA_KEY_AC_SEARCH = 268,       /**< AC Search */
    PERIA_KEY_AC_HOME = 269,         /**< AC Home */
    PERIA_KEY_AC_BACK = 270,         /**< AC Back */
    PERIA_KEY_AC_FORWARD = 271,      /**< AC Forward */
    PERIA_KEY_AC_STOP = 272,         /**< AC Stop */
    PERIA_KEY_AC_REFRESH = 273,      /**< AC Refresh */
    PERIA_KEY_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    PERIA_KEY_BRIGHTNESSDOWN = 275,
    PERIA_KEY_BRIGHTNESSUP = 276,
    PERIA_KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    PERIA_KEY_KBDILLUMTOGGLE = 278,
    PERIA_KEY_KBDILLUMDOWN = 279,
    PERIA_KEY_KBDILLUMUP = 280,
    PERIA_KEY_EJECT = 281,
    PERIA_KEY_SLEEP = 282,           /**< SC System Sleep */

    PERIA_KEY_APP1 = 283,
    PERIA_KEY_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    PERIA_KEY_AUDIOREWIND = 285,
    PERIA_KEY_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    PERIA_KEY_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    PERIA_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    PERIA_KEY_CALL = 289, /**< Used for accepting phone calls. */
    PERIA_KEY_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    PERIA_NUM_CODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

enum class mouse_button {
    LEFT,
    MID,
    RIGHT
};

struct mouse {
    i32 x;
    i32 y;
};
}
