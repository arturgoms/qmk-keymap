# Artur's QMK Keymap

![Sofle v2 Choc](doc/image.JPG)

Highly inspired by: [getreuer](https://getreuer.info/posts/keyboards/tour/index.html)

## Keyboard

Sofle v2 Choc with RP2040 controller

## My Keymap

![Keymap](doc/idobao.png)

## License

This code uses the Apache License 2.0. See the [LICENSE file](LICENSE.txt) for details.

## Installation

Clone the [QMK firmware](https://github.com/qmk/qmk_firmware) and place this
repo in `qmk_firmware/keyboards/sofle_choc/keymaps/arturgoms`.

1. Compile
```bash
qmk compile -kb sofle_choc -km arturgoms
```

2. Flash
   - Put the RP2040 into bootloader mode (hold BOOT button while plugging in, or double-tap RESET)
   - It will appear as a USB drive (usually named `RPI-RP2`)
   - Drag the `.uf2` file from `qmk_firmware/.build/` to the drive
   - The controller will automatically reboot with the new firmware
