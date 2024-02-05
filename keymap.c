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
#include "features/select_word.h"
#include "features/sentence_case.h"
#include "layout.h"
#include "quantum.h"

enum layers {
    BASE,
    LOWER,
    RAISE,
    MAINTENCE,
    TMUX,
		GAMER
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
		MC_HARPOON_UI,
		MC_HARPOON_MARK,
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
    MC_SHIFT_CAPS,
	  M_ION,
		M_NION,
		M_MENT,
		M_QUEN,
		M_TMENT,
		M_THE,
		M_UPDIR,
		M_INCLUDE,
		M_DOCSTR,
		M_MKGRVS,
		M_EQEQ,
		MC_HARPOON_PREV,
		MC_HARPOON_NEXT,
		MC_HARPOON_GOTO_1,
		MC_HARPOON_GOTO_2,
		MC_HARPOON_GOTO_3,
		MC_HARPOON_GOTO_4,
		MC_HARPOON_GOTO_5,
};

enum tap_dance{
    TD_SHIFT_CAPS,
};


// This keymap uses Ikcelaks' Magic Sturdy layout for the base layer (see
// https://github.com/Ikcelaks/keyboard_layouts). I've also made some twists of
// my own. The "magic" is a key whose function depends on the last pressed key,
// implemented using the Alternate Repeat Key. This key is used to remove the
// top SFBs and type common n-grams.
//
// The following describes the functionality, where * is the magic key and @ the
// repeat key. For example, tapping A and then the magic key types "ao".
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PY      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (lite "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     < -   -> <-              (Haskell code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
#define MAGIC QK_AREP  // The "magic" key is Alternate Repeat.

// This keymap uses home row mods. In addition to mods, I have home row
// layer-tap keys for the SYM layer. The key arrangement is a variation on
// "GASC-order" home row mods:
//
//             Left hand                          Right hand
// +-------+-------+-------+-------+   +-------+-------+-------+-------+
// |  Gui  |  Alt  | Shift | Ctrl  |   | Ctrl  | Shift |  Alt  |  Gui  |
// +-------+-------+-------+-------+   +-------+-------+-------+-------+


// Home row mods for Magic Sturdy layer.
#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LSFT_T(KC_S)
#define HOME_T LCTL_T(KC_T)

#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_I LALT_T(KC_I)
#define HOME_O RGUI_T(KC_O)


#define LR_TMUX  LT(TMUX, KC_TAB)
#define LR_RAISE LT(RAISE, KC_BSPC)
#define LR_LOWER LT(LOWER, KC_SPC)
#define LR_MAIN  LT(MAINTENCE, QK_REP)
#define LR_MAIN  LT(MAINTENCE, QK_REP)

/* #define MOD_SFEN  OSM(MOD_LSFT) */
#define MOD_CTEN  OSM(MOD_LCTL)

#define KC_PPM KC_MEDIA_PLAY_PAUSE

// // Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_SHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(MOD_LSFT, KC_CAPS),
};


// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  [BASE] = LAYOUT_ortho_5x15(
		// Base layer: Colemak with home row mods.
		KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5    , _______ ,_______, _______ , KC_6    , KC_7   , KC_8   , KC_9   , KC_0      , KC_BSPC,
		KC_TAB , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B    , _______ ,_______, _______ , KC_J    , KC_L   , KC_U   , KC_Y   , KC_QUOT   , KC_DEL,
	  KC_ESC , HOME_A , HOME_R , HOME_S , HOME_T , KC_G    , _______ ,_______, _______ , KC_M    , HOME_N , HOME_E , HOME_I , HOME_O    , KC_ENT,
		_______, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V    , _______ ,_______, _______ , KC_K    , KC_H   , KC_COMM, KC_DOT , KC_SLSH   , _______,
		_______,_______ , KC_LALT, KC_LGUI, LR_TMUX, LR_RAISE, MOD_CTEN  ,_______, QK_AREP, LR_LOWER, LR_MAIN, DF(GAMER), CW_TOGG, _______   , _______
  ),

  [LOWER] = LAYOUT_ortho_5x15(
    KC_GRV , KC_F1  , KC_F2  , KC_F3  , KC_F4    , KC_F5      ,  _______ ,_______, _______ , KC_F6           , KC_F7        , KC_F8        , KC_F9  , KC_F10  , KC_BSPC,
    KC_TAB , SCOPE  , SELWORD, KC_RBRC, KC_RPRN, LSFT(KC_RBRC),  _______ ,_______, _______ , KC_DQUO         , LCTL(KC_LEFT)   , LCTL(KC_RGHT)   , KC_QUOT, _______ , KC_DEL,
    KC_ESC , _______, KC_PIPE, KC_LBRC, KC_LPRN, LSFT(KC_LBRC),  _______ ,_______, _______ , KC_LEFT         , KC_DOWN      , KC_UP        , KC_RGHT, _______ , KC_ENT,
    _______, _______, KC_SLSH, LSFT(KC_EQL), KC_SCLN , KC_MINS,  _______ ,_______, _______ , LSFT(KC_MINS)   , KC_COLN      , KC_EQL       , KC_BSLS, _______ , _______,
    _______, _______, _______, _______, _______, _______      ,  _______ ,_______, _______ , _______, _______, _______, _______, _______ ,_______
  ),

  [RAISE] = LAYOUT_ortho_5x15(
    KC_GRV , MC_HARPOON_GOTO_1 , MC_HARPOON_GOTO_2 ,MC_HARPOON_GOTO_3,MC_HARPOON_GOTO_4 , MC_HARPOON_GOTO_5 ,  _______ ,_______, _______   , _______    , _______        , _______       , _______    , _______, KC_BSPC,
    KC_TAB , MC_QUIT, MC_HARPOON_MARK, LCTL(KC_H) , LCTL(KC_L)    , MC_HARPOON_UI,  _______ ,_______, _______   , LCTL(KC_U) , KC_HOME        , KC_END        , _______    , _______, KC_DEL,
    KC_ESC , _______, _______, MC_PREV_BUFFER     , MC_NEXT_BUFFER, LCTL(KC_T)   ,  _______ ,_______, _______   , LCTL(KC_D) , MC_HARPOON_PREV , MC_HARPOON_NEXT, _______    , _______, KC_ENT,
    _______, _______, _______, MC_CPR             , MC_DELETE_WORD, MC_MAXIMIZER ,  _______ ,_______, _______   , MC_SAVE    , MC_VISTA       , _______       , _______    , _______, KC_RSFT,
    _______, _______, _______, _______            , _______       , _______      ,  _______ ,_______, _______   , _______    , _______        , _______       , _______    , _______,_______
  ),

  [MAINTENCE] = LAYOUT_ortho_5x15(
    KC_GRV , QK_BOOT, _______ , _______ , _______ , _______ ,  _______ ,_______, _______   , _______, _______, _______, _______, _______, KC_BSPC,
    KC_TAB , EXIT   , RGB_VAD , RGB_VAI , RGB_MOD  , _______ ,  _______ ,_______, _______   , KC_WH_U, _______, _______, _______, _______, KC_DEL,
    KC_ESC , _______, RGB_HUD , RGB_HUI , _______  , _______ ,  _______ ,_______, _______  , KC_WH_D, _______, _______, _______, _______, KC_ENT,
    KC_LSFT, _______, RGB_SAD , RGB_SAI , _______  , _______ ,  _______ ,_______, _______   , _______, _______, _______, _______, _______, KC_RSFT,
    _______, _______, _______ , _______ , _______  , _______ ,  _______ ,_______, _______   , _______, _______, _______, _______, _______,_______
  ),

  [TMUX] = LAYOUT_ortho_5x15(
    KC_GRV , _______              , _______           , _______      , _______        , _______        ,  _______,_______, _______   , _______            , _______             , _______           , _______             , _______, KC_BSPC,
    KC_TAB , MC_TMUX_KILL_SESSION , MC_TMUX_KILL_PANE , MC_TMUX_PREV , MC_TMUX_NEXT   , MC_TMUX_CHSH   ,  _______,_______, _______   , MC_TMUX_SESSIONIZER, _______             , MC_TMUX_INSTALL   , _______             , _______, KC_DEL,
    KC_ESC , MC_TMUX_RELOAD       , MC_TMUX_RESTORE   , MC_TMUX_SAVE , MC_TMUX_NEW    , MC_TMUX_RENAME ,  _______,_______, _______   , MC_TMUX_SWITCH_LEFT, MC_TMUX_SWITCH_DOWN , MC_TMUX_SWITCH_UP , MC_TMUX_SWITCH_RIGHT, _______, KC_ENT,
    _______, _______              , _______           , MC_TMUX_SESSIONS, MC_TMUX_DETACH , MC_TMUX_SPLIT_V,  _______,_______, _______   , _______            , MC_TMUX_SPLIT_H     , _______           , _______             , _______, _______,
    _______, _______              , _______           , _______      , _______        , _______        ,  _______ ,_______, _______   , _______            , _______             , _______           , _______             , _______,_______
  ),
  [GAMER] = LAYOUT_ortho_5x15(  // Alternative base layer: GAMER.
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,  _______ ,_______, _______ , KC_6    , KC_7   , KC_8   , KC_9   , KC_0    , KC_BSPC,
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,  _______ ,_______, _______ , KC_Y    , KC_U   , KC_I   , KC_O   , KC_P    , KC_DEL,
    KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,  _______ ,_______, _______ , KC_H    , KC_J   , KC_K   , KC_L   , KC_SCLN , KC_ENT,
    _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,  _______ ,_______, _______ , KC_N    , KC_M   , KC_COMM, KC_DOT , KC_SLSH , _______,
    _______, _______, KC_LALT, KC_SPC , KC_LCTL, LR_RAISE, MOD_CTEN,_______, QK_AREP, LR_LOWER, LR_MAIN, KC_RGUI, DF(BASE), _______ , _______
  ),
};
// clang-format on

const uint32_t unicode_map[] PROGMEM = {};

const custom_shift_key_t custom_shift_keys[] = {
    /* {KC_DOT, KC_QUES},  // Shift . is ? */
    /* {KC_COMM, KC_EXLM}, */
    /* {KC_EQL, KC_EQL},  // Don't shift = */
    /* {KC_SLSH, KC_SLSH},  // Don't shift / */
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(*custom_shift_keys);

const uint16_t caps_combo[] PROGMEM = {KC_C, KC_COMM, COMBO_END};
combo_t key_combos[] = {
    // C and , => activate Caps Word.
    COMBO(caps_combo, CW_TOGG),
};
uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(*key_combos);

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    // Increase the tapping term a little for slower ring and pinky fingers.
    case HOME_A:
    case HOME_R:
    case HOME_S:
    case HOME_T:
    default:
      return TAPPING_TERM;
  }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case HOME_N:
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

#ifdef AUTOCORRECT_ENABLE
bool apply_autocorrect(uint8_t backspaces, const char* str,
                       char* typo, char* correct) {
  for (uint8_t i = 0; i < backspaces; ++i) {
    tap_code(KC_BSPC);
  }
  send_string_with_delay_P(str, TAP_CODE_DELAY);
  return false;
}
#endif  // AUTOCORRECT_ENABLE

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
    // These magic patterns work with Caps Word.
    case M_ION:
    case M_MENT:
    case M_QUEN:
    case M_TMENT:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                     uint16_t other_keycode, keyrecord_t* other_record) {
  // Exceptionally consider the following chords as holds, even though they
  // are on the same hand in Magic Sturdy.
  switch (tap_hold_keycode) {
		case LR_LOWER:
			return true;
		case LR_RAISE:
			return true;
		case LR_MAIN:
			return true;
		case LR_TMUX:
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
  return 800;  // Use a timeout of 800 ms.
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
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
  if ((mods & ~MOD_MASK_SHIFT) == 0) {
    switch (keycode) {
      // For navigating next/previous search results in Vim:
      // N -> Shift + N, Shift + N -> N.
      case HOME_N:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return S(KC_N);
        }
        // Fall through intended.
      case KC_N: return KC_N;

      // Behavior for Magic Sturdy's "magic" key.
      case HOME_A: return KC_O;        // A -> O
      case KC_C: return KC_Y;         // C -> Y
		  case KC_D: return KC_Y;         // D -> Y
      case HOME_E: return KC_U;       // E -> U
      case KC_G: return KC_Y;         // G -> Y
      case KC_L: return KC_K;         // L -> K
      case KC_M: return M_MENT;       // M -> ENT
      case KC_O: return KC_A;         // O -> A
      case KC_P: return KC_Y;         // P -> Y
      case KC_Q: return M_QUEN;       // Q -> UEN
      case HOME_R: return KC_L;       // R -> L
      case HOME_S: return KC_K;       // S -> K
      case HOME_T: return M_TMENT;    // T -> TMENT
      case KC_U: return KC_E;         // U -> E
      case KC_Y: return KC_P;         // Y -> P
      case KC_SPC: return M_THE;      // spc -> THE

      case KC_DOT: return M_UPDIR;    // . -> ./
      case KC_COMM:                   // ! -> ==
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return KC_NO;
        }
        // Fall through intended.
      case KC_EQL: return M_EQEQ;     // = -> ==
      case KC_HASH: return M_INCLUDE; // # -> include
      case KC_QUOT:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          return M_DOCSTR;  // " -> ""<cursor>"""
        }
        return KC_NO;
      case KC_GRV:  // ` -> ``<cursor>``` (for Markdown code)
        return M_MKGRVS;
      case KC_LABK: return KC_MINS;   // < -> - (for Haskell)
    }
  } else if ((mods & MOD_MASK_CTRL)) {
    switch (keycode) {
      case HOME_A:  // Ctrl+A -> Ctrl+C
        return C(KC_C);
    }
  }
  return KC_TRNS;
}
// clang-format on

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
  // Unpack tapping keycode for tap-hold keys.
  switch (keycode) {
#ifndef NO_ACTION_TAPPING
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
      keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
      break;
#ifndef NO_ACTION_LAYER
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
      break;
#endif  // NO_ACTION_LAYER
#endif  // NO_ACTION_TAPPING
  }

  // Forget Shift on letters when Shift or AltGr are the only mods.
  // Exceptionally, I want Shift remembered on N and Z for "NN" and "ZZ" in Vim.
  switch (keycode) {
    case KC_A ... KC_M:
    case KC_O ... KC_Y:
      if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        *remembered_mods &= ~MOD_MASK_SHIFT;
      }
      break;
  }

  return true;
}

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`.
#define MAGIC_STRING(str, repeat_keycode) \
  magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
  uint8_t saved_mods = 0;
  // If Caps Word is on, save the mods and hold Shift.
  if (is_caps_word_on()) {
    saved_mods = get_mods();
    register_mods(MOD_BIT(KC_LSFT));
  }

  send_string_with_delay_P(str, TAP_CODE_DELAY);  // Send the string.
  set_last_keycode(repeat_keycode);

  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}

typedef enum {
    PROCESS_RECORD_RETURN_TRUE,
    PROCESS_RECORD_RETURN_FALSE,
    PROCESS_RECORD_CONTINUE
} process_record_result_t;

// clang-format off
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_sentence_case(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  if (!process_custom_shift_keys(keycode, record)) { return false; }

  const uint8_t mods = get_mods();
  const bool shifted = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
                       ) & MOD_MASK_SHIFT;

  // If alt repeating a key A-Z with no mods other than Shift, set the last key
  // to KC_N. Above, alternate repeat of KC_N is defined to be again KC_N. This
  // way, either tapping alt repeat and then repeat (or double tapping alt
  // repeat) is useful to type certain patterns without SFBs:
  //
  //   O <altrep> <rep> -> OAN (as in "loan")
  //   D <altrep> <rep> -> DYN (as in "dynamic")
  if (get_repeat_key_count() < 0 &&
      KC_A <= keycode && keycode <= KC_Z && keycode != KC_N &&
      (get_last_mods() & ~MOD_MASK_SHIFT) == 0) {
    set_last_keycode(KC_N);
    set_last_mods(0);
  }

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
				// Macros invoked through the MAGIC key.
				case M_ION:     MAGIC_STRING(/*i*/"on", KC_S); break;
				case M_MENT:    MAGIC_STRING(/*m*/"ent", KC_S); break;
				case M_QUEN:    MAGIC_STRING(/*q*/"uen", KC_C); break;
				case M_TMENT:   MAGIC_STRING(/*t*/"ment", KC_S); break;
				case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
				case M_UPDIR:   MAGIC_STRING(/*.*/"./", UPDIR); break;
				case M_INCLUDE: SEND_STRING_DELAY(/*#*/"include ", TAP_CODE_DELAY); break;
				case M_EQEQ:    SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY); break;
				case M_DOCSTR:
					SEND_STRING_DELAY(/*"*/"\"\"\"\"\""
							SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
					break;
				case M_MKGRVS:
					SEND_STRING_DELAY(/*`*/"``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY);
					break;

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

        case MC_HARPOON_UI:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").toggle_quick_menu()");
            tap_code(KC_ENT);
            return PROCESS_RECORD_RETURN_FALSE;

        case MC_HARPOON_MARK:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.mark\").add_file()");
            tap_code(KC_ENT);
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

				// Harpoon
				case MC_HARPOON_NEXT:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_next()");
            tap_code(KC_ENT);
          return PROCESS_RECORD_RETURN_FALSE;

				case MC_HARPOON_PREV:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_prev()");
            tap_code(KC_ENT);
          return PROCESS_RECORD_RETURN_FALSE;

				case MC_HARPOON_GOTO_1:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_file(1)");
            tap_code(KC_ENT);
          return PROCESS_RECORD_RETURN_FALSE;

				case MC_HARPOON_GOTO_2:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_file(2)");
            tap_code(KC_ENT);
          return PROCESS_RECORD_RETURN_FALSE;

				case MC_HARPOON_GOTO_3:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_file(3)");
            tap_code(KC_ENT);
          return PROCESS_RECORD_RETURN_FALSE;

				case MC_HARPOON_GOTO_4:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_file(4)");
            tap_code(KC_ENT);
          return PROCESS_RECORD_RETURN_FALSE;

				case MC_HARPOON_GOTO_5:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" harpoon.ui\").nav_file(5)");
            tap_code(KC_ENT);
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

// RGB Matrix
int leds[] = {
     68, 67, 66, 65, 64, /*    */ 60, 59, 58, 57, 56,
     53, 52, 51 ,50, 49, /*    */ 45, 44, 43, 42, 41,
     38, 37, 36, 35, 34, /*    */ 30, 29, 28, 27, 26,
};

int layer_gamer_led[] = {12,};
int layer_base_led[] = {13,};
const int led_count = 30;

void set_rgblight_by_layer(uint32_t layer) {
     switch (layer) {
          case BASE:
	       rgb_matrix_set_color(13, RGB_RED);
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
	  case GAMER:
	       rgb_matrix_set_color(12, RGB_GREEN);
          default:
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
