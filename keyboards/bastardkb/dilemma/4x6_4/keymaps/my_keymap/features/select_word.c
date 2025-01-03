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

#include "select_word.h"

// Mac users, uncomment this line:
// #define MAC_HOTKEYS
// ^^^ THIS IS NO LONGER NEEDED. REMOVED OR COMMENTED OUT.

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

// Idle timeout timer to disable Select Word after a period of inactivity.
#if SELECT_WORD_TIMEOUT > 0
static uint16_t idle_timer = 0;

void select_word_task(void) {
    if (state && timer_expired(timer_read(), idle_timer)) {
        state = STATE_NONE;
    }
}
#endif // SELECT_WORD_TIMEOUT > 0

bool process_select_word(uint16_t keycode, keyrecord_t *record, uint16_t sel_keycode) {
    if (keycode == KC_LSFT || keycode == KC_RSFT) {
        return true;
    }

#if SELECT_WORD_TIMEOUT > 0
    idle_timer = record->event.time + SELECT_WORD_TIMEOUT;
#endif // SELECT_WORD_TIMEOUT > 0

    if (keycode == sel_keycode && record->event.pressed) { // On key press.
        const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
        const bool shifted = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
        clear_oneshot_mods();
#else
        const bool shifted = mods & MOD_MASK_SHIFT;
#endif // NO_ACTION_ONESHOT

        if (!shifted) {
            // Select word.
#ifdef OS_DETECTION_ENABLE
            os_variant_t host = detected_host_os();
            if (host == OS_MACOS || host == OS_IOS) {
                // macOS/iOS uses Left Alt (Option) for word jumps.
                set_mods(MOD_BIT(KC_LALT));
            } else {
                // Linux/Windows typically use Ctrl for word jumps.
                set_mods(MOD_BIT(KC_LCTL));
            }
#else
            // Default if OS detection is not enabled.
            set_mods(MOD_BIT(KC_LCTL));
#endif // OS_DETECTION_ENABLE

            if (state == STATE_NONE) {
                // On first use, tap Ctrl+Right then Ctrl+Left (or Alt if macOS) to
                // ensure the cursor is positioned at the beginning of the word.
                send_keyboard_report();
                tap_code(KC_RGHT);
                tap_code(KC_LEFT);
            }
            register_mods(MOD_BIT(KC_LSFT));
            register_code(KC_RGHT);
            state = STATE_WORD;

        } else {
            // Select line.
            if (state == STATE_NONE) {
#ifdef OS_DETECTION_ENABLE
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Tap GUI (Command) + Left, then Shift + GUI + Right.
                    set_mods(MOD_BIT(KC_LGUI));
                    send_keyboard_report();
                    tap_code(KC_LEFT);
                    register_mods(MOD_BIT(KC_LSFT));
                    tap_code(KC_RGHT);
                } else {
                    // Tap Home, then Shift + End.
                    clear_mods();
                    send_keyboard_report();
                    tap_code(KC_HOME);
                    register_mods(MOD_BIT(KC_LSFT));
                    tap_code(KC_END);
                }
#else
                // Default if OS detection is not enabled. (Home + Shift + End)
                clear_mods();
                send_keyboard_report();
                tap_code(KC_HOME);
                register_mods(MOD_BIT(KC_LSFT));
                tap_code(KC_END);
#endif // OS_DETECTION_ENABLE
                set_mods(mods);
                state = STATE_FIRST_LINE;
            } else {
                register_code(KC_DOWN);
                state = STATE_LINE;
            }
        }
        return false;
    }

    // `sel_keycode` was released, or another key was pressed.
    switch (state) {
        case STATE_WORD:
            unregister_code(KC_RGHT);
#ifdef OS_DETECTION_ENABLE
            {
                // Ensure we remove both Shift and whichever mod (Alt or Ctrl) was used.
                os_variant_t host = detected_host_os();
                unregister_mods(MOD_BIT(KC_LSFT));
                if (host == OS_MACOS || host == OS_IOS) {
                    unregister_mods(MOD_BIT(KC_LALT));
                } else {
                    unregister_mods(MOD_BIT(KC_LCTL));
                }
            }
#else
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
            if (keycode == KC_ESC) {
                tap_code(KC_RGHT);
                state = STATE_NONE;
                return false;
            }
            // Fallthrough intended.

        default:
            state = STATE_NONE;
    }

    return true;
}

#ifdef OS_DETECTION_ENABLE
uint32_t custom_os_settings(uint32_t trigger_time, void *cb_arg) {
    os_variant_t host     = detected_host_os();
    uint16_t     retry_ms = 500;

    // Automatically swap Ctrl and GUI if on macOS or iOS.
    if (host == OS_MACOS || host == OS_IOS) {
        keymap_config.swap_lctl_lgui = true;
        keymap_config.swap_rctl_rgui = true;
        // Setting retry_ms to 0 means no re-trigger is needed.
        retry_ms = 0;
    }

    return retry_ms;
}

void keyboard_post_init_user(void) {
    // Defer execution of custom_os_settings by 100ms to detect OS.
    defer_exec(100, custom_os_settings, NULL);
}
#endif // OS_DETECTION_ENABLE