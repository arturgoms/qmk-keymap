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
#include QMK_KEYBOARD_H

#include "features/achordion.h"
#include "features/custom_shift_keys.h"
#include "features/select_word.h"
#include "features/sentence_case.h"
#include "features/orbital_mouse.h"
#include "features/socd_cleaner.h"
#include "features/mouse_turbo_click.h"
#include "features/palettefx.h"
#include "os_detection.h"
#include "quantum.h"

enum layers {
    BASE,
    LOWER,
    RAISE,
    MAINTENANCE,
    TMUX,
    GAMER,
    BASE_PLUS,
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
    MC_SPLIT_HELPER,
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
    MC_HARPOON_ADD,
    MC_HARPOON_MENU,
    // Nvim macros (3+ keystroke sequences)
    MC_FIND_FILES,
    MC_GREP_TEXT,
    MC_LSP_FORMAT,
    MC_LSP_ACTION,
    MC_LSP_RENAME,
    MC_GIT_STAGE,
    MC_GIT_BLAME,
    // Tmux macros
    MC_TMUX_ZOOM,
    MC_TMUX_COPY_MODE,
    MC_TMUX_LAST_WINDOW,
    // Pane navigation (sends C-w then h/j/k/l)
    MC_PANE_LEFT,
    MC_PANE_DOWN,
    MC_PANE_UP,
    MC_PANE_RIGHT,
    // OS-aware word navigation (Alt on macOS, Ctrl on Win/Linux)
    OS_WORD_LEFT,
    OS_WORD_RIGHT,
    OS_DEL_WORD,
    OS_DEL_WORD_FWD,
    MC_SELECT_WORD,
    TURBO,
};

// Select Word keycode binding.
uint16_t SELECT_WORD_KEYCODE = SELWORD;

// SOCD Cleaner state for GAMER layer WASD.
socd_cleaner_t socd_v = {{KC_W, KC_S}, SOCD_CLEANER_LAST};
socd_cleaner_t socd_h = {{KC_A, KC_D}, SOCD_CLEANER_LAST};

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
#define MAGIC QK_AREP // The "magic" key is Alternate Repeat.

// This keymap uses home row mods. In addition to mods, I have home row
// layer-tap keys for the SYM layer. The key arrangement is a variation on
// "GASC-order" home row mods:
//
//             Left hand                          Right hand
// +-------+-------+-------+-------+   +-------+-------+-------+-------+
// |  Gui  |  Alt  | Shift | Ctrl  |   | Ctrl  | Shift |  Alt  |  Gui  |
// +-------+-------+-------+-------+   +-------+-------+-------+-------+

#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LSFT_T(KC_S)
#define HOME_T LCTL_T(KC_T)

#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_I LALT_T(KC_I)
#define HOME_O RGUI_T(KC_O)

#define LR_TMUX LT(TMUX, KC_TAB)
#define LR_RAISE LT(RAISE, KC_BSPC)
#define LR_LOWER LT(LOWER, KC_SPC)
#define LR_MAIN LT(MAINTENANCE, QK_REP)

#define MOD_CTEN OSM(MOD_LCTL)

#define KC_PPM KC_MEDIA_PLAY_PAUSE

// Tap Dance actions (empty array - tap dance is enabled but not currently used)
tap_dance_action_t tap_dance_actions[] = {};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*
     * BASE - Colemak with home row mods
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   '  | Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |GUI/A |ALT/R |SFT/S |CTL/T |   G  |-------.    ,-------|   M  |CTL/N |SFT/E |ALT/I |GUI/O | Ent  |
     * |------+------+------+------+------+------| Mute  |    | Play  |------+------+------+------+------+------|
     * |CpsLck|   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            | LAlt | LGui |TMUX/ |RAISE/| /OSM   /       \AREP  \ |LOWER/|MAINT/| GAMER| BASE+|
     *            |      |      | Tab  | Bspc |/ Ctrl /         \      \ | Spc  | Rep  |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [BASE] = LAYOUT(
        KC_GRV,       KC_1,     KC_2,     KC_3,      KC_4,       KC_5,                             KC_6,      KC_7,        KC_8,        KC_9,          KC_0,      KC_BSPC,
        KC_TAB,       KC_Q,     KC_W,     KC_F,      KC_P,       KC_B,                             KC_J,      KC_L,        KC_U,        KC_Y,          KC_QUOT,   KC_DEL,
        KC_ESC,       HOME_A,   HOME_R,   HOME_S,    HOME_T,     KC_G,                             KC_M,      HOME_N,      HOME_E,      HOME_I,        HOME_O,    KC_ENT,
        KC_CAPS_LOCK, KC_Z,     KC_X,     KC_C,      KC_D,       KC_V,      KC_MUTE,   KC_MPLY,   KC_K,      KC_H,        KC_COMM,     KC_DOT,        KC_SLSH,   KC_RSFT,
                                KC_LALT,  KC_LGUI,   LR_TMUX,    LR_RAISE,  MOD_CTEN,  QK_AREP,   LR_LOWER,  LR_MAIN,     DF(GAMER), DF(BASE_PLUS)
    ),

    /*
     * LOWER - Symbols (left) + Navigation Cluster (right)
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   |   1  |   2  |   3  |   4  |   5  |                    |C-Left|C-Down| C-Up |C-Rght|   0  |W-Bspc|
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  | ::   |SELWRD|   ]  |   )  |   }  |                    |A-Lft | Home |  End |A-Rgt | PgUp | Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |   =  |   |  |   [  |   (  |   {  |-------.    ,-------| Left | Down |  Up  | Right| PgDn | Ent  |
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LShift|   \  |   /  |   +  |   ;  |   -  |-------|    |-------|   _  |   :  |   =  |      |   /  |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            |      |      |      |      |/       /       \      \ |      |      |      |      |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [LOWER] = LAYOUT(
        KC_GRV,   KC_1,     KC_2,          KC_3,           KC_4,           KC_5,                                LCTL(KC_LEFT),    LCTL(KC_DOWN), LCTL(KC_UP), LCTL(KC_RGHT), KC_0,      OS_DEL_WORD,
        KC_TAB,   SCOPE,    SELWORD,       KC_RBRC,        KC_RPRN,        LSFT(KC_RBRC),                       OS_WORD_LEFT,     KC_HOME,     KC_END,     OS_WORD_RIGHT,  KC_PGUP,    KC_DEL,
        KC_ESC,   KC_EQL,   KC_PIPE,       KC_LBRC,        KC_LPRN,        LSFT(KC_LBRC),                       KC_LEFT,          KC_DOWN,     KC_UP,      KC_RGHT,        KC_PGDN,    KC_ENT,
        KC_LSFT,  KC_BSLS,  KC_SLSH,       LSFT(KC_EQL),   KC_SCLN,        KC_MINS,       _______,  _______,   KC_UNDS,          KC_COLN,     KC_EQL,     _______,         KC_SLSH,   KC_RSFT,
                                KC_LALT,  _______,        _______,        _______,       _______,  _______,   _______,          _______,     _______,    _______
    ),

    /*
     * RAISE - Pane/Buffer Navigation + Editor + Nvim Macros
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   |Harp1 |Harp2 |Harp3 |Harp4 |Harp5 |                    |      |      |      |      |      |W-Del |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  | Quit |      | Split|FindFl| Grep |                    | S-H  | C-D  | C-U  | S-L  |      | Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |      |LSPFmt|LSPAct|LSPRen| CPR  |-------.    ,-------|CwPane|CwPane|CwPane|CwPane|      | Ent  |
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LShift|GitStg|GitBlm| viw  | diw  | Save |-------|    |-------|HrpMnu|HrpPrv|HrpNxt|HrpAdd|      |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            |      |      |      |      |/       /       \      \ |      |      |      |      |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [RAISE] = LAYOUT(
        KC_GRV,   MC_HARPOON_GOTO_1, MC_HARPOON_GOTO_2, MC_HARPOON_GOTO_3, MC_HARPOON_GOTO_4, MC_HARPOON_GOTO_5,                     _______,       _______,          _______,       _______,       _______,       OS_DEL_WORD_FWD,
        KC_TAB,   MC_QUIT,           _______,           MC_SPLIT_HELPER,   MC_FIND_FILES,     MC_GREP_TEXT,                            LSFT(KC_H),    LCTL(KC_D),       LCTL(KC_U),    LSFT(KC_L),    _______,       KC_DEL,
        KC_ESC,   _______,           MC_LSP_FORMAT,     MC_LSP_ACTION,     MC_LSP_RENAME,     MC_CPR,                                  MC_PANE_LEFT,  MC_PANE_DOWN,     MC_PANE_UP,    MC_PANE_RIGHT, _______,       KC_ENT,
        KC_LSFT,  MC_GIT_STAGE,      MC_GIT_BLAME,      MC_SELECT_WORD,    MC_DELETE_WORD,    MC_SAVE,           _______,  _______,   MC_HARPOON_MENU, MC_HARPOON_PREV, MC_HARPOON_NEXT, MC_HARPOON_ADD, _______,    KC_RSFT,
                                KC_LALT,  _______,           _______,           _______,           _______,  _______,   _______,       _______,          _______,       _______
    ),

    /*
     * MAINTENANCE - System, Orbital Mouse, RGB
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   | Boot |      |      |      |      |                    |      |      |      |      |      | Bspc |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  | Exit |RGB V-|RGB V+|RGB M+|QK_LLC|                    |OM_W_U|OM_BTN|OM_U  |OM_BT2| TURBO| Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |      |RGB H-|RGB H+|      |      |-------.    ,-------|OM_W_D|OM_L  |OM_D  |OM_R  |OM_SLW| Ent  |
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LShift|      |RGB S-|RGB S+|      |      |-------|    |-------|      |OM_BT3|      |      |      |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            |      |      |      |      |/       /       \      \ |      |      |      |      |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [MAINTENANCE] = LAYOUT(
        KC_GRV,   QK_BOOT,  _______,  _______,  _______,  _______,                        _______,  _______,  _______,  _______,  _______,  KC_BSPC,
        KC_TAB,   EXIT,     RM_VALD,  RM_VALU,  RM_NEXT,  QK_LLCK,                        OM_W_U,   OM_BTNS,  OM_U,     OM_BTN2,  TURBO,    KC_DEL,
        KC_ESC,   _______,  RM_HUED,  RM_HUEU,  _______,  _______,                        OM_W_D,   OM_L,     OM_D,     OM_R,     OM_SLOW,  KC_ENT,
        KC_LSFT,  _______,  RM_SATD,  RM_SATU,  _______,  _______,  _______,    _______,  _______,  OM_BTN3,  _______,  _______,  _______,  KC_RSFT,
                                KC_LALT,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______, _______
    ),

    /*
     * TMUX - Tmux Session Management
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   |      |      |      |      |      |                    |      |      |      |      |      | Bspc |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |KillSe|KillPa| Prev | Next | ChSh |                    |Sessiz|LastWn|Instl |      | Zoom | Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |Reload|Restor| Save | New  |Rename|-------.    ,-------|  Left| Down |  Up  | Right|CopyMd| Ent  |
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LShift|      |      |Sessio|Detach|SplitV|-------|    |-------|      |SplitH|      |      |      |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            |      |      |      |      |/       /       \      \ |      |      |      |      |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [TMUX] = LAYOUT(
        KC_GRV,   _______,              _______,           _______,        _______,        _______,                                       _______,                 _______,              _______,            _______,                  _______,  KC_BSPC,
        KC_TAB,   MC_TMUX_KILL_SESSION, MC_TMUX_KILL_PANE, MC_TMUX_PREV,   MC_TMUX_NEXT,   MC_TMUX_CHSH,                                  MC_TMUX_SESSIONIZER,     MC_TMUX_LAST_WINDOW,  MC_TMUX_INSTALL,    _______,                  MC_TMUX_ZOOM,  KC_DEL,
        KC_ESC,   MC_TMUX_RELOAD,       MC_TMUX_RESTORE,   MC_TMUX_SAVE,   MC_TMUX_NEW,    MC_TMUX_RENAME,                                MC_TMUX_SWITCH_LEFT,     MC_TMUX_SWITCH_DOWN,  MC_TMUX_SWITCH_UP,  MC_TMUX_SWITCH_RIGHT,     MC_TMUX_COPY_MODE,  KC_ENT,
        KC_LSFT,  _______,              _______,           MC_TMUX_SESSIONS, MC_TMUX_DETACH, MC_TMUX_SPLIT_V, _______,         _______,  _______,                 MC_TMUX_SPLIT_H,      _______,            _______,                  _______,  KC_RSFT,
                                KC_LALT,  _______,        _______,        _______,         _______,         _______,  _______,                 _______,              _______, _______
    ),

    /*
     * GAMER - QWERTY for Gaming
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  | Ent  |
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            | LAlt | Space| LCtrl|RAISE/| /LCtrl /       \AREP  \ |LOWER/|MAINT/| BASE+| BASE |
     *            |      |      |      | Bspc |/      /         \      \ | Spc  | Rep  |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [GAMER] = LAYOUT(
        KC_GRV,   KC_1,     KC_2,     KC_3,      KC_4,       KC_5,                             KC_6,      KC_7,      KC_8,        KC_9,        KC_0,      KC_BSPC,
        KC_TAB,   KC_Q,     KC_W,     KC_E,      KC_R,       KC_T,                             KC_Y,      KC_U,      KC_I,        KC_O,        KC_P,      KC_DEL,
        KC_ESC,   KC_A,     KC_S,     KC_D,      KC_F,       KC_G,                             KC_H,      KC_J,      KC_K,        KC_L,        KC_SCLN,   KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,      KC_V,       KC_B,      _______,    _______,   KC_N,      KC_M,      KC_COMM,     KC_DOT,      KC_SLSH,   KC_RSFT,
                                KC_LALT,  KC_SPC,     KC_LCTL,   LR_RAISE,   KC_LCTL,   QK_AREP,   LR_LOWER,  LR_MAIN,     DF(BASE_PLUS), DF(BASE)
    ),

    /*
     * BASE_PLUS - Colemak with partial home row mods (right hand only)
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   '  | Del  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  |   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |CTL/N |SFT/E |ALT/I |GUI/O | Ent  |
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LShift|   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |RShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            | LAlt | LGui |TMUX/ |RAISE/| /LCtrl /       \AREP  \ |LOWER/|MAINT/| GAMER| BASE |
     *            |      |      | Tab  | Bspc |/      /         \      \ | Spc  | Rep  |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [BASE_PLUS] = LAYOUT(
        KC_GRV,   KC_1,     KC_2,     KC_3,      KC_4,       KC_5,                             KC_6,      KC_7,      KC_8,        KC_9,        KC_0,      KC_BSPC,
        KC_TAB,   KC_Q,     KC_W,     KC_F,      KC_P,       KC_B,                             KC_J,      KC_L,      KC_U,        KC_Y,        KC_QUOT,   KC_DEL,
        KC_ESC,   KC_A,     KC_R,     KC_S,      KC_T,       KC_G,                             KC_M,      HOME_N,    HOME_E,      HOME_I,      HOME_O,    KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,      KC_D,       KC_V,      _______,    _______,   KC_K,      KC_H,      KC_COMM,     KC_DOT,      KC_SLSH,   KC_RSFT,
                                KC_LALT,  KC_LGUI,    LR_TMUX,   LR_RAISE,   KC_LCTL,   QK_AREP,   LR_LOWER,  LR_MAIN,     DF(GAMER), DF(BASE)
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE]        = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU),          ENCODER_CCW_CW(KC_MPRV, KC_MNXT)},
    [LOWER]       = {ENCODER_CCW_CW(OS_WORD_LEFT, OS_WORD_RIGHT), ENCODER_CCW_CW(KC_PGUP, KC_PGDN)},
    [RAISE]       = {ENCODER_CCW_CW(LSFT(KC_H), LSFT(KC_L)),  ENCODER_CCW_CW(LCTL(KC_U), LCTL(KC_D))},
    [MAINTENANCE] = {ENCODER_CCW_CW(RM_VALD, RM_VALU),         ENCODER_CCW_CW(RM_PREV, RM_NEXT)},
    [TMUX]        = {ENCODER_CCW_CW(MC_TMUX_PREV, MC_TMUX_NEXT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [GAMER]       = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU),          ENCODER_CCW_CW(KC_MPRV, KC_MNXT)},
    [BASE_PLUS]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU),          ENCODER_CCW_CW(KC_MPRV, KC_MNXT)},
};
#endif

const uint32_t unicode_map[] PROGMEM = {};

const custom_shift_key_t custom_shift_keys[] = {
};
uint8_t NUM_CUSTOM_SHIFT_KEYS = sizeof(custom_shift_keys) / sizeof(*custom_shift_keys);

const uint16_t caps_combo[] PROGMEM = {KC_C, KC_COMM, COMBO_END};
combo_t        key_combos[]         = {
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
            return 0; // Otherwise, force hold and disable key repeating.
    }
}

#ifdef AUTOCORRECT_ENABLE
bool apply_autocorrect(uint8_t backspaces, const char* str, char* typo, char* correct) {
    for (uint8_t i = 0; i < backspaces; ++i) {
        tap_code(KC_BSPC);
    }
    send_string_with_delay_P(str, TAP_CODE_DELAY);
    return false;
}
#endif // AUTOCORRECT_ENABLE

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to the next key.
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
            return false; // Deactivate Caps Word.
    }
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
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
    return 800; // Use a timeout of 800 ms.
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

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
    // Unpack tapping keycode for tap-hold keys.
    switch (keycode) {
#ifndef NO_ACTION_TAPPING
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
            break;
#    ifndef NO_ACTION_LAYER
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
            break;
#    endif // NO_ACTION_LAYER
#endif     // NO_ACTION_TAPPING
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
#define MAGIC_STRING(str, repeat_keycode) magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
    uint8_t saved_mods = 0;
    // If Caps Word is on, save the mods and hold Shift.
    if (is_caps_word_on()) {
        saved_mods = get_mods();
        register_mods(MOD_BIT(KC_LSFT));
    }

    send_string_with_delay_P(str, TAP_CODE_DELAY); // Send the string.
    set_last_keycode(repeat_keycode);

    // If Caps Word is on, restore the mods.
    if (is_caps_word_on()) {
        set_mods(saved_mods);
    }
}

// clang-format off
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  // 1. SOCD Cleaner (gaming input filtering)
  if (!process_socd_cleaner(keycode, record, &socd_v)) { return false; }
  if (!process_socd_cleaner(keycode, record, &socd_h)) { return false; }
  // 2. Orbital Mouse
  if (!process_orbital_mouse(keycode, record)) { return false; }
  // 3. Achordion
  if (!process_achordion(keycode, record)) { return false; }
  // 4. Sentence Case
  if (!process_sentence_case(keycode, record)) { return false; }
  // 5. Select Word
  if (!process_select_word(keycode, record)) { return false; }
  // 6. Custom Shift Keys
  if (!process_custom_shift_keys(keycode, record)) { return false; }
  // 7. Mouse Turbo Click
  if (!process_mouse_turbo_click(keycode, record, TURBO)) { return false; }

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

  // OS-aware word navigation: Alt on macOS, Ctrl on Win/Linux.
  {
    uint16_t mod = (detected_host_os() == OS_MACOS) ? KC_LALT : KC_LCTL;
    switch (keycode) {
      case OS_WORD_LEFT:
        if (record->event.pressed) { register_code(mod); register_code(KC_LEFT); }
        else { unregister_code(KC_LEFT); unregister_code(mod); }
        return false;
      case OS_WORD_RIGHT:
        if (record->event.pressed) { register_code(mod); register_code(KC_RGHT); }
        else { unregister_code(KC_RGHT); unregister_code(mod); }
        return false;
      case OS_DEL_WORD:
        if (record->event.pressed) { register_code(mod); register_code(KC_BSPC); }
        else { unregister_code(KC_BSPC); unregister_code(mod); }
        return false;
      case OS_DEL_WORD_FWD:
        if (record->event.pressed) { register_code(mod); register_code(KC_DEL); }
        else { unregister_code(KC_DEL); unregister_code(mod); }
        return false;
    }
  }

  if (record->event.pressed) {
    switch (keycode) {
        case EXIT:
        layer_off(MAINTENANCE);
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
        SEND_STRING(SS_LGUI("ct") SS_DELAY(100) SS_LGUI("v") SS_TAP(X_ENTER));
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
            return false;

        case MC_COMMENT:
            tap_code(KC_ESCAPE);
            SEND_STRING(" /");
            return false;

        case MC_SELECT_WORD:
            tap_code(KC_V);
            tap_code(KC_I);
            tap_code(KC_W);
            return false;

        case MC_DELETE_WORD:
            tap_code(KC_D);
            tap_code(KC_I);
            tap_code(KC_W);
            return false;

        case MC_SAVE:
            tap_code(KC_ESCAPE);
            SEND_STRING(":w");
            tap_code(KC_ENT);
            return false;

        case MC_QUIT:
            tap_code(KC_ESCAPE);
            SEND_STRING(":q");
            tap_code(KC_ENT);
            return false;

        case MC_VISTA:
            tap_code(KC_ESCAPE);
            SEND_STRING(":Vista!!");
            tap_code(KC_ENT);
            return false;

        case MC_NEXT_TAB:
            tap_code(KC_ESCAPE);
            SEND_STRING(":tabnext");
            tap_code(KC_ENT);
            return false;

        case MC_PREV_TAB:
            tap_code(KC_ESCAPE);
            SEND_STRING(":tabprevious");
            tap_code(KC_ENT);
            return false;

        case MC_BUFFERS:
            tap_code(KC_ESCAPE);
            SEND_STRING(":lua require(\" user.bfs\").open()");
            tap_code(KC_ENT);
            return false;

        case MC_SPLIT_HELPER:
            tap_code16(LGUI(KC_K));
            return false;

        // Tmux

        case MC_TMUX_NEXT:
            tap_code16(LCTL(KC_B));
            SEND_STRING("n");
            return false;

        case MC_TMUX_PREV:
            tap_code16(LCTL(KC_B));
            SEND_STRING("p");
            return false;

        case MC_TMUX_SPLIT_H:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_H);
            return false;

        case MC_TMUX_SPLIT_V:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_V);
            return false;

        case MC_TMUX_RELOAD:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_R);
            return false;

        case MC_TMUX_RENAME:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_COMM);
            return false;

        case MC_TMUX_SWITCH_UP:
            tap_code16(LCTL(KC_B));
            tap_code(KC_UP);
            return false;

        case MC_TMUX_SWITCH_DOWN:
            tap_code16(LCTL(KC_B));
            tap_code(KC_DOWN);
            return false;

        case MC_TMUX_SWITCH_LEFT:
            tap_code16(LCTL(KC_B));
            tap_code(KC_LEFT);
            return false;

        case MC_TMUX_SWITCH_RIGHT:
            tap_code16(LCTL(KC_B));
            tap_code(KC_RGHT);
            return false;

        case MC_TMUX_KILL_SESSION:
            tap_code16(LCTL(KC_B));
            tap_code(KC_Q);
            return false;

        case MC_TMUX_KILL_PANE:
            tap_code16(LCTL(KC_B));
            tap_code(KC_W);
            return false;

        case MC_TMUX_NEW:
            tap_code16(LCTL(KC_B));
            tap_code(KC_C);
            return false;

        case MC_TMUX_INSTALL:
            tap_code16(LCTL(KC_B));
            tap_code16(LSFT(KC_U));
            return false;

        case MC_TMUX_DETACH:
            tap_code16(LCTL(KC_B));
            tap_code(KC_D);
            return false;

        case MC_TMUX_SAVE:
            tap_code16(LCTL(KC_B));
            tap_code16(LCTL(KC_S));
            return false;

        case MC_TMUX_CHSH:
            tap_code16(LCTL(KC_B));
            tap_code16(KC_I);
            return false;

        case MC_TMUX_SESSIONIZER:
            tap_code16(LCTL(KC_B));
            tap_code(KC_F);
            return false;

        case MC_TMUX_SESSIONS:
            tap_code16(LCTL(KC_B));
            tap_code(KC_S);
            return false;

        case MC_TMUX_RESTORE:
              tap_code16(LCTL(KC_B));
              tap_code16(LCTL(KC_R));
              return false;

        case MC_TMUX_ZOOM:
            tap_code16(LCTL(KC_B));
            tap_code(KC_Z);
            return false;

        case MC_TMUX_COPY_MODE:
            tap_code16(LCTL(KC_B));
            tap_code(KC_LBRC);
            return false;

        case MC_TMUX_LAST_WINDOW:
            tap_code16(LCTL(KC_B));
            tap_code(KC_L);
            return false;

        // Pane navigation (C-w + direction)
        case MC_PANE_LEFT:
            tap_code16(LCTL(KC_W));
            tap_code(KC_H);
            return false;

        case MC_PANE_DOWN:
            tap_code16(LCTL(KC_W));
            tap_code(KC_J);
            return false;

        case MC_PANE_UP:
            tap_code16(LCTL(KC_W));
            tap_code(KC_K);
            return false;

        case MC_PANE_RIGHT:
            tap_code16(LCTL(KC_W));
            tap_code(KC_L);
            return false;

        // Nvim macros (3+ keystroke sequences)
        case MC_FIND_FILES:
            tap_code(KC_ESCAPE);
            SEND_STRING(" ff");
            return false;

        case MC_GREP_TEXT:
            tap_code(KC_ESCAPE);
            SEND_STRING(" ft");
            return false;

        case MC_LSP_FORMAT:
            tap_code(KC_ESCAPE);
            SEND_STRING(" lf");
            return false;

        case MC_LSP_ACTION:
            tap_code(KC_ESCAPE);
            SEND_STRING(" la");
            return false;

        case MC_LSP_RENAME:
            tap_code(KC_ESCAPE);
            SEND_STRING(" ln");
            return false;

        case MC_GIT_STAGE:
            tap_code(KC_ESCAPE);
            SEND_STRING(" gs");
            return false;

        case MC_GIT_BLAME:
            tap_code(KC_ESCAPE);
            SEND_STRING(" gb");
            return false;

        // Harpoon (uses nvim leader shortcuts)
        case MC_HARPOON_NEXT:
            tap_code(KC_ESCAPE);
            tap_code(KC_DOT);
            return false;

        case MC_HARPOON_PREV:
            tap_code(KC_ESCAPE);
            tap_code(KC_COMM);
            return false;

        case MC_HARPOON_GOTO_1:
            tap_code(KC_ESCAPE);
            SEND_STRING(" h1");
            return false;

        case MC_HARPOON_GOTO_2:
            tap_code(KC_ESCAPE);
            SEND_STRING(" h2");
            return false;

        case MC_HARPOON_GOTO_3:
            tap_code(KC_ESCAPE);
            SEND_STRING(" h3");
            return false;

        case MC_HARPOON_GOTO_4:
            tap_code(KC_ESCAPE);
            SEND_STRING(" h4");
            return false;

        case MC_HARPOON_GOTO_5:
            tap_code(KC_ESCAPE);
            SEND_STRING(" h5");
            return false;

        case MC_HARPOON_ADD:
            tap_code(KC_ESCAPE);
            SEND_STRING(" ha");
            return false;

        case MC_HARPOON_MENU:
            tap_code(KC_ESCAPE);
            SEND_STRING(" hh");
            return false;
    }
  }

  return true;
}

void housekeeping_task_user(void) {
  achordion_task();
  select_word_task();
  sentence_case_task();
  orbital_mouse_task();
}

// RGB Matrix - Status indicator LEDs (15 and 16).
// PaletteFx handles the base RGB effect; we only overlay status indicators.
bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    switch (layer) {
        case BASE:
            rgb_matrix_set_color(15, RGB_RED);
            rgb_matrix_set_color(16, RGB_BLUE);
            break;
        case GAMER:
            rgb_matrix_set_color(15, RGB_BLUE);
            rgb_matrix_set_color(16, RGB_GREEN);
            break;
        case BASE_PLUS:
            rgb_matrix_set_color(15, RGB_RED);
            rgb_matrix_set_color(16, RGB_GREEN);
            break;
        default:
            break;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Enable SOCD cleaner only on GAMER layer.
    socd_cleaner_enabled = IS_LAYER_ON_STATE(state, GAMER);
    return state;
}

void keyboard_post_init_user(void) {
    // RGB mode is persisted in EEPROM automatically.
    // Default mode is set via RGB_MATRIX_DEFAULT_MODE in config.h.
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,240,248,252,254,255,255,255,199,131,255,255,255,255,255,254,124,  0,  0,  0,  0,  0,  0,  0,254,255,239,  3,  7, 63, 62, 60,  0,  0,252,254,255,  7,  7,255,254,252,  0,  0,255,255,255,  0,  0,255,255,255,  0,  0,255,255,255,254,240,192,255,255,  0,  0,  7,  7,  7,255,255,255,  7,  7,  0,  0,255,255,255,199,199,199,  7,  0,  0,255,255,255,199,199,255,254,124,  0,  0,255,255,255,199,199,255,254,124,  0,  0,248,255, 63,255,255,224,  0,  0,  0,255,255,255,199,199,255,254,124,  0,  7,  7,  7,255,255,255,  7,
        7,  7, 15, 31, 63,127,127,127,127,127,  1,  3,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0, 63,127,127, 96,112,126, 62, 30,  0,  0, 31,127,127,112,112,127, 63, 31,  0,  0, 31, 63,127,112, 96,127,127, 31,  0,  0,127,127,127,  0, 15,127,127,127,  0,  0,  0,  0,  0,127,127,127,  0,  0,  0,  0,127,127,127,112,113,112,112,  0,  0,127,127,127,  1, 31,127,126, 96,  0,  0,127,127,127,  1,  1,  1,  0,  0, 96,127,127, 15, 14, 14, 63,127,124, 64,  0,127,127,127,  1, 31,127,124, 96,  0,  0,  0,  0,127,127,127,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

static void render_status(void) {
    // Line 1: Layer name
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    switch (layer) {
        case BASE:        oled_write_P(PSTR("BASE\n"), false); break;
        case LOWER:       oled_write_P(PSTR("LOWER\n"), false); break;
        case RAISE:       oled_write_P(PSTR("RAISE\n"), false); break;
        case MAINTENANCE: oled_write_P(PSTR("MAINT\n"), false); break;
        case TMUX:        oled_write_P(PSTR("TMUX\n"), false); break;
        case GAMER:       oled_write_P(PSTR("GAMER\n"), false); break;
        case BASE_PLUS:   oled_write_P(PSTR("BASE+\n"), false); break;
        default:          oled_write_P(PSTR("???\n"), false); break;
    }

    // Line 2: Active modifiers (GASC = Gui, Alt, Shift, Ctrl)
    uint8_t mods = get_mods() | get_oneshot_mods();
    oled_write_P((mods & MOD_MASK_GUI)   ? PSTR("GUI ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_ALT)   ? PSTR("ALT ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SFT ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_CTRL)  ? PSTR("CTL\n") : PSTR("   \n"), false);

    // Line 3: Status indicators
    oled_write_P(is_caps_word_on() ? PSTR("CW ") : PSTR("   "), false);
    oled_write_P(is_layer_locked(layer) ? PSTR("LOCK ") : PSTR("     "), false);
    oled_write_P(socd_cleaner_enabled ? PSTR("SOCD\n") : PSTR("    \n"), false);

    // Line 4: WPM
    oled_write_P(PSTR("WPM: "), false);
    oled_write(get_u8_str(get_current_wpm(), ' '), false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();
    } else {
        render_logo();
    }
    return false;
}

#endif
