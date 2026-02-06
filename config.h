/* Copyright 2023 Brian Low
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

// Enabling this option changes the startup behavior to listen for an
// active USB communication to delegate which part is master and which
// is slave. With this option enabled and theres's USB communication,
// then that half assumes it is the master, otherwise it assumes it
// is the slave.
//
// I've found this helps with some ProMicros where the slave does not boot
#define SPLIT_USB_DETECT

#define RGB_MATRIX_SLEEP     // turn off effects when suspended
#define SPLIT_TRANSPORT_MIRROR             // If LED_MATRIX_KEYPRESSES or LED_MATRIX_KEYRELEASES is enabled, you also will want to enable SPLIT_TRANSPORT_MIRROR
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 100  // limits maximum brightness of LEDs (max 255). Higher may cause the controller to crash.

// RGB indicators on split keyboards will require state information synced to the slave half (e.g. #define SPLIT_LAYER_STATE_ENABLE). See data sync options for more details.
// https://docs.qmk.fm/#/feature_split_keyboard?id=data-sync-options
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE

// Tap-hold configuration for home row mods.
#define TAPPING_TERM 180
#define PERMISSIVE_HOLD

// Caps Word
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 5000
#define CAPS_WORD_INVERT_ON_SHIFT

// Select Word
#define SELECT_WORD_TIMEOUT 2000

// Sentence Case
#define SENTENCE_CASE_TIMEOUT 2000

// Layer Lock
#define LAYER_LOCK_IDLE_TIMEOUT 60000

// Custom Shift Keys
#define CUSTOM_SHIFT_KEYS_NEGMODS ~MOD_MASK_SHIFT

// Orbital Mouse
#define ORBITAL_MOUSE_SPEED_CURVE \
    {24, 24, 24, 32, 62, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72}

// Mouse Turbo Click - use new keycode name
#define MOUSE_TURBO_CLICK_KEY MS_BTN1

// PaletteFx
#define RGB_MATRIX_CUSTOM_USER
#define PALETTEFX_ENABLE_ALL_EFFECTS
#define PALETTEFX_ENABLE_ALL_PALETTES

// TAP_CODE_DELAY for macros
#ifndef TAP_CODE_DELAY
#define TAP_CODE_DELAY 5
#endif
