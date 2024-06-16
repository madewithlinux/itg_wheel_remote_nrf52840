#include <fmt/core.h>
#include <fmt/format.h>
// #include <fmt/ranges.h>

#include "util.h"

void print_ble_addr(uint8_t addr[BLE_GAP_ADDR_LEN])
{
    fmt::print(
        "{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}",
        addr[5],
        addr[4],
        addr[3],
        addr[2],
        addr[1],
        addr[0]);
}