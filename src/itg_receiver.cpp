#ifdef ITG_RECEIVER

#include <fmt/core.h>
/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/*
 * This sketch demonstrate the central API(). An additional bluefruit
 * that has blehid as peripheral is required for the demo.
 */
#include <bluefruit.h>
#include <queue>

#include "itg_receiver.h"
#include "util.h"
#include "hardware.h"

void scan_callback(ble_gap_evt_adv_report_t *report);
void connect_callback(uint16_t conn_handle);
void connection_secured_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void gamepad_report_callback(hid_gamepad_report_t *report);
void processGamepadReport(hid_gamepad_report_t *report);
void keyboard_report_callback(hid_keyboard_report_t *report);
void processKeyboardReport(hid_keyboard_report_t *report);

/*
 * Polling or callback implementation
 * 0 = Request data using a poll
 * 1 = Use callbacks
 */
#define POLLING 0
/*
 * Device Type flag for use in the example
 * Change this if you want to demo the Gamepad!
 * 0 = Keyboard + Mouse
 * 1 = Gamepad
 */
#define DEVICE_TYPE 0

BLEClientHidAdafruit hid;

// Last checked report, to detect if there is changes between reports
hid_keyboard_report_t last_kbd_report = {0};
hid_mouse_report_t last_mse_report = {0};
hid_gamepad_report_t last_gpd_report = {0};

Adafruit_USBD_HID usb_hid;
uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()};
std::queue<hid_keyboard_report_t> usb_hid_pending_reports;

void receiver_setup1()
{
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
    // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
    TinyUSB_Device_Init(0);
#endif

    // Setup HID
    usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
    usb_hid.setPollInterval(2);
    usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    usb_hid.setStringDescriptor("TinyUSB Keyboard");

    // // Set up output report (on control endpoint) for Capslock indicator
    // usb_hid.setReportCallback(NULL, hid_report_callback);

    usb_hid.begin();

    Serial.begin(115200);
    // //  while ( !Serial ) delay(10);   // for nrf52840 with native usb
    while (!TinyUSBDevice.mounted())
    {
        digitalWrite(PIN_LED, 0);
        delay(100);
        digitalWrite(PIN_LED, 1);
        delay(400);
    };
}

void receiver_setup2()
{

#ifdef DEBUG_LOGS
    Serial.println("Bluefruit52 Central HID (Keyboard + Mouse + Gamepad) Example");
    Serial.println("--------------------------------------------------\n");
#endif // DEBUG_LOGS

    // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
    // SRAM usage required by SoftDevice will increase dramatically with number of connections
    Bluefruit.begin(0, 1);

    Bluefruit.setName("Bluefruit52 Central");

    // Init BLE Central Hid Serivce
    hid.begin();

#if POLLING == 0
    hid.setKeyboardReportCallback(keyboard_report_callback);
    hid.setGamepadReportCallback(gamepad_report_callback);
#endif

    // Increase Blink rate to different from PrPh advertising mode
    Bluefruit.setConnLedInterval(250);

    Bluefruit.configCentralBandwidth(BANDWIDTH_MAX);
    Bluefruit.Central.setConnInterval(6, 12); // 7.5 - 15 ms

    // Callbacks for Central
    Bluefruit.Central.setConnectCallback(connect_callback);
    Bluefruit.Central.setDisconnectCallback(disconnect_callback);

    // Set connection secured callback, invoked when connection is encrypted
    Bluefruit.Security.setSecuredCallback(connection_secured_callback);

    /* Start Central Scanning
     * - Enable auto scan if disconnected
     * - Interval = 100 ms, window = 80 ms
     * - Don't use active scan
     * - Filter only accept HID service in advertising
     * - Start(timeout) with timeout = 0 will scan forever (until connected)
     */
    Bluefruit.Scanner.setRxCallback(scan_callback);
    Bluefruit.Scanner.restartOnDisconnect(true);
    Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
    Bluefruit.Scanner.filterService(hid);   // only report HID service
    Bluefruit.Scanner.useActiveScan(true);
    Bluefruit.Scanner.start(0); // 0 = Don't stop scanning after n seconds
}

/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void scan_callback(ble_gap_evt_adv_report_t *report)
{
    // Since we configure the scanner with filterUuid()
    // Scan callback only invoked for device with hid service advertised
    // Connect to the device with hid service in advertising packet
    Bluefruit.Central.connect(report);
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void connect_callback(uint16_t conn_handle)
{
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

#ifdef DEBUG_LOGS
    Serial.println("Connected");
    Serial.print("Discovering HID  Service ... ");
#endif // DEBUG_LOGS

    if (hid.discover(conn_handle))
    {
#ifdef DEBUG_LOGS
        Serial.println("Found it");
#endif // DEBUG_LOGS

        // HID device mostly require pairing/bonding
        conn->requestPairing();
    }
    else
    {
#ifdef DEBUG_LOGS
        Serial.println("Found NONE");
#endif // DEBUG_LOGS

        // disconnect since we couldn't find blehid service
        conn->disconnect();
    }
}

void connection_secured_callback(uint16_t conn_handle)
{
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

#ifdef DEBUG_LOGS
    Serial.print("connected to ");
    print_ble_addr(conn->getPeerAddr().addr);
    Serial.println();
    char namebuf[128] = {0};
    conn->getPeerName(namebuf, 128);
    fmt::print("name: {}\n", namebuf);
#endif // DEBUG_LOGS

    if (!conn->secured())
    {
        // It is possible that connection is still not secured by this time.
        // This happens (central only) when we try to encrypt connection using stored bond keys
        // but peer reject it (probably it remove its stored key).
        // Therefore we will request an pairing again --> callback again when encrypted
        conn->requestPairing();
    }
    else
    {
#ifdef DEBUG_LOGS
        Serial.println("Secured");
#endif // DEBUG_LOGS

        // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.hid_information.xml
        uint8_t hidInfo[4];
        hid.getHidInfo(hidInfo);

#ifdef DEBUG_LOGS
        Serial.printf("HID version: %d.%d\n", hidInfo[0], hidInfo[1]);
        Serial.print("Country code: ");
        Serial.println(hidInfo[2]);
        Serial.printf("HID Flags  : 0x%02X\n", hidInfo[3]);
#endif // DEBUG_LOGS

        // BLEClientHidAdafruit currently only supports Boot Protocol Mode
        // for Keyboard and Mouse. If we are using a Keyboard + Mouse,
        // let's set the protocol mode on prph to Boot Mode.
#if DEVICE_TYPE == 0
        bool bootMode = hid.setBootMode(true);
#ifdef DEBUG_LOGS
        Serial.print("hid.setBootMode(true) ");
        Serial.println(bootMode);
#endif // DEBUG_LOGS
        (void)bootMode;
#endif

        // Enable Keyboard report notification if present on prph
        if (hid.keyboardPresent())
            hid.enableKeyboard();

        // Enable Mouse report notification if present on prph
        if (hid.mousePresent())
            hid.enableMouse();

        // Enable Gamepad report notification if present on prph
        if (hid.gamepadPresent())
            hid.enableGamepad();
#ifdef DEBUG_LOGS
        Serial.println("Ready to receive from peripheral");
#endif // DEBUG_LOGS
        digitalWrite(PIN_LED, 0);
    }
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    (void)conn_handle;
    (void)reason;

    digitalWrite(PIN_LED, 1);

#ifdef DEBUG_LOGS
    Serial.print("Disconnected, reason = 0x");
    Serial.println(reason, HEX);
#endif // DEBUG_LOGS
}

void receiver_loop()
{

#if POLLING == 1
    // nothing to do if hid not discovered
    if (!hid.discovered())
        return;

    if (hid.keyboardPresent())
    {
        /*------------- Polling Keyboard  -------------*/
        hid_keyboard_report_t kbd_report;

        // Get latest report
#ifdef DEBUG_LOGS
        Serial.println("Get report from Keyboard");
#endif // DEBUG_LOGS
        hid.getKeyboardReport(&kbd_report);
        processKeyboardReport(&kbd_report);
    }

    if (hid.gamepadPresent())
    {
        /*------------- Polling Gamepad  -------------*/
        hid_gamepad_report_t gpd_report;

        // Get latest report
#ifdef DEBUG_LOGS
        Serial.println("Get report from Gamepad");
#endif // DEBUG_LOGS
        hid.getGamepadReport(&gpd_report);
        processGamepadReport(&gpd_report);
    }

    // polling interval is 5 ms
    delay(5);
#endif

    if (TinyUSBDevice.suspended())
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        TinyUSBDevice.remoteWakeup();
    }

    if (!usb_hid_pending_reports.empty() && usb_hid.ready())
    {
        hid_keyboard_report_t report = usb_hid_pending_reports.front();
        usb_hid.keyboardReport(0, report.modifier, report.keycode);
        usb_hid_pending_reports.pop();
    }
    delay(2);
}

void gamepad_report_callback(hid_gamepad_report_t *report)
{
#ifdef DEBUG_LOGS
    Serial.println("Recieved report from Gamepad");
#endif // DEBUG_LOGS
    processGamepadReport(report);
}

void processGamepadReport(hid_gamepad_report_t *report)
{
    // TODO: do something with the gamepad report
    memcpy(&last_gpd_report, report, sizeof(hid_gamepad_report_t));
#ifdef DEBUG_LOGS
    Serial.print("Current Gamepad State: Buttons: ");
    Serial.print(report->buttons, BIN);
    Serial.print(" X: ");
    Serial.print(report->x);
    Serial.print(" Y: ");
    Serial.println(report->y);
#endif // DEBUG_LOGS
}

void keyboard_report_callback(hid_keyboard_report_t *report)
{
    processKeyboardReport(report);
}

#ifdef DEBUG_LOGS
void logKeyboardReport(hid_keyboard_report_t *report)
{
    bool shifted = false;
    if (report->modifier)
    {
        if (report->modifier & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL))
        {
            Serial.print("Ctrl ");
        }

        if (report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT))
        {
            Serial.print("Shift ");

            shifted = true;
        }

        if (report->modifier & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT))
        {
            Serial.print("Alt ");
        }
    }

    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t kc = report->keycode[i];
        char ch = 0;
        if (kc == 0)
        {
            continue;
        }

        if (kc < 128)
        {
            ch = shifted ? hid_keycode_to_ascii[kc][1] : hid_keycode_to_ascii[kc][0];
        }
        else
        {
            // non-US keyboard !!??
        }

        // Printable
        if (ch)
        {
            // Serial.print(ch);
            fmt::print("keycode {} char {}\n", kc, ch);
        }
        else
        {
            fmt::print("keycode {}\n", kc);
        }
    }
}
#endif // DEBUG_LOGS

void processKeyboardReport(hid_keyboard_report_t *report)
{
    // Check with last report to see if there is any changes
    if (memcmp(&last_kbd_report, report, sizeof(hid_keyboard_report_t)))
    {
#ifdef DEBUG_LOGS
        logKeyboardReport(report);
#endif // DEBUG_LOGS

        usb_hid_pending_reports.push(*report);
    }

    // update last report
    memcpy(&last_kbd_report, report, sizeof(hid_keyboard_report_t));
}

#endif // ITG_RECEIVER
