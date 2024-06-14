#pragma once
// #include <bluemicro_hid.h>
#include <unordered_set>

typedef uint8_t keycode_t;

class KeyboardApiProxy
{
public:
    KeyboardApiProxy();

    void begin();
    void press(keycode_t keycode);
    void release(keycode_t keycode);
    void tap(keycode_t keycode);
    void processDirtyKeys(bool flush = false);
    void clear();

    // uint32_t tap_code_delay = 1;

private:
    // HIDKeyboard currentReport;
    std::unordered_set<keycode_t> held_keycodes;
    std::unordered_set<keycode_t> dirty_press;
    std::unordered_set<keycode_t> dirty_release;
    // bool dirty = false;
    // HIDKeyboard getKeyboardReport();
};

extern KeyboardApiProxy keyboard_api;
