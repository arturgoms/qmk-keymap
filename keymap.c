// Copyright 2021-2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @mainpage Pascal Getreuer's QMK keymap
 *
 * This is my Quantum Mechanical Keyboard (QMK) keymap for the Dactyl Ergodox.
 * Who knew a keyboard could do so much?
 *
 * Feature libraries
 * -----------------
 *  * features/achordion.h: customize the tap-hold decision
 *  * features/autocorrection.h: run rudimentary autocorrection on your keyboard
 *  * features/caps_word.h: modern alternative to Caps Lock
 *  * features/custom_shift_keys.h: they're surprisingly tricky to get right;
 *                                  here is my approach
 *  * features/layer_lock.h: macro to stay in the current layer
 *  * features/mouse_turbo_click.h: macro that clicks the mouse rapidly
 *  * features/repeat_key.h: a "repeat last key" implementation
 *  * features/sentence_case.h: capitalize first letter of sentences
 *  * features/select_word.h: macro for convenient word or line selection
 *
 * License
 * -------
 * This code uses the Apache License 2.0. See LICENSE.txt for details.
 *
 * For further documentation of this keymap's features, see
 * <https://getreuer.info/posts/keyboards>
 */

#include QMK_KEYBOARD_H

#include "features/achordion.h"
#include "features/custom_shift_keys.h"
#include "features/repeat_key.h"
#include "features/select_word.h"
#include "features/sentence_case.h"
#include "layout.h"

enum layers {
  BASE,
  QWERTY,
  SYM,
  ADJUST,
};

enum custom_keycodes {
  UPDIR = SAFE_RANGE,
  EXIT,
  SCOPE,
  SELWORD,
  TMUXESC,
  SRCHSEL,
  USRNAME,
  DASH,
  ARROW,
  THMBUP,
  REPEAT,
  ALTREP,
};

// This keymap uses home row mods. In addition to mods, I have home row
// layer-tap keys for the SYM layer. The key arrangement is a variation on
// "GASC-order" home row mods:
//
//             Left hand                          Right hand
// +-------+-------+-------+-------+   +-------+-------+-------+-------+
// |  Sym  |  Alt  | Shift | Ctrl  |   | Ctrl  | Shift |  Alt  |  Sym  |
// +-------+-------+-------+-------+   +-------+-------+-------+-------+
// |  Gui  |                                                   |  Gui  |
// +-------+                                                   +-------+

// Home row mods for Dvorak layer.
#define HOME_A LT(SYM, KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LSFT_T(KC_S)
#define HOME_T LCTL_T(KC_T)
#define HOME_Z LGUI_T(KC_Z)

#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_I LALT_T(KC_I)
#define HOME_O LALT_T(KC_O)
#define HOME_SLSH LT(SYM, KC_SLSH)

// Home row mods for QWERTY layer.
#define QHOME_A LT(SYM, KC_A)
#define QHOME_S LALT_T(KC_S)
#define QHOME_D LSFT_T(KC_D)
#define QHOME_F LCTL_T(KC_F)
#define QHOME_J RCTL_T(KC_J)
#define QHOME_K RSFT_T(KC_K)
#define QHOME_L LALT_T(KC_L)
#define QHOME_SC LT(SYM, KC_SCLN)
#define QHOME_Z LGUI_T(KC_Z)
#define QHOME_SL RGUI_T(KC_SLSH)

// I have F13 bound to toggle Redshift (http://jonls.dk/redshift/).
#define REDSHFT KC_F13

// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  [BASE] = LAYOUT_ortho_5x15(  // Base layer: Colemak with home row mods.
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , QK_BOOT, EXIT    , _______    , KC_6   , KC_7   , KC_8   , KC_9   , KC_0      , KC_BSPC,
    KC_TAB , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   , DF(BASE), RGB_VAI, DF(QWERTY) , KC_J   , KC_L   , KC_U   , KC_Y   , KC_QUOT   , KC_DEL,
    KC_ESC , HOME_A , HOME_R , HOME_S , HOME_T , KC_G   , KC_MRWD, KC_MPLY , KC_MFFD    , KC_M   , HOME_N , HOME_E , HOME_I , HOME_O    , KC_ENT,
    KC_LSFT, HOME_Z , KC_X   , KC_C   , KC_D   , KC_V   , KC_LBRC, KC_UP   , KC_RBRC    , KC_K   , KC_H   , KC_COMM, KC_DOT , HOME_SLSH , KC_RSFT,
    // KC_LCTL, XXXXXXX, KC_LALT, KC_LGUI,
    //                         LT(_TMUX, KC_ESC),LT(_RAISE, KC_BSPC), KC_DOWN ,KC_LSFT,
    //                                                          LT(_LOWER, KC_SPC) , LT(_MAINTENANCE, KC_ENT), KC_BSLS, SELWORD, REPEAT    , KC_RCTL
    MO(SYM), KC_UNDS, KC_WH_D, KC_DEL , KC_SPC , KC_BTN1, KC_LEFT, KC_DOWN, KC_RGHT , KC_MINS, ALTREP , KC_BSLS, SELWORD, REPEAT    , KC_RCTL
  ),

  [QWERTY] = LAYOUT_ortho_5x15(  // Alternative base layer: QWERTY.
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,  _______,_______, _______   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0    , KC_BSPC,
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,  DF(BASE),_______,DF(QWERTY), KC_Y   , KC_U   , KC_I   , KC_O   , KC_P    , KC_DEL,
    KC_ESC , QHOME_A, QHOME_S, QHOME_D, QHOME_F, KC_G   ,  _______,_______, _______   , KC_H   , QHOME_J, QHOME_K, QHOME_L, QHOME_SC, KC_ENT,
    KC_LSFT, QHOME_Z, KC_X   , KC_C   , KC_V   , KC_B   ,  _______,_______, _______   , KC_N   , KC_M   , KC_COMM, KC_DOT , QHOME_SL, KC_RSFT,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_RCTL
  )
};
// clang-format on

const uint32_t unicode_map[] PROGMEM = {};

const custom_shift_key_t custom_shift_keys[] = {
    {KC_DOT, KC_QUES},  // Shift . is ?
    {KC_COMM, KC_EXLM},
    {KC_EQL, KC_EQL},  // Don't shift =
    {KC_SLSH, KC_SLSH},  // Don't shift /
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(*custom_shift_keys);

const uint16_t caps_combo[] PROGMEM = {KC_DOT, KC_C, COMBO_END};
combo_t key_combos[] = {
    // . and C => activate Caps Word.
    COMBO(caps_combo, CW_TOGG),
};
uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(*key_combos);

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning true means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    // Repeating is useful for Vim navigation keys.
    case HOME_T:
    case HOME_N:
    case QHOME_J:
    case QHOME_K:
    case QHOME_L:
    // Repeating Z is useful for spamming undo.
    case HOME_Z:
      return false;  // Enable key repeating.
    default:
      return true;  // Otherwise, force hold and disable key repeating.
  }
}

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    // I have a dedicated underscore key, so no need to shift KC_MINS.
    case KC_MINS:
    case KC_UNDS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                     uint16_t other_keycode, keyrecord_t* other_record) {
  // Exceptionally consider the following chords as holds, even though they
  // are on the same hand in Dvorak.
  switch (tap_hold_keycode) {
    case HOME_A:  // A + U.
      if (other_keycode == HOME_T) {
        return true;
      }
      break;

    case HOME_O:  // S + H and S + G.
      if (other_keycode == HOME_N || other_keycode == KC_L) {
        return true;
      }
      break;
  }

  // Also allow same-hand holds when the other key is in the rows below the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
  if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 4) {
    return true;
  }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case HOME_Z:
      return 0;  // Bypass Achordion for these keys.
  }

  return 800;  // Otherwise use a timeout of 800 ms.
}

char sentence_case_press_user(uint16_t keycode, keyrecord_t* record,
                              uint8_t mods) {
  if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
    const bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
      case KC_LCTL ... KC_RGUI:  // Mod keys.
        return '\0';  // These keys are ignored.

      case KC_A ... KC_Z:
        return 'a';  // Letter key.

      case KC_DOT:  // Both . and Shift . (?) punctuate sentence endings.
        return '.';
      case KC_COMM:  // Shift , (!) is a sentence ending.
        return shifted ? '.' : '#';

      case KC_1 ... KC_0:  // 1 2 3 4 5 6 7 8 9 0
      case KC_MINS ... KC_SCLN:  // - = [ ] ; ` backslash
      case KC_GRV:
      case KC_SLSH:
        return '#';  // Symbol key.

      case KC_SPC:
        return ' ';  // Space key.

      case KC_QUOT:
        return '\'';  // Quote key.
    }
  }

  // Otherwise clear Sentence Case to initial state.
  sentence_case_clear();
  return '\0';
}

// clang-format off
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_repeat_key_with_alt(keycode, record, REPEAT, ALTREP)) {
    return false;
  }
  if (!process_custom_shift_keys(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  if (!process_sentence_case(keycode, record)) { return false; }

  const uint8_t mods = get_mods();
  const bool shifted = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
                       ) & MOD_MASK_SHIFT;

  if (record->event.pressed) {
    switch (keycode) {
      case EXIT:
        layer_off(ADJUST);
        return false;

      case SCOPE:
        SEND_STRING("::");
        return false;

      case UPDIR:
        SEND_STRING("../");
        return false;

      case TMUXESC:  // Enter copy mode in Tmux.
        SEND_STRING(SS_LCTL("a") SS_TAP(X_ESC));
        return false;

      case SRCHSEL:  // Searches the current selection in a new tab.
        // Mac users, change LCTL to LGUI.
        SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
        return false;

      case USRNAME: {  // Type my username, or if Shift is held, my last name.
        static const char username[] PROGMEM = "getreuer";
        static const char last_name[] PROGMEM = "Getreuer";
        clear_weak_mods();
        unregister_mods(mods);  // Clear mods before send_string.
        send_string_P(shifted ? last_name : username);
        register_mods(mods);  // Restore mods.
      } break;

      // The following cases type a few Unicode symbols.
      //
      // `send_unicode_hex_string()` is deprecated. The docs suggest to ensure
      // keymap.c is UTF-8 encoded and write literal Unicode characters in the
      // string passed to `send_unicode_string()`. Unfortunately, terminals can
      // have problems displaying Unicode correctly with monospaced width (or
      // at all). So we take another approach: write escape codes `\xhh` for the
      // UTF-8 encoding.

      case DASH:  // En dash, or em dash when shifted.
        send_unicode_string(shifted ? "\xe2\x80\x94" : "\xe2\x80\x93");
        return false;

      case ARROW:  // -> Unicode arrow, or => when shifted.
        send_unicode_string(shifted ? "\xe2\x87\x92" : "\xe2\x86\x92");
        return false;

      case THMBUP:  // Thumbs up emoji, or party emoji when shifted.
        send_unicode_string(shifted ? "\xf0\x9f\xa5\xb3" : "\xf0\x9f\x91\x8d");
        return false;
    }
  }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
  select_word_task();
  sentence_case_task();
}
