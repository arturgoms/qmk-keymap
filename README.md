# Artur's QMK Keymap

![Sofle v2 Choc](doc/image.JPG)

Highly inspired by: [getreuer](https://getreuer.info/posts/keyboards/tour/index.html)

## Keyboard

Sofle v2 Choc with RP2040 controller

## My Keymap

![Keymap](doc/sofle.svg)

## License

This code uses the Apache License 2.0. See the [LICENSE file](LICENSE.txt) for details.

## Installation

Clone the [QMK firmware](https://github.com/qmk/qmk_firmware) and place this
repo in `qmk_firmware/keyboards/sofle_choc/keymaps/arturgoms`.

1. Compile
```bash
qmk compile -kb sofle_choc -km arturgoms -e CONVERT_TO=rp2040_ce
```

2. Flash
   - Put the RP2040 into bootloader mode (hold BOOT button while plugging in, or double-tap RESET)
   - It will appear as a USB drive (usually named `RPI-RP2`)
   - Drag the `.uf2` file from `qmk_firmware/.build/` to the drive
   - The controller will automatically reboot with the new firmware

## Adding Autocorrection Words

The autocorrection dictionary lives in `features/autocorrection_dict.txt`. To add a new word:

1. Edit the dictionary file and add your entry:
```
typo          -> correction
```

Use `:` prefix/suffix to match word boundaries (e.g., `:thier` only matches at word start, `looses:` only at word end).

2. Regenerate the binary trie (run from the `qmk_firmware` directory):
```bash
qmk generate-autocorrect-data keyboards/sofle_choc/keymaps/arturgoms/features/autocorrection_dict.txt \
  -o keyboards/sofle_choc/keymaps/arturgoms/autocorrect_data.h
```

3. **Clean build** and flash (important: `qmk clean` is needed after updating the autocorrect data):
```bash
qmk clean && qmk compile -kb sofle_choc -km arturgoms -e CONVERT_TO=rp2040_ce
```

**Why `qmk clean`?** QMK's autocorrect loads the dictionary via `__has_include("autocorrect_data.h")` inside `process_autocorrect.c`. The build cache doesn't track when this header is added or changed — without `qmk clean`, the stale object file is reused and your new words won't take effect.

See [QMK Autocorrect docs](https://docs.qmk.fm/features/autocorrect) for full syntax details.

## Generating the Keymap SVG

To regenerate the `doc/sofle.svg` keymap image:

1. Generate the YAML file from the keymap source:
```bash
qmk c2json keyboards/sofle_choc/keymaps/arturgoms/keymap.c --no-cpp \
  | keymap parse --layer-names BASE LOWER RAISE MAINTENANCE TMUX GAMER BASE_PLUS -c 14 -q - \
  > ~/projects/personal/qmk-keymap/sofle_keymap.yaml
```

2. Upload `sofle_keymap.yaml` to [keymap-drawer](https://keymap-drawer.streamlit.app) to generate the SVG
