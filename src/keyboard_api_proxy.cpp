#ifdef ITG_REMOTE

#include <fmt/core.h>

#include <Adafruit_TinyUSB.h> // for Serial
#include "bluefruit.h"

#include <bluemicro_hid.h>

#include "keyboard_api_proxy.h"

KeyboardApiProxy keyboard_api;

KeyboardApiProxy::KeyboardApiProxy() {}

void connect_callback(uint16_t conn_handle)
{
    ledOff(PIN_LED);
}
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    ledOn(PIN_LED);
}

void KeyboardApiProxy::begin()
{
    // bluemicro_hid.begin();
    // bluemicro_hid.setHIDMessageDelay(0);
    bluemicro_hid.setBLEManufacturer("BLE_Manufacturer");
    bluemicro_hid.setBLEModel("ITG Wheel Remote");
    bluemicro_hid.setBLETxPower(4);
    bluemicro_hid.setHIDMessageDelay(0);
    bluemicro_hid.setUSBPollInterval(2);
    bluemicro_hid.setUSBStringDescriptor("USB_Descriptor");
    // usb_hid.setReportCallback(NULL, hid_report_callback);
    // blehid.setKeyboardLedCallback(set_keyboard_led);

    // Bluefruit.Periph.setConnInterval
    Bluefruit.Periph.setConnInterval(6, 12);  // 7.5 - 15 ms
    Bluefruit.Periph.setConnSlaveLatency(10); // TODO: add this when 0.22.0 gets
    Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
    Bluefruit.Periph.setConnectCallback(connect_callback);
    Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

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

#define CONNECTION_MODE CONNECTION_MODE_BLE_ONLY

void KeyboardApiProxy::processDirtyKeys(bool flush)
{
    uint32_t t0 = millis();
    if (held_keycodes.empty())
    {
        bluemicro_hid.keyboardRelease();
    }
    while (!bluemicro_hid.isKeyboardQueueEmpty())
    {
        bluemicro_hid.processQueues(CONNECTION_MODE);
    }

    if (!flush && dirty_press.empty() && dirty_release.empty())
    {
        // fmt::print("processDirtyKeys() no-op {} ms\n", millis() - t0);
        return;
    }

    static HIDKeyboard last_report = {0};
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

    if (last_report == report)
    {
        return;
    }
    last_report = report;

    bluemicro_hid.keyboardReport(&report);
    // bluemicro_hid.processQueues(CONNECTION_MODE);
    while (!bluemicro_hid.isKeyboardQueueEmpty())
    {
        bluemicro_hid.processQueues(CONNECTION_MODE);
    }
    dirty_press.clear();
    dirty_release.clear();
#ifdef DEBUG_LOGS
    fmt::print("processDirtyKeys() finished {} ms\n", millis() - t0);
    fmt::print("blehid.isBootMode() {}\n", blehid.isBootMode());
#endif
}

void KeyboardApiProxy::clear()
{
    // make sure not to drop keys
    processDirtyKeys();
    // if (
    //     !held_keycodes.empty() ||
    //     !dirty_press.empty() ||
    //     !dirty_release.empty())
    // {
    //     bluemicro_hid.keyboardRelease();
    //     while (!bluemicro_hid.isKeyboardQueueEmpty())
    //     {
    //         bluemicro_hid.processQueues(CONNECTION_MODE);
    //     }
    // }
    held_keycodes.clear();
    dirty_press.clear();
    dirty_release.clear();
}

// HIDKeyboard KeyboardApiProxy::getKeyboardReport()
// {
//     HIDKeyboard report = HIDKeyboard();
//     return report;
// }

#endif // ITG_REMOTE
