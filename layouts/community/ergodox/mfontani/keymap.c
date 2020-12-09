#include <stdarg.h>
#include QMK_KEYBOARD_H
#ifdef MFONTANI_UPRINTF
#include "debug.h"
#endif
#include "action_layer.h"
#include "version.h"

/* OS Identifier */
enum
{
  OS_WIN = 0,
  OS_OSX,
  OS_LIN,
};

// EITHER of these should be set as default!
uint8_t os_type = OS_LIN;

// "BASE" is the zeroth, and default, layer.
#define BASE 0
// "MDIA" is the media layer, on top of the BASE layer.
#define MDIA 1 // media keys

// Ease bitmasks for whether shift, ctrl, alt, or gui (super) is pressed
#define MODS_SHIFT_MASK (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
#define MODS_CTRL_MASK  (MOD_BIT(KC_LCTL)|MOD_BIT(KC_RCTRL))
#define MODS_ALT_MASK   (MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
#define MODS_GUI_MASK   (MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI))

// Track WPM (words per minute)
char wpm_str[12];

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE, // can always be here
    VRSN,               // show the keyboard's version and other stats (i.e. wpm)
    TOGGLE_BACK_LIGHT,  // does what it says
    EMOJI_DISFACE,      // send an emoji: ಠ_ಠ
    EMOJI_SHRUG,        // send an emoji: ¯\_(ツ)_/¯
};

#ifdef TAP_DANCE_ENABLE
// Tap dances declarations
enum {
    CT_MI = 0,
    CT_EQ,
    CT_AT,
    CT_IT,
    CT_BB,
};

// Tap dances' behaviour:
qk_tap_dance_action_t tap_dance_actions[] = {
    // -- to get ~
    [CT_MI] = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_TILDE),
    // == to get ~/
    [CT_EQ] = ACTION_TAP_DANCE_FN(dance_double_home),
    // @@ to get @_
    [CT_AT] = ACTION_TAP_DANCE_FN(dance_double_at),
    // $$ to get $_
    [CT_IT] = ACTION_TAP_DANCE_FN(dance_double_it),
    // BB to get CTRL+B
    [CT_BB] = ACTION_TAP_DANCE_DOUBLE(KC_B, LCTL(KC_B)),
};
#endif

// Easy function to "just" tap a key
inline void tap(uint16_t keycode) {
    register_code(keycode);
    unregister_code(keycode);
};

// Change bottom LEDs colours whenever the layer changes, or when a special key is
// pressed.
bool has_layer_changed = false;
bool want_light_on     = false;
bool pressed_meh       = false;
bool was_leading       = false;
static uint8_t current_layer;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Esc    |   1  |   2  |   3  |   4  |   5  |   6  |           |   7  |   8  |   9  |   0  |   -  |   =  |Backspac|
 * |--------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  /   |           |   \  |   Y  |   U  |   I  |   O  |   P  |   '"   |
 * |--------+------+------+------+------+------| MDIA |           |  CAG |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |;/MDIA|Ent/CMD |
 * |--------+------+------+------+------+------|  `~  |           | LEAD |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  UP  | / Shift|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |LCtrl |MDIA/`| AltG | Alt  |  Cmd |                                       |   [  |   ]  | LEFT | DOWN | RIGHT|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |  /?  |  '"  |       | Del  | AltGr|
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 | Space|LCtrl |------|       |------| Enter| Space|
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
#ifdef TAP_DANCE_ENABLE
        KC_ESC,        KC_1,            TD(CT_AT), KC_3,    TD(CT_IT), KC_5, KC_6,
#else
        KC_ESC,        KC_1,            KC_2,      KC_3,    KC_4,      KC_5, KC_6,
#endif
        KC_TAB,        KC_Q,            KC_W,      KC_E,    KC_R,      KC_T, LT(MDIA,KC_SLSH),
        KC_LCTL,       KC_A,            KC_S,      KC_D,    KC_F,      KC_G,
#ifdef TAP_DANCE_ENABLE
        KC_LSFT,       KC_Z,            KC_X,      KC_C,    KC_V,      TD(CT_BB), KC_GRV,
#else
        KC_LSFT,       KC_Z,            KC_X,      KC_C,    KC_V,      KC_B, KC_GRV,
#endif
        KC_LCTL,       LT(MDIA,KC_GRV), KC_RALT,   KC_LALT, KC_LGUI,
                                                        KC_SLSH,KC_QUOT,
                                                                KC_HOME,
                                                  KC_SPC,KC_LCTL,KC_END,
        // right hand
#ifdef TAP_DANCE_ENABLE
        KC_7,            KC_8, KC_9,    KC_0,    TD(CT_MI), TD(CT_EQ),      KC_BSPC,
#else
        KC_7,            KC_8, KC_9,    KC_0,    KC_MINS,   KC_EQL,           KC_BSPC,
#endif
        LCAG_T(KC_BSLS), KC_Y, KC_U,    KC_I,    KC_O,      KC_P,             KC_QUOT,
                         KC_H, KC_J,    KC_K,    KC_L,      LT(MDIA,KC_SCLN), GUI_T(KC_ENT),
        KC_LEAD,         KC_N, KC_M,    KC_COMM, KC_DOT,    KC_UP,            SFT_T(KC_SLSH),
                               KC_LBRC, KC_RBRC, KC_LEFT,   KC_DOWN,          KC_RIGHT,
        KC_DELT, KC_RALT,
        KC_PGUP,
        KC_PGDN, KC_ENT, KC_SPC
    ),
/* Keymap 1: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Esc    |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | DELETE |
 * |--------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * | KC_MYCM|      |      | MsUp |      |      | CALC |           | VERS |      |      |C+WhUp|C+WhDn| PSCR |  APP   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | MAIL |MsLeft|MsDown|MsRght|      |------|           |------| Left | Down |  Up  | Right|      |  Play  |
 * |--------+------+------+------+------+------| EMOJI|           |EMOJI |------+------+------+------+------+--------|
 * |        |      |MSAcc0|MSAcc1|MSAcc2|      |SHRUG |           |DISFCE|      |      |MDPrev|MDNext| PgUp |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      | Mclk | Lclk | Rclk |                                       |      |      | Home | PgDn |  End |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WhUP | WhDN |       | MUTE | BACK |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Pause|       | VolUp|      |TOGGLE|
 *                                 |      |      |------|       |------|      | BACK |
 *                                 |      |      | ScLck|       | VolDn|      | LIGHT|
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = LAYOUT_ergodox(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
        KC_MYCM, _______, _______, KC_MS_U, _______, _______, KC_CALC,
        _______, KC_MAIL, KC_MS_L, KC_MS_D, KC_MS_R, _______,
        _______, _______, KC_ACL0, KC_ACL1, KC_ACL2, _______, EMOJI_SHRUG,
        _______, _______, KC_BTN3, KC_BTN1, KC_BTN2,
                                                KC_WH_U, KC_WH_D,
                                                         KC_PAUS,
                                       _______, _______, KC_SLCK,
        // right hand
        KC_F7,         KC_F8,   KC_F9,   KC_F10,        KC_F11,        KC_F12,  KC_DELT,
        VRSN,          _______, _______, LCTL(KC_WH_U), LCTL(KC_WH_D), KC_PSCR, KC_APP,
                       KC_LEFT, KC_DOWN, KC_UP,         KC_RIGHT,      _______, KC_MPLY,
        EMOJI_DISFACE, _______, _______, KC_MPRV,       KC_MNXT,       KC_PGUP, _______,
                                _______, _______,       KC_HOME,       KC_PGDN, KC_END,
        KC_MUTE, KC_WBAK,
        KC_VOLU,
        KC_VOLD, _______, TOGGLE_BACK_LIGHT
),
};

#ifdef MFONTANI_OSX_RALT_UNICODE
// I've mapped Alt+Cmd+Space to switch to next layout.
// I usually stay in US layout, so switching gets me into Unicode layout, and
// switching back gets me back to US layout.
void osx_switch_input_layout(void) {
    register_code(KC_LGUI);
    register_code(KC_LALT);
    tap(KC_SPC);
    unregister_code(KC_LALT);
    unregister_code(KC_LGUI);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case VRSN:
            if (record->event.pressed) {
#ifdef MFONTANI_UPRINTF
                uprintf("process_record_user - VRSN\n");
#endif
                SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                SEND_STRING (" ");
                sprintf(wpm_str, "WPM: %03d", get_current_wpm());
                send_string(wpm_str);
            }
            return false;
            break;
        case TOGGLE_BACK_LIGHT:
            if (record->event.pressed) {
#ifdef MFONTANI_UPRINTF
                uprintf("process_record_user - TOGGLE_BACK_LIGHT\n");
#endif
                want_light_on = !want_light_on;
            }
            return false;
            break;
        case EMOJI_SHRUG: // ¯\_(ツ)_/¯
            if (record->event.pressed) {
#ifdef MFONTANI_UPRINTF
                uprintf("process_record_user - EMOJI_SHRUG\n");
#endif
#ifdef MFONTANI_OSX_RALT_UNICODE
                if (os_type == OS_OSX)
                    osx_switch_input_layout();
#endif
                send_unicode_hex_string("00AF 005C 005F 0028 30C4 0029 005F 002F 00AF");
#ifdef MFONTANI_OSX_RALT_UNICODE
                if (os_type == OS_OSX)
                    osx_switch_input_layout();
#endif
            }
            return false;
            break;
        case EMOJI_DISFACE: // ಠ_ಠ
            if(record->event.pressed){
#ifdef MFONTANI_UPRINTF
                uprintf("process_record_user - EMOJI_DISFACE\n");
#endif
#ifdef MFONTANI_OSX_RALT_UNICODE
                if (os_type == OS_OSX)
                    osx_switch_input_layout();
#endif
                send_unicode_hex_string("0CA0 005F 0CA0");
#ifdef MFONTANI_OSX_RALT_UNICODE
                if (os_type == OS_OSX)
                    osx_switch_input_layout();
#endif
            }
            return false;
            break;
    }
    return true;
}

void reset_unicode_input_mode(void) {
    if (os_type == OS_LIN) {
        set_unicode_input_mode(UC_LNX); // Linux
    } else if (os_type == OS_OSX) {
#ifdef MFONTANI_OSX_RALT_UNICODE
        set_unicode_input_mode(UC_OSX_RALT); // Mac OSX using right alt key
#else
        set_unicode_input_mode(UC_OSX); // Mac OSX
#endif
    } else if (os_type == OS_WIN) {
        set_unicode_input_mode(UC_WIN); // Windows (with registry key, see wiki)
        // set_unicode_input_mode(UC_WINC); // Windows (with WinCompose, see wiki)
    }
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    reset_unicode_input_mode();
    ergodox_led_all_on();
#ifdef RGBLIGHT_ENABLE
    rgblight_enable();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(0x0,0x0,0x50);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
    wait_ms(200);
    ergodox_led_all_off();
#endif
    has_layer_changed = true;
}

#ifdef TAP_DANCE_ENABLE
// tap dance for == for ~/
void dance_double_home(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap(KC_EQL);
            break;
        case 2:
            if (keyboard_report->mods == 0) {
                SEND_STRING("~/");
                break;
            }
        default:
            for (int i = 0 ; i < state->count ; i++) {
                tap(KC_EQL);
                if (i < (state->count-1)) {
                    wait_ms(20);
                }
            }
    }
    reset_tap_dance(state);
}
// tap dance for @@ to @_
void dance_double_at(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap(KC_2);
            break;
        case 2:
            if (keyboard_report->mods == 0) {
                SEND_STRING("@_");
                break;
            }
        default:
            for (int i = 0 ; i < state->count ; i++) {
                tap(KC_2);
                if (i < (state->count-1)) {
                    wait_ms(20);
                }
            }
    }
    reset_tap_dance(state);
}
// tap dance for $$ to $_
void dance_double_it(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap(KC_4);
            break;
        case 2:
            if (keyboard_report->mods == 0) {
                SEND_STRING("$_");
                break;
            }
        default:
            for (int i = 0 ; i < state->count ; i++) {
                tap(KC_4);
                if (i < (state->count-1)) {
                    wait_ms(20);
                }
            }
    }
    reset_tap_dance(state);
}
#endif

// Runs constantly in the background, in a loop.
LEADER_EXTERNS();
void matrix_scan_user(void) {

    uint8_t layer         = biton32(layer_state);
    uint8_t modifiders    = get_mods();
    uint8_t led_usb_state = host_keyboard_leds();
    uint8_t one_shot      = get_oneshot_mods();

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    // Since we're not using the LEDs here for layer indication anymore,
    // then lets use them for modifier indicators.  Shame we don't have 4...
    // Also, no "else", since we want to know each, independently.
    if (modifiders & MODS_SHIFT_MASK || led_usb_state & (1<<USB_LED_CAPS_LOCK) || one_shot & MODS_SHIFT_MASK) {
      ergodox_right_led_2_on();
      ergodox_right_led_2_set( 20 );
    }
    if (modifiders & MODS_CTRL_MASK || one_shot & MODS_CTRL_MASK) {
      ergodox_right_led_1_on();
      ergodox_right_led_1_set( 10 );
    }
    if (modifiders & MODS_ALT_MASK || one_shot & MODS_ALT_MASK) {
      ergodox_right_led_3_on();
      ergodox_right_led_3_set( 10 );
    }
    // That said, the MEH key should also show the underglow.
    if (modifiders & MODS_CTRL_MASK && modifiders & MODS_ALT_MASK && modifiders & MODS_GUI_MASK) {
        #ifdef RGBLIGHT_ENABLE
        rgblight_sethsv_noeeprom_yellow();
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        #endif
        pressed_meh = true;
    } else if (pressed_meh) { // reset underglow color on unpress
        has_layer_changed = true;
        pressed_meh = false;
    }

    if (leading) {
        if (!was_leading) {
            #ifdef RGBLIGHT_ENABLE
            rgblight_sethsv_noeeprom_purple();
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            #endif
            was_leading = true;
        }
    }
    // This is a bit of a hack, to ensure my script which shows the keyboard
    // layout using a Perl/Tk script, shows the list of supported leader keys:
/* Keymap 666: LEAD xxx
 * - WIN, OSX, LIN - switch to Windows, OSX RALT or Linux Unicode modes
 * - M - which "mode" are we? (WIN, OSX, LIN)
 * - V - print VERSION
 * - - - en dash
 * - = - em dash
 * - ? - interrobang
 * - 4 - ALT+F4 (kill window under Windows)
 * - R - taps 9 random Base64 bytes
 * - ` - types "~/"
 * - h - types "~/"
 * - . - types "…"
 */
    LEADER_DICTIONARY()
    {
        leading     = false;
        was_leading = false;
        leader_end();

        // Switch "os_type"
        SEQ_THREE_KEYS(KC_W, KC_I, KC_N) { os_type = OS_WIN; reset_unicode_input_mode(); };
        SEQ_THREE_KEYS(KC_O, KC_S, KC_X) { os_type = OS_OSX; reset_unicode_input_mode(); };
        SEQ_THREE_KEYS(KC_L, KC_I, KC_N) { os_type = OS_LIN; reset_unicode_input_mode(); };

        // Leader M -> "Mode" (which OS are we on?)
        SEQ_ONE_KEY (KC_M) {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - M - MODE\n");
#endif
            if (os_type == OS_WIN) {
                SEND_STRING("WIN");
            } else if (os_type == OS_OSX) {
                SEND_STRING("OSX");
            } else if (os_type == OS_LIN) {
                SEND_STRING("LIN");
            } else {
                SEND_STRING("WTF");
            }
        }
        // Leader - -> en dash
        SEQ_ONE_KEY (KC_MINS) {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - - - EN DASH\n");
#endif
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
            send_unicode_hex_string("2013");
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
        }
        // Leader = -> em dash
        SEQ_ONE_KEY (KC_EQL) {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - = - EM DASH\n");
#endif
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
            send_unicode_hex_string("2014");
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
        }
        // Leader / -> interrobang
        SEQ_ONE_KEY (KC_SLSH) {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - ? - INTERROBANG\n");
#endif
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
            send_unicode_hex_string("203d");
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
        }
        // Leader V -> Version
        SEQ_ONE_KEY (KC_V) {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - V - VERSION\n");
#endif
            SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            SEND_STRING (" ");
            sprintf(wpm_str, "WPM: %03d", get_current_wpm());
            send_string(wpm_str);
        }
        // Leader 4 -> Alt+F4
        SEQ_ONE_KEY(KC_4)
        {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - 4 - Alt+F4\n");
#endif
            register_code(KC_LALT);
            tap(KC_F4);
            unregister_code(KC_LALT);
        }
        // Leader R -> a few random Base64 bytes
        SEQ_ONE_KEY(KC_R)
        {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - R - RANDOM BASE64\n");
#endif
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
            tap_random_base64();
        }
        // Leader ` -> ~/
        SEQ_ONE_KEY(KC_GRV)
        {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - ` - ~/\n");
#endif
            register_code(KC_RSFT);
            tap(KC_GRV);
            unregister_code(KC_RSFT);
            tap(KC_SLSH);
        }
        // Leader h -> ~/
        SEQ_ONE_KEY(KC_H)
        {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - h - ~/\n");
#endif
            register_code(KC_RSFT);
            tap(KC_GRV);
            unregister_code(KC_RSFT);
            tap(KC_SLSH);
        }
        // Leader . -> …
        SEQ_ONE_KEY(KC_DOT)
        {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - . - dot dot dot\n");
#endif
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
            send_unicode_hex_string("2026");
#ifdef MFONTANI_OSX_RALT_UNICODE
            if (os_type == OS_OSX)
                osx_switch_input_layout();
#endif
        }
        // Not really working, possibly because KC_SCLN does also "MEDIA" layer
        // switch on my keyboard.
        // Leader ; -> …
        SEQ_ONE_KEY(KC_SCLN)
        {
#ifdef MFONTANI_UPRINTF
            uprintf("LEADER - ; - dot dot dot\n");
#endif
            register_code(KC_RALT);
            tap(KC_SCLN);
            unregister_code(KC_RALT);
        }

        has_layer_changed = true;
    }

    if (leading || was_leading) return;

    switch (layer) {
        case MDIA:
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_sethsv_noeeprom_red();
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            #endif
            break;
        default:
            // none
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                if (want_light_on) {
                    rgblight_sethsv_noeeprom_orange();
                } else {
                    rgblight_sethsv_noeeprom(0,0,0);
                }
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            #endif
            break;
    }
    if (current_layer == layer) {
        has_layer_changed = false;
    } else {
        has_layer_changed = true;
        current_layer = layer;
    }
}
