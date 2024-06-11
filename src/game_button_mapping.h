#pragma once
#include "keyboard_api_proxy.h"

enum itg_button_t
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    START,
    SELECT,
    BACK,
    OPERATOR,
    _NUM_ITG_BUTTONS,
};

enum itg_player_t {
    PLAYER_1,
    PLAYER_2,
};
enum itg_menu_or_game {
    MENU,
    GAME,
};

// keycode_t get_itg_keycode_for_button(
//     itg_button_t button, bool player_switch_value, bool menu = true);
keycode_t get_itg_keycode_for_button(
    itg_button_t button, itg_player_t player, itg_menu_or_game menu_or_game = MENU);
