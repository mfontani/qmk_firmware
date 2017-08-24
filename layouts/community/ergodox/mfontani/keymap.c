#include <stdarg.h>
#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#ifdef LEADER_TIMEOUT
#undef LEADER_TIMEOUT
#endif
#define LEADER_TIMEOUT 300

#define BASE 0 // default layer
#define MDIA 1 // media keys
#define SYMB 2 // symbols
#define LRGB 3 // unused RGB layer

#define _______ KC_TRNS

#define MODS_SHIFT_MASK (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
#define MODS_CTRL_MASK  (MOD_BIT(KC_LCTL)|MOD_BIT(KC_RCTRL))
#define MODS_ALT_MASK   (MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
#define MODS_GUI_MASK   (MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI))

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE, // can always be here
    EPRM,
    VRSN,
    TOGGLE_BACK_LIGHT,
    RGB_SLD,
    EMOJI_DISFACE,
    EMOJI_SHRUG,
};

// Tap dances
enum {
    CT_MINSTILDE = 0,
};

inline void tap(uint16_t keycode) {
    register_code(keycode);
    unregister_code(keycode);
};

// custom layer change change colour
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
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  /   |           |   \  |   Y  |   U  |   I  |   O  |   P  |   '"   |
 * |--------+------+------+------+------+------| SYMB |           |  CAG |------+------+------+------+------+--------|
 * | `~ Ctrl|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / MD|Ent/Cmd |
 * |--------+------+------+------+------+------| LEAD |           | LEAD |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  UP  | / Shift|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |LCtrl |SYMB/`| AltG | Alt  |  Cmd |                                       |[ AltG|] AltG| LEFT | DOWN | RIGHT|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        |  /?  |  '"  |       | Del  |  AltGr |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Bkspc |------|       |------|  Enter | Space|
 *                                 |      |      | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_ESC,        KC_1,            KC_2,    KC_3,    KC_4,    KC_5, KC_6,
        KC_TAB,        KC_Q,            KC_W,    KC_E,    KC_R,    KC_T, LT(SYMB,KC_SLSH),
        CTL_T(KC_GRV), KC_A,            KC_S,    KC_D,    KC_F,    KC_G,
        KC_LSFT,       KC_Z,            KC_X,    KC_C,    KC_V,    KC_B, KC_LEAD,
        KC_LCTRL,      LT(SYMB,KC_GRV), KC_RALT, KC_LALT, KC_LGUI,
                                                      KC_SLSH,KC_QUOT,
                                                              KC_HOME,
                                               KC_SPC,KC_BSPC,KC_END,
        // right hand
        KC_7,            KC_8, KC_9,            KC_0,            TD(CT_MINSTILDE), KC_EQL,           KC_BSPC,
        LCAG_T(KC_BSLS), KC_Y, KC_U,            KC_I,            KC_O,    KC_P,             KC_QUOT,
                         KC_H, KC_J,            KC_K,            KC_L,    LT(MDIA,KC_SCLN), GUI_T(KC_ENT),
        KC_LEAD,         KC_N, KC_M,            KC_COMM,         KC_DOT,  KC_UP,            SFT_T(KC_SLSH),
                               ALGR_T(KC_LBRC), ALGR_T(KC_RBRC), KC_LEFT, KC_DOWN,          KC_RIGHT,
        KC_DELT, KC_RALT,
        KC_PGUP,
        KC_PGDN, KC_ENT, KC_SPC
    ),
/* Keymap 1: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Esc    |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | Version|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------| Left | Down |  Up  | Right|      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MSAcc0|MSAcc1|MSAcc2|      |      |           |      |      |      | Prev | Next |  Up  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |      |      | Left | Down | Right|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WhUP | WhDN |       | MUTE | BACK |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | BriUP|       | VolUp|      |TOGGLE|
 *                                 |      |      |------|       |------|      | BACK |
 *                                 |      |      | BriDN|       | VolDn|      | LIGHT|
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = LAYOUT_ergodox(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
        _______, _______, _______, KC_MS_U, _______, _______, _______,
        _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,
        _______, _______, KC_ACL0, KC_ACL1, KC_ACL2, _______, _______,
        _______, _______, _______, KC_BTN1, KC_BTN2,
                                                KC_WH_U, KC_WH_D,
                                                         KC_PAUS,
                                       _______, _______, KC_SLCK,
        // right hand
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,  VRSN,
        _______, _______, _______, _______, _______,  _______, _______,
                 KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, KC_MPLY,
        _______, _______, _______, KC_MPRV, KC_MNXT,  KC_UP,   _______,
                          _______, _______, KC_RIGHT, KC_DOWN, KC_RIGHT,
        KC_MUTE, KC_WBAK,
        KC_VOLU,
        KC_VOLD, _______, TOGGLE_BACK_LIGHT
),
/* Keymap 2: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * | Esc     |      |      |      |      |      |      |           |      |      |      |      |      |      | DELETE |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           | VERS |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |¯(ツ)¯|           | ಠ_ಠ  |      |      |      |      | Page |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |  UP  |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      | HOME |PageDN|  END |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WhUP | WhDN |       | MUTE | BACK |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | BriUP|       | VolUp|      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      | BriDN|       | VolDn|      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = LAYOUT_ergodox(
       // left hand
       KC_ESC,  _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, EMOJI_SHRUG,
       _______, _______, _______, _______, _______,
                                                KC_WH_U, KC_WH_D,
                                                         KC_PAUS,
                                       _______, _______, KC_SLCK,
       // right hand
       _______,       _______, _______,_______, _______, _______, KC_DELT,
       VRSN,          _______, _______,_______, _______, _______, _______,
                      _______, _______,_______, _______, _______, _______,
       EMOJI_DISFACE, _______, _______,_______, _______, KC_PGUP, _______,
                      _______, _______,KC_HOME, KC_PGDN, KC_END,
       KC_MUTE, KC_WBAK,
       KC_VOLU,
       KC_VOLD, _______, _______
),
/* Keymap 3: COMPLETELY UNUSED RGB light layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           | VERS |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Animat| EPRM |       |Toggle|Solid |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |Bright|Bright|      |       |      |Hue-  |Hue+  |
 *                                 |ness- |ness+ |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[LRGB] = LAYOUT_ergodox(
       // left hand
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______,
                                       RGB_MOD,_______,
                                               _______,
                               RGB_VAD,RGB_VAI,_______,
       // right hand
       _______, _______, _______, _______, _______, _______, _______,
       VRSN,    _______, _______, _______, _______, _______, _______,
                _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______,
       RGB_TOG, RGB_SLD,
       _______,
       _______, RGB_HUD, RGB_HUI
),
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

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

// old way with M(...); unused.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case EPRM:
            if (record->event.pressed) {
                uprintf("process_record_user - EPRM\n");
                eeconfig_init();
            }
            return false;
            break;
        case VRSN:
            if (record->event.pressed) {
                uprintf("process_record_user - VRSN\n");
                SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            }
            return false;
            break;
        case TOGGLE_BACK_LIGHT:
            if (record->event.pressed) {
                uprintf("process_record_user - TOGGLE_BACK_LIGHT\n");
                want_light_on = !want_light_on;
            }
            return false;
            break;
        case RGB_SLD:
            if (record->event.pressed) {
                uprintf("process_record_user - RGB_SLD\n");
                #ifdef RGBLIGHT_ENABLE
                rgblight_mode(1);
                #endif
            }
            return false;
            break;
        case EMOJI_SHRUG: // ¯\_(ツ)_/¯
            if (record->event.pressed) {
                uprintf("process_record_user - EMOJI_SHRUG\n");
                osx_switch_input_layout();
                process_unicode((0x00AF|QK_UNICODE), record);   // Hand
                tap(KC_BSLS);                                   // Arm
                register_code(KC_RSFT);
                tap(KC_UNDS);                                   // Arm
                tap(KC_LPRN);                                   // Head
                unregister_code(KC_RSFT);
                process_unicode((0x30C4|QK_UNICODE), record);   // Face
                register_code(KC_RSFT);
                tap(KC_RPRN);                                   // Head
                tap(KC_UNDS);                                   // Arm
                unregister_code(KC_RSFT);
                tap(KC_SLSH);                                   // Arm
                process_unicode((0x00AF|QK_UNICODE), record);   // Hand
                osx_switch_input_layout();
            }
            return false;
            break;
        case EMOJI_DISFACE: // ಠ_ಠ
            if(record->event.pressed){
                uprintf("process_record_user - EMOJI_DISFACE\n");
                osx_switch_input_layout();
                process_unicode((0x0CA0|QK_UNICODE), record);   // Eye
                register_code(KC_RSFT);
                tap(KC_MINS);
                unregister_code(KC_RSFT);
                process_unicode((0x0CA0|QK_UNICODE), record);   // Eye
                osx_switch_input_layout();
            }
            return false;
            break;
    }
    return true;
}

// MODE 2-5, breathing
// MODE 6-8, rainbow mood
// MODE 9-14, rainbow swirl
// MODE 15-20, snake
// MODE 21-23, knight
// MODE 24, xmas
// MODE 25-34, static rainbow

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    //set_unicode_input_mode(UC_LNX); // Linux
    //set_unicode_input_mode(UC_OSX); // Mac OSX
    set_unicode_input_mode(UC_OSX_RALT); // Mac OSX using right alt key
    //set_unicode_input_mode(UC_WIN); // Windows (with registry key, see wiki)
    //set_unicode_input_mode(UC_WINC); // Windows (with WinCompose, see wiki)
    ergodox_led_all_on();
#ifdef RGBLIGHT_ENABLE
    rgblight_enable();
    rgblight_sethsv(0x0,0x0,0x50);
    rgblight_sethsv(0x0,0x0,0x50);
    rgblight_mode(2);
    wait_ms(500);
    rgblight_sethsv(0,0,0);
    rgblight_sethsv(0,0,0);
    rgblight_mode(1);
    ergodox_led_all_off();
#endif
    has_layer_changed = true;
}

// tap dance for minus to tilde or home
void dance_tilde_home(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap(KC_MINS);
    }
    else if (state->count == 2) {
        SEND_STRING("~");
    }
    else if (state->count == 3) {
        SEND_STRING("~/");
    }
}

// tap dances
qk_tap_dance_action_t tap_dance_actions[] = {
    // -- to get ~ or --- to get ~/
    [CT_MINSTILDE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_tilde_home, NULL)
};

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
        rgblight_sethsv_turquoise();
        rgblight_sethsv_turquoise();
        rgblight_mode(1);
        pressed_meh = true;
    } else if (pressed_meh) { // reset underglow color on unpress
        has_layer_changed = true;
        pressed_meh = false;
    }

    if (leading) {
        if (!was_leading) {
            #ifdef RGBLIGHT_ENABLE
            rgblight_sethsv_orange();
            rgblight_sethsv_orange();
            rgblight_mode(1);
            #endif
            was_leading = true;
        }
    }
    LEADER_DICTIONARY()
    {
        leading     = false;
        was_leading = false;
        leader_end();

        // Leader V -> Version
        SEQ_ONE_KEY (KC_V) {
            uprintf("LEADER - V - VERSION\n");
            SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        // Leader C -> CTRL+B C for tmux
        SEQ_ONE_KEY(KC_C)
        {
            uprintf("LEADER - B - CTRL+B C\n");
            register_code(KC_LCTRL);
            tap(KC_B);
            unregister_code(KC_LCTRL);
            tap(KC_C);
        }
        // Leader B -> CTRL+B for tmux
        SEQ_ONE_KEY(KC_B)
        {
            uprintf("LEADER - B - CTRL+B\n");
            register_code(KC_LCTRL);
            tap(KC_B);
            unregister_code(KC_LCTRL);
        }
        // Leader S -> CTRL+B + S for tmux switch sessions
        SEQ_ONE_KEY(KC_S)
        {
            uprintf("LEADER - S - CTRL+B S\n");
            register_code(KC_LCTRL);
            tap(KC_B);
            unregister_code(KC_LCTRL);
            tap(KC_S);
        }
        // Leader 4 -> Alt+F4
        SEQ_ONE_KEY(KC_4)
        {
            uprintf("LEADER - 4 - Alt+F4\n");
            register_code(KC_LALT);
            tap(KC_F4);
            unregister_code(KC_LALT);
        }
        // Leader R -> a few random Base64 bytes
        SEQ_ONE_KEY(KC_R)
        {
            uprintf("LEADER - R - RANDOM BASE64\n");
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
            uprintf("LEADER - ` - ~/\n");
            register_code(KC_RSFT);
            tap(KC_GRV);
            unregister_code(KC_RSFT);
            tap(KC_SLSH);
        }
        // Leader h -> ~/
        SEQ_ONE_KEY(KC_H)
        {
            uprintf("LEADER - h - ~/\n");
            register_code(KC_RSFT);
            tap(KC_GRV);
            unregister_code(KC_RSFT);
            tap(KC_SLSH);
        }
        // Leader . -> …
        SEQ_ONE_KEY(KC_DOT)
        {
            uprintf("LEADER - . - dot dot dot\n");
            register_code(KC_RALT);
            tap(KC_SCLN);
            unregister_code(KC_RALT);
        }
        // Not really working, possibly because KC_SCLN does also "MEDIA" layer
        // switch on my keyboard.
        // Leader ; -> …
        SEQ_ONE_KEY(KC_SCLN)
        {
            uprintf("LEADER - ; - dot dot dot\n");
            register_code(KC_RALT);
            tap(KC_SCLN);
            unregister_code(KC_RALT);
        }

        has_layer_changed = true;
    }

    if (leading || was_leading) return;

    switch (layer) {
        case 1:
            // ergodox_right_led_1_on();
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_sethsv(0, 200, 200);
                rgblight_sethsv(0, 200, 200);
                rgblight_mode(1);
            }
            #endif
            break;
        case 2:
            // ergodox_right_led_2_on();
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_sethsv(120,200,200);
                rgblight_sethsv(120,200,200);
                rgblight_mode(1);
            }
            #endif
            break;
        case 3:
            // ergodox_right_led_3_on();
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_sethsv_blue();
                rgblight_sethsv_blue();
                rgblight_mode(1);
            }
            #endif
            break;
        default:
            // none
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                if (want_light_on) {
                    rgblight_sethsv(0x30,0x30,0x50);
                    rgblight_sethsv(0x30,0x30,0x50);
                    rgblight_mode(3);
                } else {
                    rgblight_sethsv(0,0,0);
                    rgblight_sethsv(0,0,0);
                    rgblight_mode(1);
                }
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
