#!/usr/bin/env bash
set -euxo pipefail

source "$HOME/.platformio/penv/bin/activate"
pio="$HOME/.platformio/penv/bin/pio"
uf2conv="$HOME/.platformio/packages/framework-arduinoadafruitnrf52/tools/uf2conv/uf2conv.py"


# $pio run -e remote -t dfu
# python3 $uf2conv .pio/build/remote/firmware.hex -c -f 0xADA52840 -o remote_firmware.uf2
# $pio run -e receiver -t dfu
# python3 $uf2conv .pio/build/receiver/firmware.hex -c -f 0xADA52840 -o receiver_firmware.uf2
# python3 $uf2conv .pio/build/remote/firmware.hex -f 0xADA52840 -d /dev/ttyACM0
# python3 $uf2conv .pio/build/remote/firmware.hex -f 0xADA52840

for TARGET in receiver remote; do
    $pio run -e $TARGET
    $pio run -e $TARGET -t dfu
    # python3 $uf2conv .pio/build/$TARGET/firmware.hex -c -f 0xADA52840 -o ${TARGET}_firmware.uf2
    python3 $uf2conv \
        .pio/build/$TARGET/firmware.bin \
        -c -b 0x26000 -f 0xADA52840 \
        -o "${TARGET}_firmware.uf2"
done

# pio run -e remote -t upload
# pio run -e receiver -t upload
