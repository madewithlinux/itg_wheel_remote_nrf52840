// #include <bluemicro_hid.h>
#include <hid_keycodes.h>
#include "game_button_mapping.h"

// clang-format off
// // default P1 mappings
// #define ITG_KEYCODE_P1_MENU_LEFT    KC_DELETE
// #define ITG_KEYCODE_P1_MENU_RIGHT   KC_PAGE_DOWN
// #define ITG_KEYCODE_P1_MENU_UP      KC_HOME
// #define ITG_KEYCODE_P1_MENU_DOWN    KC_END
// #define ITG_KEYCODE_P1_LEFT         KC_LEFT_ARROW
// #define ITG_KEYCODE_P1_RIGHT        KC_RIGHT_ARROW
// #define ITG_KEYCODE_P1_UP           KC_UP_ARROW
// #define ITG_KEYCODE_P1_DOWN         KC_DOWN_ARROW

// my mappings. note: these are intentionally swapped for P1!
#define ITG_KEYCODE_P1_MENU_LEFT    KC_LEFT
#define ITG_KEYCODE_P1_MENU_RIGHT   KC_RIGHT
#define ITG_KEYCODE_P1_MENU_UP      KC_UP
#define ITG_KEYCODE_P1_MENU_DOWN    KC_DOWN
#define ITG_KEYCODE_P1_LEFT         KC_DELETE
#define ITG_KEYCODE_P1_RIGHT        KC_PGDOWN
#define ITG_KEYCODE_P1_UP           KC_HOME
#define ITG_KEYCODE_P1_DOWN         KC_END

#define ITG_KEYCODE_P1_START        KC_ENTER
#define ITG_KEYCODE_P1_SELECT       KC_SLASH
#define ITG_KEYCODE_P1_BACK         KC_ESCAPE
//
#define ITG_KEYCODE_P2_MENU_LEFT    KC_KP_SLASH
#define ITG_KEYCODE_P2_MENU_RIGHT   KC_KP_ASTERISK
#define ITG_KEYCODE_P2_MENU_UP      KC_KP_MINUS
#define ITG_KEYCODE_P2_MENU_DOWN    KC_KP_PLUS
#define ITG_KEYCODE_P2_LEFT         KC_KP_4
#define ITG_KEYCODE_P2_RIGHT        KC_KP_6
#define ITG_KEYCODE_P2_UP           KC_KP_8
#define ITG_KEYCODE_P2_DOWN         KC_KP_2
#define ITG_KEYCODE_P2_START        KC_KP_ENTER
#define ITG_KEYCODE_P2_SELECT       KC_KP_0
#define ITG_KEYCODE_P2_BACK         KC_BSLASH
//
#define ITG_KEYCODE_OPERATOR        KC_SCROLLLOCK
// #define ITG_KEYCODE_COIN            KC_F1
// clang-format on

// clang-format off
const keycode_t itg_button_keycodes[2][2][_NUM_ITG_BUTTONS] = {
    [PLAYER_1] = {
        [MENU] = {
            [LEFT]     = ITG_KEYCODE_P1_MENU_LEFT,
            [RIGHT]    = ITG_KEYCODE_P1_MENU_RIGHT,
            [UP]       = ITG_KEYCODE_P1_MENU_UP,
            [DOWN]     = ITG_KEYCODE_P1_MENU_DOWN,
            [START]    = ITG_KEYCODE_P1_START,
            [SELECT]   = ITG_KEYCODE_P1_SELECT,
            [BACK]     = ITG_KEYCODE_P1_BACK,
            [OPERATOR] = ITG_KEYCODE_OPERATOR,
        },
        [GAME] = {
            [LEFT]     = ITG_KEYCODE_P1_LEFT,
            [RIGHT]    = ITG_KEYCODE_P1_RIGHT,
            [UP]       = ITG_KEYCODE_P1_UP,
            [DOWN]     = ITG_KEYCODE_P1_DOWN,
            [START]    = ITG_KEYCODE_P1_START,
            [SELECT]   = ITG_KEYCODE_P1_SELECT,
            [BACK]     = ITG_KEYCODE_P1_BACK,
            [OPERATOR] = ITG_KEYCODE_OPERATOR,
        }
    },
    [PLAYER_2] = {
        [MENU] = {
            [LEFT]     = ITG_KEYCODE_P2_MENU_LEFT,
            [RIGHT]    = ITG_KEYCODE_P2_MENU_RIGHT,
            [UP]       = ITG_KEYCODE_P2_MENU_UP,
            [DOWN]     = ITG_KEYCODE_P2_MENU_DOWN,
            [START]    = ITG_KEYCODE_P2_START,
            [SELECT]   = ITG_KEYCODE_P2_SELECT,
            [BACK]     = ITG_KEYCODE_P2_BACK,
            [OPERATOR] = ITG_KEYCODE_OPERATOR,
        },
        [GAME] = {
            [LEFT]     = ITG_KEYCODE_P2_LEFT,
            [RIGHT]    = ITG_KEYCODE_P2_RIGHT,
            [UP]       = ITG_KEYCODE_P2_UP,
            [DOWN]     = ITG_KEYCODE_P2_DOWN,
            [START]    = ITG_KEYCODE_P2_START,
            [SELECT]   = ITG_KEYCODE_P2_SELECT,
            [BACK]     = ITG_KEYCODE_P2_BACK,
            [OPERATOR] = ITG_KEYCODE_OPERATOR,
        }
    },
};
// clang-format on

keycode_t get_itg_keycode_for_button(
    itg_button_t button,
    itg_player_t player,
    itg_menu_or_game menu_or_game)
{
    return itg_button_keycodes[player][menu_or_game][button];
}
