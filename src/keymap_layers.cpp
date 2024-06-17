#include "keyboard_api_proxy.h"
#include "keymap_layers.h"
#include <cassert>

// clang-format off
#define RESERVED_ITG    KC_RESERVED_A5
#define RESERVED_LAYER  KC_RESERVED_A6
#define RESERVED_MACRO  KC_RESERVED_A7
// #define KC_TRANSPARENT  (0x01 << 8)
// #define KC_NO           (0x00 << 8)
// #define KC_TRNS         KC_TRANSPARENT
// #define _______         KC_TRANSPARENT
// #define XXXXXXX         KC_NO
// #define ITG(x) (((uint16_t)x) << 8 | RESERVED_ITG)

#define KE(extra, keycode) (((uint16_t)extra) << 8 | keycode)

// clang-format on

keymap_entry_t ITG(itg_button_t b) { return KE(b, RESERVED_ITG); }
keymap_entry_t L(layer_names_t l) { return KE(l, RESERVED_LAYER); }
keycode_t keymap_entry_keycode(keymap_entry_t ke) { return static_cast<uint8_t>(ke & 0x00FF); }
uint8_t keymap_entry_extra(keymap_entry_t ke) { return static_cast<uint8_t>((ke & 0xFF00) >> 8); }

enum macros_t
{
    _close_folder,
    _open_menu,
    _favorite,
    _profile,
    _alt_f4,
    _qk_boot,
    _ir_soundbar_vol_down,
    _ir_soundbar_vol_up,
    _ir_soundbar_toggle_mute,
    _kb_batt,
};
const keymap_entry_t CLOSE_FOLDER = KE(_close_folder, RESERVED_MACRO);
const keymap_entry_t OPEN_MENU = KE(_open_menu, RESERVED_MACRO);
const keymap_entry_t FAVORITE = KE(_favorite, RESERVED_MACRO);
const keymap_entry_t PROFILE = KE(_profile, RESERVED_MACRO);
const keymap_entry_t ALT_F4 = KE(_alt_f4, RESERVED_MACRO);
const keymap_entry_t QK_BOOT = KE(_qk_boot, RESERVED_MACRO);
const keymap_entry_t IR_SOUNDBAR_VOL_DOWN = KE(_ir_soundbar_vol_down, RESERVED_MACRO);
const keymap_entry_t IR_SOUNDBAR_VOL_UP = KE(_ir_soundbar_vol_up, RESERVED_MACRO);
const keymap_entry_t IR_SOUNDBAR_TOGGLE_MUTE = KE(_ir_soundbar_toggle_mute, RESERVED_MACRO);
const keymap_entry_t KB_BATT = KE(_kb_batt, RESERVED_MACRO);

const uint16_t keymap_layers[_NUM_LAYERS][LAYOUT_SIZE] = {
    // clang-format off
    [_BASE] = LAYOUT(
            ITG(LEFT), ITG(RIGHT),
                    ITG(UP)    ,
        ITG(LEFT),  ITG(START) , ITG(RIGHT),
                    ITG(DOWN)  ,

        CLOSE_FOLDER  , OPEN_MENU,
        ITG(SELECT)   , FAVORITE,
        ITG(BACK)     , PROFILE,
        L(_LOWER)     , L(_RAISE)
    ),
    [_LOWER] = LAYOUT(
        IR_SOUNDBAR_VOL_DOWN, IR_SOUNDBAR_VOL_UP,
                 _______,
        _______, IR_SOUNDBAR_TOGGLE_MUTE, _______,
                 _______,

        QK_BOOT, ALT_F4  ,
        KB_BATT, OPERATOR,
        _______, _______ ,
        L(_LOWER)     , L(_RAISE)
    ),
    [_RAISE] = LAYOUT(
        _______, _______,
                 _______,
        _______, _______, _______,
                 _______,

        _______, _______,
        _______, _______,
        _______, _______,
        L(_LOWER)     , L(_RAISE)
    ),
    [_ADJUST] = LAYOUT(
        _______, _______,
                 _______,
        _______, _______, _______,
                 _______,

        _______, _______,
        _______, _______,
        _______, _______,
        L(_LOWER)     , L(_RAISE)
    ),
    // clang-format on
};

layer_names_t find_active_layer(
    const trigger_keys_t &activeKeys)
{
    bool lower = false;
    bool raise = false;
    for (auto key_idx : activeKeys)
    {
        keymap_entry_t ke = keymap_layers[_BASE][key_idx];
        if (keymap_entry_keycode(ke) == RESERVED_LAYER)
        {
            uint8_t extra = keymap_entry_extra(ke);
            switch (extra)
            {
            case _LOWER:
                lower = true;
                break;
            case _RAISE:
                raise = true;
                break;
            case _ADJUST:
                return _ADJUST;
            }
        }
    }
    if (lower && raise)
    {
        return _ADJUST;
    }
    else if (lower)
    {
        return _LOWER;
    }
    else if (raise)
    {
        return _RAISE;
    }
    return _BASE;
}

void handle_macro_press(macros_t macro, itg_player_t current_player)
{
    switch (macro)
    {
    case _close_folder:
        keyboard_api.press(get_itg_keycode_for_button(UP, current_player, GAME));
        keyboard_api.press(get_itg_keycode_for_button(DOWN, current_player, GAME));
        break;
    case _open_menu:
        keyboard_api.press(get_itg_keycode_for_button(LEFT, current_player, MENU));
        keyboard_api.press(get_itg_keycode_for_button(RIGHT, current_player, MENU));
        break;
    case _favorite:
        keyboard_api.press(current_player == PLAYER_1 ? KC_I : KC_O);
        break;
    case _profile:
        keyboard_api.press(KC_P);
        break;
    case _alt_f4:
        keyboard_api.press(KC_LALT);
        keyboard_api.press(KC_F4);
        break;
    case _qk_boot:
        Serial.println("will now reboot to UF2 bootloader");
        enum
        {
            DFU_MAGIC_UF2_RESET = 0x57
        };
        sd_power_gpregret_set(0, DFU_MAGIC_UF2_RESET);
        NVIC_SystemReset();
        break;
    case _ir_soundbar_vol_down:
        break;
    case _ir_soundbar_vol_up:
        break;
    case _ir_soundbar_toggle_mute:
        break;
    case _kb_batt:
        break;
    default:
        break;
    }
}

void process_triggered_keys(
    const trigger_keys_t &activeKeys,
    const bool player_switch_value,
    const int encoder_delta)
{
    itg_player_t current_player = player_switch_value ? PLAYER_1 : PLAYER_2;

    keyboard_api.clear();
    layer_names_t active_layer = find_active_layer(activeKeys);
    for (auto key_idx : activeKeys)
    {
        keymap_entry_t ke = keymap_layers[active_layer][key_idx];
        keycode_t keycode = keymap_entry_keycode(ke);
        uint8_t extra = keymap_entry_extra(ke);
        switch (keycode)
        {
        case KC_NO:
            // do nothing
            break;
        case RESERVED_LAYER:
            // handled earlier, nothing to do here
            break;
        case KC_TRANSPARENT:
            // TODO lookup lower layer
            break;
        case RESERVED_ITG:
            keyboard_api.press(
                get_itg_keycode_for_button(
                    (itg_button_t)extra,
                    current_player));
            break;
        case RESERVED_MACRO:
            handle_macro_press((macros_t)extra, current_player);
            break;
        default:
            keyboard_api.press(keycode);
            break;
        }
    }

    if (encoder_delta != 0)
    {
        size_t key_idx = (encoder_delta > 0) ? INDEX_ENCODER_CW : INDEX_ENCODER_CCW;
        keymap_entry_t ke = keymap_layers[active_layer][key_idx];
        keycode_t keycode = keymap_entry_keycode(ke);
        uint8_t extra = keymap_entry_extra(ke);
        for (int i = 0; i < abs(encoder_delta); i++)
        {
            switch (keycode)
            {
            case KC_NO:
                // do nothing
                break;
            case RESERVED_LAYER:
                // handled earlier, nothing to do here
                break;
            case KC_TRANSPARENT:
                // TODO lookup lower layer
                break;
            case RESERVED_ITG:
                keyboard_api.tap(
                    get_itg_keycode_for_button(
                        (itg_button_t)extra,
                        current_player));
                break;
            case RESERVED_MACRO:
                // TODO: tap instead of press?
                handle_macro_press((macros_t)extra, current_player);
                break;
            default:
                keyboard_api.tap(keycode);
                break;
            }
        }
    }
}
