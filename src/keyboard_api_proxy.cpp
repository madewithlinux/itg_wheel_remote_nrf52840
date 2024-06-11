#include <Adafruit_TinyUSB.h> // for Serial
#include "bluefruit.h"

#include <bluemicro_hid.h>

#include "keyboard_api_proxy.h"

KeyboardApiProxy keyboard_api;

KeyboardApiProxy::KeyboardApiProxy() {}

void KeyboardApiProxy::begin()
{
    bluemicro_hid.begin();
    bluemicro_hid.setHIDMessageDelay(0);
}

void KeyboardApiProxy::press(keycode_t keycode)
{
    if (dirty_release.count(keycode) > 0)
    {
        processDirtyKeys();
    }
    // held_keycodes.insert(keycode);
    if (held_keycodes.insert(keycode).second)
    {
        dirty_press.insert(keycode);
    }
}

void KeyboardApiProxy::release(keycode_t keycode)
{
    if (dirty_press.count(keycode) > 0)
    {
        processDirtyKeys();
    }
    // held_keycodes.erase(keycode);
    if (held_keycodes.erase(keycode) == 1)
    {
        dirty_release.insert(keycode);
    }
}

void KeyboardApiProxy::tap(keycode_t keycode)
{
    if (held_keycodes.count(keycode) > 0)
    {
        // cannot tap a key while it is also held
        return;
    }
    press(keycode);
    release(keycode);
}

void KeyboardApiProxy::processDirtyKeys()
{
    while (!bluemicro_hid.isKeyboardQueueEmpty())
    {
        bluemicro_hid.processQueues(CONNECTION_MODE_AUTO);
    }

    if (dirty_press.empty() && dirty_release.empty())
    {
        return;
    }

    HIDKeyboard report = {0};
    size_t idx = 0;
    for (auto keycode : this->held_keycodes)
    {
        switch (keycode)
        {
        case KC_LCTRL:
            report.modifier |= BIT_LCTRL;
            break;
        case KC_LSHIFT:
            report.modifier |= BIT_LSHIFT;
            break;
        case KC_LALT:
            report.modifier |= BIT_LALT;
            break;
        case KC_LGUI:
            report.modifier |= BIT_LGUI;
            break;
        case KC_RCTRL:
            report.modifier |= BIT_RCTRL;
            break;
        case KC_RSHIFT:
            report.modifier |= BIT_RSHIFT;
            break;
        case KC_RALT:
            report.modifier |= BIT_RALT;
            break;
        case KC_RGUI:
            report.modifier |= BIT_RGUI;
            break;
        default:
            report.keycode[idx] = keycode;
            idx++;
            break;
        }
    }

    bluemicro_hid.keyboardReport(&report);
    // bluemicro_hid.processQueues(CONNECTION_MODE_AUTO);
    while (!bluemicro_hid.isKeyboardQueueEmpty())
    {
        bluemicro_hid.processQueues(CONNECTION_MODE_AUTO);
    }
    dirty_press.clear();
    dirty_release.clear();
}

// HIDKeyboard KeyboardApiProxy::getKeyboardReport()
// {
//     HIDKeyboard report = HIDKeyboard();
//     return report;
// }
