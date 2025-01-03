/**************************************************************************************
 *  Existing GPL License Header from your keymap (unchanged)
 **************************************************************************************/
/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#ifdef OS_DETECTION_ENABLE
uint32_t custom_os_settings(uint32_t trigger_time, void *cb_arg) {
    os_variant_t host     = detected_host_os();
    uint16_t     retry_ms = 500;

    if (host == OS_MACOS || host == OS_IOS) {
        keymap_config.swap_lctl_lgui = true;
        keymap_config.swap_rctl_rgui = true;
        retry_ms                     = 0;
    }

    return retry_ms;
}

void keyboard_post_init_user(void) {
    defer_exec(100, custom_os_settings, NULL);
}
#endif

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_QWERTY,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
};

// Automatically enable sniping-mode on the pointer layer.
// #define DILEMMA_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define POINT MO(LAYER_POINTER)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
       KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_LGUI,
                         KC_LALT, KC_BSPC,  KC_SPC,   LOWER,      RAISE,  KC_ENT, KC_DEL,  KC_MUTE
  ),

  [LAYER_QWERTY] = LAYOUT(
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
       KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_LGUI,
                         KC_LALT, KC_BSPC,  KC_SPC,   LOWER,      RAISE,  KC_ENT, KC_DEL,  KC_MUTE
  ),

  [LAYER_LOWER] = LAYOUT(
       KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
       RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_LBRC,   KC_P7,   KC_P8,   KC_P9, KC_RBRC, XXXXXXX,
       RGB_TOG, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PPLS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_PEQL,
      RGB_RMOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PAST,   KC_P1,   KC_P2,   KC_P3, KC_PSLS, KC_PDOT,
                         XXXXXXX, XXXXXXX, XXXXXXX, _______,      POINT, _______, XXXXXXX,   KC_P0
  ),

  [LAYER_RAISE] = LAYOUT(
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
       KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
                         XXXXXXX, _______, _______,   POINT,    _______, XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [LAYER_POINTER] = LAYOUT(
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
       XXXXXXX, _______, DRGSCRL, SNIPING, EE_CLR,  QK_BOOT,    QK_BOOT, EE_CLR,  SNIPING, DRGSCRL, _______, XXXXXXX,
                         XXXXXXX, KC_BTN2, KC_BTN1, KC_BTN3,    KC_BTN3, KC_BTN1, KC_BTN2, XXXXXXX
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_enable(true); // always required before the auto mouse feature will work
}
#    endif // POINTING_DEVICE_AUTO_MOUSE_ENABLE
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif // RGB_MATRIX_ENABLE

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_QWERTY]     = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_LOWER]      = {ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SAD, RGB_SAI)},
    [LAYER_RAISE]      = {ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_HOME, KC_END)},
    [LAYER_POINTER]    = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE

/**************************************************************************************
 *  Add the Google Apache-licensed Select Word code below, adjusted for OS detection
 **************************************************************************************/
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
 * @file select_word.c
 * @brief Select word implementation
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/select-word>
 */

// If you extracted select_word.h somewhere, include it here:
// #include "select_word.h"

#ifndef SELECT_WORD_TIMEOUT
#    define SELECT_WORD_TIMEOUT 5000 // Example default timeout (ms)
#endif

// clang-format off
enum {
    STATE_NONE,        // No selection.
    STATE_SELECTED,    // Macro released with something selected.
    STATE_WORD,        // Macro held with word(s) selected.
    STATE_FIRST_LINE,  // Macro held with one line selected.
    STATE_LINE         // Macro held with multiple lines selected.
};
// clang-format on

static uint8_t state = STATE_NONE;

#if SELECT_WORD_TIMEOUT > 0
static uint16_t idle_timer = 0;

void select_word_task(void) {
    if (state && timer_expired(timer_read(), idle_timer)) {
        state = STATE_NONE;
    }
}
#endif // SELECT_WORD_TIMEOUT > 0

/**
 * Example usage in your process_record_user():
 *
 *   bool process_record_user(uint16_t keycode, keyrecord_t* record) {
 *     if (!process_select_word(keycode, record, SELWORD)) {
 *         return false;
 *     }
 *     return true;
 *   }
 */
bool process_select_word(uint16_t keycode, keyrecord_t *record, uint16_t sel_keycode) {
    // Don’t interfere with shift-taps.
    if (keycode == KC_LSFT || keycode == KC_RSFT) {
        return true;
    }

#if SELECT_WORD_TIMEOUT > 0
    // Reset idle timer every time we see a relevant key event.
    idle_timer = record->event.time + SELECT_WORD_TIMEOUT;
#endif

    // If the key pressed is our "select word" key:
    if (keycode == sel_keycode && record->event.pressed) {
        const uint8_t mods = get_mods();

#ifndef NO_ACTION_ONESHOT
        const bool shifted = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
        clear_oneshot_mods();
#else
        const bool shifted = mods & MOD_MASK_SHIFT;
#endif

        // Detect OS for appropriate key combos
#ifdef OS_DETECTION_ENABLE
        os_variant_t host     = detected_host_os();
        bool         is_macos = (host == OS_MACOS || host == OS_IOS);
#else
        bool is_macos = false; // fallback if OS detection is off
#endif

        if (!shifted) {
            // ----- Select word -----
            // On macOS, hold Left Alt (Option).
            // Otherwise, hold Left Ctrl.
            if (is_macos) {
                set_mods(MOD_BIT(KC_LALT));
            } else {
                set_mods(MOD_BIT(KC_LCTL));
            }

            if (state == STATE_NONE) {
                // On first use, tap Ctrl+Right then Ctrl+Left
                // (or Alt+Right then Alt+Left on macOS)
                send_keyboard_report();
                tap_code(KC_RGHT);
                tap_code(KC_LEFT);
            }
            register_mods(MOD_BIT(KC_LSFT));
            register_code(KC_RGHT);
            state = STATE_WORD;

        } else {
            // ----- Select line -----
            if (state == STATE_NONE) {
                if (is_macos) {
                    // Tap GUI(Command)+Left, then SHIFT+GUI(Command)+Right
                    // 1) Press Command
                    set_mods(MOD_BIT(KC_LGUI));
                    send_keyboard_report();
                    tap_code(KC_LEFT);

                    // 2) Press Shift+Command+Right
                    register_mods(MOD_BIT(KC_LSFT));
                    tap_code(KC_RGHT);

                    // Restore previous mods
                    set_mods(mods);
                } else {
                    // Tap Home, then SHIFT+End
                    clear_mods();
                    send_keyboard_report();
                    tap_code(KC_HOME);
                    register_mods(MOD_BIT(KC_LSFT));
                    tap_code(KC_END);

                    // Restore previous mods
                    set_mods(mods);
                }
                state = STATE_FIRST_LINE;
            } else {
                // Already selecting a line, go down one more line
                register_code(KC_DOWN);
                state = STATE_LINE;
            }
        }
        return false;
    }

    // If select_word is being released, or any other key pressed
    switch (state) {
        case STATE_WORD:
            unregister_code(KC_RGHT);
#ifdef OS_DETECTION_ENABLE
            {
                os_variant_t host     = detected_host_os();
                bool         is_macos = (host == OS_MACOS || host == OS_IOS);
                if (is_macos) {
                    unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT));
                } else {
                    unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));
                }
            }
#else
            // fallback
            unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));
#endif
            state = STATE_SELECTED;
            break;

        case STATE_FIRST_LINE:
            state = STATE_SELECTED;
            break;

        case STATE_LINE:
            unregister_code(KC_DOWN);
            state = STATE_SELECTED;
            break;

        case STATE_SELECTED:
            // If user hits Esc, we move cursor to the right
            // to break the selection
            if (keycode == KC_ESC) {
                tap_code(KC_RGHT);
                state = STATE_NONE;
                return false;
            }
            // fall-through
        default:
            state = STATE_NONE;
            break;
    }

    return true;
}