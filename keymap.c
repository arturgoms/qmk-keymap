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
#include "quantum.h"

enum layers {
    BASE,
    GAMER,
    LOWER,
    RAISE,
    MAINTENCE,
    TMUX,
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
    MC_COMMENT,
    MC_CPR,
    MC_SAVE,
    MC_DELETE_WORD,
    MC_QUIT,
    MC_VISTA,
    MC_NEXT_TAB,
    MC_PREV_TAB,
    MC_BUFFERS,
    MC_MAXIMIZER,
    MC_HARPOON_1,
    MC_HARPOON_2,
    MC_HARPOON_3,
    MC_HARPOON_4,
    MC_HARPOON_5,
    MC_HARPOON_6,
    MC_HARPOON_7,
    MC_HARPOON_8,
    MC_HARPOON_9,
    MC_HARPOON_0,
    MC_TMUX_PREV,
    MC_TMUX_NEXT,
    MC_TMUX_SPLIT_H,
    MC_TMUX_SPLIT_V,
    MC_TMUX_RELOAD,
    MC_TMUX_RENAME,
    MC_TMUX_SWITCH_UP,
    MC_TMUX_SWITCH_DOWN,
    MC_TMUX_SWITCH_LEFT,
    MC_TMUX_SWITCH_RIGHT,
    MC_TMUX_KILL_SESSION,
    MC_TMUX_KILL_PANE,
    MC_TMUX_NEW,
    MC_TMUX_INSTALL,
    MC_TMUX_DETACH,
    MC_TMUX_SAVE,
    MC_TMUX_RESTORE,
    MC_NEXT_BUFFER,
    MC_PREV_BUFFER,
    MC_TMUX_CHSH,
    MC_TMUX_SESSIONIZER,
    MC_TMUX_SESSIONS,
    MC_SHIFT_CAPS
};

enum tap_dance{
    TD_SHIFT_CAPS,
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
// zz
// Home row mods for Colemak layer.
// #define HOME_A LT(SYM, KC_A)
// #define HOME_R LALT_T(KC_R)
#define HOME_S MT(MOD_LALT, KC_S)
#define HOME_T MT(MOD_LGUI, KC_T)
#define HOME_N MT(MOD_RGUI, KC_N)
#define HOME_E MT(MOD_RALT, KC_E)
// #define HOME_I LALT_T(KC_I)
// #define HOME_O LT(SYM, KC_O)
#define HOME_SLSH RGUI_T(KC_SLSH)

// Home row mods for QWERTY layer.
// #define QHOME_A LT(SYM, KC_A)
#define QHOME_S LALT_T(KC_S)
#define QHOME_D LSFT_T(KC_D)
#define QHOME_F LCTL_T(KC_F)
#define QHOME_J RCTL_T(KC_J)
// #define QHOME_K RSFT_T(KC_K)
#define QHOME_L LALT_T(KC_L)
// #define QHOME_SC LT(SYM, KC_SCLN)
#define QHOME_Z LGUI_T(KC_Z)
#define QHOME_SL RGUI_T(KC_SLSH)

#define LR_TMUX  LT(TMUX, KC_TAB)
#define LR_RAISE LT(RAISE, KC_BSPC)
#define LR_LOWER LT(LOWER, KC_SPC)
#define LR_MAIN  LT(MAINTENCE, KC_ENT)
#define MOD_SFEN  OSM(MOD_LSFT)
#define MOD_CTEN  OSM(MOD_LCTL)
#define KC_PPM KC_MEDIA_PLAY_PAUSE

// // Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_SHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(MOD_LSFT, KC_CAPS),
};


// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  [BASE] = LAYOUT_ortho_5x15(  // Base layer: Colemak with home row mods.
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5    , _______ ,_______, _______ , KC_6    , KC_7   , KC_8   , KC_9   , KC_0      , KC_BSPC,
    KC_TAB , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B    , _______ ,_______, _______ , KC_J    , KC_L   , KC_U   , KC_Y   , KC_QUOT   , REPEAT,
    KC_ESC , KC_A   , KC_R   , KC_S   , HOME_T , KC_G    , _______ ,_______, _______ , KC_M    , HOME_N , HOME_E , KC_I   , KC_O      , KC_ENT,
    _______, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V    , _______ ,_______, _______ , KC_K    , KC_H   , KC_COMM, KC_DOT , HOME_SLSH , _______,
    _______,_______ , KC_LALT, KC_LGUI, LR_TMUX, LR_RAISE, MOD_CTEN,_______, MOD_SFEN, LR_LOWER, LR_MAIN, KC_RGUI, KC_RALT, _______   , _______
  ),

  [GAMER] = LAYOUT_ortho_5x15(  // Alternative base layer: GAMER.
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,  _______ ,_______, _______ , KC_6    , KC_7   , KC_8   , KC_9   , KC_0    , KC_BSPC,
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,  _______ ,_______, _______ , KC_Y    , KC_U   , KC_I   , KC_O   , KC_P    , KC_DEL,
    KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,  _______ ,_______, _______ , KC_H    , KC_J   , KC_K   , KC_L   , KC_SCLN , KC_ENT,
    _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,  _______ ,_______, _______ , KC_N    , KC_M   , KC_COMM, KC_DOT , KC_SLSH , _______,
    _______, _______, KC_LALT, KC_SPC , KC_LCTL, LR_RAISE, MOD_CTEN,_______, MOD_SFEN, LR_LOWER, LR_MAIN, KC_RGUI, KC_RALT, _______ , _______
  ),

  [LOWER] = LAYOUT_ortho_5x15(
    KC_GRV , KC_F1  , KC_F2  , KC_F3  , KC_F4    , KC_F5      ,  _______ ,_______, _______ , KC_F6           , KC_F7        , KC_F8        , KC_F9  , KC_F10  , KC_BSPC,
    KC_TAB , _______, SELWORD, KC_RBRC, KC_RPRN, LSFT(KC_RBRC),  _______ ,_______, _______ , KC_DQUO         , LGUI(S(KC_TAB))   , LGUI(KC_TAB)   , KC_QUOT, _______ , KC_DEL,
    KC_ESC , _______, KC_PIPE, KC_LBRC, KC_LPRN, LSFT(KC_LBRC),  _______ ,_______, _______ , KC_LEFT         , KC_DOWN      , KC_UP        , KC_RGHT, _______ , KC_ENT,
    _______, _______, KC_SLSH, LSFT(KC_EQL), KC_SCLN , KC_MINS,  _______ ,_______, _______ , LSFT(KC_MINS)   , KC_COLN      , KC_EQL       , KC_BSLS, _______ , _______,
    _______, _______, _______, _______, _______, _______      ,  _______ ,_______, _______ , _______, _______, _______, _______, _______ ,_______
  ),

  [RAISE] = LAYOUT_ortho_5x15(
    KC_GRV , MC_HARPOON_1 , MC_HARPOON_2 ,MC_HARPOON_3,MC_HARPOON_4   , MC_HARPOON_5 ,  _______ ,_______, _______   , _______    , _______        , _______       , _______    , _______, KC_BSPC,
    KC_TAB , MC_QUIT      , _______      , LCTL(KC_H) , LCTL(KC_L)    , MC_BUFFERS   ,  _______ ,_______, _______   , LCTL(KC_U) , KC_HOME        , KC_END        , _______    , _______, KC_DEL,
    KC_ESC , _______      , _______      , LSFT(KC_L) , LSFT(KC_U)    , LCTL(KC_BSLS),  _______ ,_______, _______   , LCTL(KC_D) , MC_PREV_BUFFER , MC_NEXT_BUFFER, _______    , _______, KC_ENT,
    _______, _______      , _______      , MC_CPR     , MC_DELETE_WORD, MC_MAXIMIZER ,  _______ ,_______, _______   , MC_SAVE    , MC_VISTA       , MC_PREV_TAB   , MC_NEXT_TAB, _______, KC_RSFT,
    _______, _______      , _______      , _______    , _______       , _______      ,  _______ ,_______, _______   , _______    , _______        , _______       , _______    , _______,_______
  ),

  [MAINTENCE] = LAYOUT_ortho_5x15(
    KC_GRV , QK_BOOT, RGB_VAD , RGB_VAI , RGB_RMOD , _______ ,  _______ ,_______, _______   , _______, _______, _______, _______, _______, KC_BSPC,
    KC_TAB , EXIT   , RGB_VAD , RGB_VAI , RGB_MOD  , _______ ,  _______ ,_______, _______   , KC_WH_U, _______, _______, _______, _______, KC_DEL,
    KC_ESC , _______, RGB_HUD , RGB_HUI , _______  , DF(GAMER),  _______ ,_______, _______  , KC_WH_D, _______, _______, _______, _______, KC_ENT,
    KC_LSFT, _______, RGB_SAD , RGB_SAI , _______  , DF(BASE),  _______ ,_______, _______   , _______, _______, _______, _______, _______, KC_RSFT,
    _______, _______, _______ , _______ , _______  , _______ ,  _______ ,_______, _______   , _______, _______, _______, _______, _______,_______
  ),

  [TMUX] = LAYOUT_ortho_5x15(
    KC_GRV , _______              , _______           , _______      , _______        , _______        ,  _______,_______, _______   , _______            , _______             , _______           , _______             , _______, KC_BSPC,
    KC_TAB , MC_TMUX_KILL_SESSION , MC_TMUX_KILL_PANE , MC_TMUX_PREV , MC_TMUX_NEXT   , MC_TMUX_CHSH   ,  _______,_______, _______   , MC_TMUX_SESSIONIZER, _______             , MC_TMUX_INSTALL   , _______             , _______, KC_DEL,
    KC_ESC , MC_TMUX_RELOAD       , MC_TMUX_RESTORE   , MC_TMUX_SAVE , MC_TMUX_NEW    , MC_TMUX_RENAME ,  _______,_______, _______   , MC_TMUX_SWITCH_LEFT, MC_TMUX_SWITCH_DOWN , MC_TMUX_SWITCH_UP , MC_TMUX_SWITCH_RIGHT, _______, KC_ENT,
    _______, _______              , _______           , MC_TMUX_SESSIONS, MC_TMUX_DETACH , MC_TMUX_SPLIT_V,  _______,_______, _______   , _______            , MC_TMUX_SPLIT_H     , _______           , _______             , _______, _______,
    _______, _______              , _______           , _______      , _______        , _______        ,  _______ ,_______, _______   , _______            , _______             , _______           , _______             , _______,_______
  )
};
// clang-format on

const uint32_t unicode_map[] PROGMEM = {};

const custom_shift_key_t custom_shift_keys[] = {
    // {KC_DOT, KC_QUES},  // Shift . is ?
    // {KC_COMM, KC_EXLM},
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
    case QHOME_L:
    // Repeating Z is useful for spamming undo.
    // case HOME_Z:
    //   return false;  // Enable key repeating.
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
    // case HOME_A:  // A + U.
    //   if (other_keycode == HOME_T) {
    //     return true;
    //   }
    //   break;
    //
    // case HOME_O:  // S + H and S + G.
    //   if (other_keycode == HOME_N || other_keycode == KC_L) {
    //     return true;
    //   }
    //   break;

    default:
      return true;
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
  // switch (tap_hold_keycode) {
  //   case HOME_Z:
  //     return 0;  // Bypass Achordion for these keys.
  // }

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

typedef enum {
    PROCESS_RECORD_RETURN_TRUE,
    PROCESS_RECORD_RETURN_FALSE,
    PROCESS_RECORD_CONTINUE
} process_record_result_t;

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
        layer_off(MAINTENCE);
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
        static const char username[] PROGMEM = "arturgoms";
        static const char last_name[] PROGMEM = "Gomes";
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
                // Vim
        case MC_CPR:
            tap_code(KC_ESCAPE);
            SEND_STRING(":%s///g");
            SEND_STRING(SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_COMMENT:
            tap_code(KC_ESCAPE);
            SEND_STRING(" /");
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_DELETE_WORD:
            tap_code(KC_D);
            tap_code(KC_I);
            tap_code(KC_W);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_SAVE:
            tap_code(KC_ESCAPE);
            SEND_STRING(":w");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_QUIT:
            tap_code(KC_ESCAPE);
            SEND_STRING(":q");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_VISTA:
            tap_code(KC_ESCAPE);
            SEND_STRING(":Vista!!");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_NEXT_TAB:
            tap_code(KC_ESCAPE);
            SEND_STRING(":tabnext");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_PREV_TAB:
            tap_code(KC_ESCAPE);
            SEND_STRING(":tabprevious");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_BUFFERS:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" user.bfs\").open()");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_MAXIMIZER:
            tap_code(KC_ESCAPE);
            SEND_STRING(":MaximizerToggle");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_1:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_1);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_2:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_2);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_3:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_3);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_4:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_4);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_5:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_5);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_6:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_6);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_7:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_7);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_8:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_8);
            return  PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_9:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_9);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_0:
            tap_code(KC_ESCAPE);
            SEND_STRING("M");
            tap_code(KC_0);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_NEXT_BUFFER:
            tap_code(KC_ESCAPE);
            SEND_STRING(":bnext");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_PREV_BUFFER:
            tap_code(KC_ESCAPE);
            SEND_STRING(":bprevious");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        // Tmux

        case MC_TMUX_NEXT:
            tap_code16(LCTL(KC_B));
            SEND_STRING("n");
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_PREV:
            tap_code16(LCTL(KC_B));
            SEND_STRING("p");
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SPLIT_H:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_H);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SPLIT_V:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_V);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_RELOAD:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_R);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_RENAME:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_COMM);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SWITCH_UP:
            tap_code16(LCTL(KC_B));
            tap_code(KC_UP);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SWITCH_DOWN:
            tap_code16(LCTL(KC_B));
            tap_code(KC_DOWN);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SWITCH_LEFT:
            tap_code16(LCTL(KC_B));
            tap_code(KC_LEFT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SWITCH_RIGHT:
            tap_code16(LCTL(KC_B));
            tap_code(KC_RGHT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_KILL_SESSION:
            tap_code16(LCTL(KC_B));
            tap_code(KC_Q);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_KILL_PANE:
            tap_code16(LCTL(KC_B));
            tap_code(KC_W);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_NEW:
            tap_code16(LCTL(KC_B));
            tap_code(KC_C);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_INSTALL:
            tap_code16(LCTL(KC_B));
            tap_code16(LSFT(KC_U));
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_DETACH:
            tap_code16(LCTL(KC_B));
            tap_code(KC_D);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SAVE:
            tap_code16(LCTL(KC_B));
            tap_code16(LCTL(KC_S));
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_CHSH:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_I);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SESSIONIZER:
            tap_code16(LCTL(KC_B));
            tap_code(KC_F);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_SESSIONS:
            tap_code16(LCTL(KC_B));
            tap_code(KC_S);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_TMUX_RESTORE:
              tap_code16(LCTL(KC_B));
              tap_code16(LCTL(KC_R));
              return PROCESS_RECORD_RETURN_FALSE;
    }
  }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
  select_word_task();
  sentence_case_task();
}

int leds[] = {
     68, 67, 66, 65, 64, /*    */ 60, 59, 58, 57, 56,
     53, 52, 51 ,50, 49, /*    */ 45, 44, 43, 42, 41,
     38, 37, 36, 35, 34, /*    */ 30, 29, 28, 27, 26,
};
const int led_count = 30;

void set_rgblight_by_layer(uint32_t layer) {
     switch (layer) {
          case BASE:
               break;
          case LOWER:
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_WHITE);
               }
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_PURPLE);
               }
               break;
          case RAISE:
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_WHITE);
               }
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_RED);
               }
               break;
          case MAINTENCE:
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_WHITE);
               }
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_SPRINGGREEN);
               }
               break;
          default:
               for (int i = 0; i < led_count; i++) {
                    rgb_matrix_set_color(leds[i], RGB_WHITE);
               }
               break;
     }
}

void set_current_layer_rgb(void) {
    set_rgblight_by_layer(get_highest_layer(layer_state | default_layer_state));
}

layer_state_t layer_state_set_user(layer_state_t state) {
    set_rgblight_by_layer(biton32(state));
    return state;
}

bool rgb_matrix_indicators_user(void) {
    uint8_t mods                = get_mods();
    uint8_t oneshot_mods        = get_oneshot_mods();
    uint8_t oneshot_locked_mods = get_oneshot_locked_mods();

    bool isShift = mods & MOD_MASK_SHIFT || oneshot_mods & MOD_MASK_SHIFT || oneshot_locked_mods & MOD_MASK_SHIFT;
    bool isCtrl  = mods & MOD_MASK_CTRL || oneshot_mods & MOD_MASK_CTRL || oneshot_locked_mods & MOD_MASK_CTRL;
    bool isAlt   = mods & MOD_MASK_ALT || oneshot_mods & MOD_MASK_ALT || oneshot_locked_mods & MOD_MASK_ALT;
    bool isGui   = mods & MOD_MASK_GUI || oneshot_mods & MOD_MASK_GUI || oneshot_locked_mods & MOD_MASK_GUI;

    for (int i = 0; i < led_count; i++) {
        if (isShift) {
            rgb_matrix_set_color(leds[i], RGB_SPRINGGREEN);
        } else if (isCtrl) {
            rgb_matrix_set_color(leds[i], RGB_RED);
        } else if ( isAlt || isGui) {
            rgb_matrix_set_color(leds[i], RGB_WHITE);
        } else {
            set_current_layer_rgb();
        }
    }

    return true;
}
