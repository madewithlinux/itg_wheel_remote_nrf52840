#pragma once
#include <bluemicro_hid.h>

#include "game_button_mapping.h"

typedef uint16_t keymap_entry_t;

// clang-format off
// #define KC_TRANSPARENT (0x01 << 8)
// #define KC_NO          (0x00 << 8)
// #define KC_TRNS        KC_TRANSPARENT
// #define _______        KC_TRANSPARENT
// #define XXXXXXX        KC_NO

#define INDEX_ENCODER_MIDDLE 12
#define INDEX_ENCODER_CCW    13
#define INDEX_ENCODER_CW     14
#define LAYOUT_SIZE          15

#define LAYOUT(          \
    encoder_ccw,         \
    encoder_cw,          \
          up,            \
    left, middle, right, \
          down,          \
    F00, F01,            \
    F10, F11,            \
    F20, F21,            \
    F30, F31             \
)                        \
    {                    \
        up,    F00, F01, \
        right, F10, F11, \
        down,  F20, F21, \
        left,  F30, F31, \
        [INDEX_ENCODER_MIDDLE] = middle,          \
        [INDEX_ENCODER_CCW]    = encoder_ccw,     \
        [INDEX_ENCODER_CW]     = encoder_cw,      \
    }
// clang-format on

enum layer_names_t
{
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NUM_LAYERS
};

#define LAYOUT_SIZE 15
extern const uint16_t keymap_layers[_NUM_LAYERS][LAYOUT_SIZE];

// uint8_t get_active_layer_mask(const trigger_keys_t &activeKeys);

void process_triggered_keys(
    const trigger_keys_t &activeKeys,
    const bool player_switch_value,
    const int encoder_delta);
