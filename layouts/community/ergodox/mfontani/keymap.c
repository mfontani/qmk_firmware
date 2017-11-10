#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define BASE 0 // default layer
#define MDIA 1 // media keys
#define SYMB 2 // symbols
#define LRGB 3 // unused RGB layer

#define _______ KC_TRNS

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE, // can always be here
    EPRM,
    VRSN,
    RGB_SLD,
    EMOJI_DISFACE,
    EMOJI_SHRUG,
    M_HOME,
    M_SSH_WEB01,
    M_SSH_WEB02,
    M_SSH_WEB03,
    M_SSH_WEB04,
    M_SSH_OPS01,
    M_SSH_OPS02,
    M_SSH_OPS03,
    M_SSH_APP01,
    M_SSH_APP02,
    M_SSH_HAPPY,
};

inline void tap(uint16_t keycode) {
    register_code(keycode);
    unregister_code(keycode);
};

// custom layer change change colour
bool has_layer_changed = false;
static uint8_t current_layer;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Esc    |   1  |   2  |   3  |   4  |   5  |   6  |           |   7  |   8  |   9  |   0  |   -  |   =  |Backspac|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  /   |           |   \  |   Y  |   U  |   I  |   O  |   P  |   '"   |
 * |--------+------+------+------+------+------| SYMB |           |      |------+------+------+------+------+--------|
 * | CAG `~ |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / MD|Ent/Cmd |
 * |--------+------+------+------+------+------|  ~/  |           |   /  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  UP  | / Shift|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |LCtrl |SYMB/`|   /  | Alt  |  Cmd |                                       |[ AltG|] AltG| LEFT | DOWN | RIGHT|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        | AltGr|  '"  |       | Del  |  AltGr |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Enter |------|       |------|  Enter | Space|
 *                                 |      |      | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_ESC,         KC_1,            KC_2,    KC_3,    KC_4,    KC_5, KC_6,
        KC_TAB,         KC_Q,            KC_W,    KC_E,    KC_R,    KC_T, LT(SYMB,KC_SLSH),
        LCAG_T(KC_GRV), KC_A,            KC_S,    KC_D,    KC_F,    KC_G,
        KC_LSFT,        KC_Z,            KC_X,    KC_C,    KC_V,    KC_B, M_HOME,
        KC_LCTRL,       LT(SYMB,KC_GRV), KC_SLSH, KC_LALT, KC_LGUI,
                                                      KC_RALT,KC_QUOT,
                                                              KC_HOME,
                                                KC_SPC,KC_ENT,KC_END,
        // right hand
        KC_7,    KC_8, KC_9,            KC_0,            KC_MINS, KC_EQL,           KC_BSPC,
        KC_BSLS, KC_Y, KC_U,            KC_I,            KC_O,    KC_P,             KC_QUOT,
                 KC_H, KC_J,            KC_K,            KC_L,    LT(MDIA,KC_SCLN), GUI_T(KC_ENT),
        KC_SLSH, KC_N, KC_M,            KC_COMM,         KC_DOT,  KC_UP,            SFT_T(KC_SLSH),
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
 *                                 |      |      |      |       | VolUp|      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       | VolDn|      |      |
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
                                                         _______,
                                       _______, _______, _______,
        // right hand
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,  VRSN,
        _______, _______, _______, _______, _______,  _______, _______,
                 KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, KC_MPLY,
        _______, _______, _______, KC_MPRV, KC_MNXT,  KC_UP,   _______,
                          _______, _______, KC_RIGHT, KC_DOWN, KC_RIGHT,
        KC_MUTE, KC_WBAK,
        KC_VOLU,
        KC_VOLD, _______, _______
),
/* Keymap 2: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * | Esc     | web01| web02| web03| web04|      |      |           |      |      |      |      |      |      | DELETE |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      | app01|      |      | ops01|      |           | VERS |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         | app02| ops03|      | ops02|      |------|           |------|happy |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |¯(ツ)¯|           | ಠ_ಠ  |      |      |      |      | Page |        |
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |  UP  |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      | HOME |PageDN|  END |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WhUP | WhDN |       | MUTE | BACK |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       | VolUp|      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       | VolDn|      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = LAYOUT_ergodox(
       // left hand
       KC_ESC,  M_SSH_WEB01, M_SSH_WEB02, M_SSH_WEB03, M_SSH_WEB04, _______,     _______,
       _______, _______,     M_SSH_APP01, _______,     _______,     M_SSH_OPS01, _______,
       _______, M_SSH_APP02, M_SSH_OPS03, _______,     M_SSH_OPS02, _______,
       _______, _______,     _______,     _______,     _______,     _______,     EMOJI_SHRUG,
       _______, _______,     _______,     _______,     _______,
                                                KC_WH_U, KC_WH_D,
                                                         _______,
                                       _______, _______, _______,
       // right hand
       _______,       _______,     _______,_______, _______, _______, KC_DELT,
       VRSN,          _______,     _______,_______, _______, _______, _______,
                      M_SSH_HAPPY, _______,_______, _______, _______, _______,
       EMOJI_DISFACE, _______,     _______,_______, _______, KC_PGUP, _______,
                      _______,     _______,KC_HOME, KC_PGDN, KC_END,
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

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // MACRODOWN only works in this function
    switch(id) {
        case 0:
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            }
            break;
        case 1:
            if (record->event.pressed) { // For resetting EEPROM
                eeconfig_init();
            }
            break;
    }
    return MACRO_NONE;
};

#define mfontani_send_ssh(record, host)  \
    if (record->event.pressed) { \
        register_code(KC_LCTRL); \
        tap(KC_A); \
        tap(KC_K); \
        unregister_code(KC_LCTRL); \
        SEND_STRING("ssh " host); \
        tap(KC_ENT); \
    }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case EPRM:
            if (record->event.pressed) {
                eeconfig_init();
            }
            return false;
            break;
        case VRSN:
            if (record->event.pressed) {
                SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            }
            return false;
            break;
        case RGB_SLD:
            if (record->event.pressed) {
                #ifdef RGBLIGHT_ENABLE
                rgblight_mode(1);
                #endif
            }
            return false;
            break;
        case EMOJI_SHRUG: // ¯\_(ツ)_/¯
            if (record->event.pressed) {
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
        case M_HOME:
            if (record->event.pressed) {
                SEND_STRING("~/");
            }
            return false;
            break;
        case M_SSH_WEB01:
            mfontani_send_ssh(record,"web01.theregister.co.uk");
            return false;
            break;
        case M_SSH_WEB02:
            mfontani_send_ssh(record,"web02.theregister.co.uk");
            return false;
            break;
        case M_SSH_WEB03:
            mfontani_send_ssh(record,"web03.theregister.co.uk");
            return false;
            break;
        case M_SSH_WEB04:
            mfontani_send_ssh(record,"web04.theregister.co.uk");
            return false;
            break;
        case M_SSH_APP01:
            mfontani_send_ssh(record,"app01.theregister.co.uk");
            return false;
            break;
        case M_SSH_APP02:
            mfontani_send_ssh(record,"app02.theregister.co.uk");
            return false;
            break;
        case M_SSH_OPS01:
            mfontani_send_ssh(record,"ops01.theregister.co.uk");
            return false;
            break;
        case M_SSH_OPS02:
            mfontani_send_ssh(record,"ops02.theregister.co.uk");
            return false;
            break;
        case M_SSH_OPS03:
            mfontani_send_ssh(record,"ops03.theregister.co.uk");
            return false;
            break;
        case M_SSH_HAPPY:
            mfontani_send_ssh(record,"happy.sitpub.com");
            return false;
            break;
    }
    return true;
}

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
    rgblight_mode(1);
    rgblight_setrgb(0xff,0xff,0xff);
#endif
    for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
        ergodox_led_all_set(i);
        wait_ms(5);
    }
    wait_ms(250);
    for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
        ergodox_led_all_set(i);
        wait_ms(10);
    }
    ergodox_led_all_off();
    has_layer_changed = true;
}


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
        case 1:
            ergodox_right_led_1_on();
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_setrgb(0xff,0x00,0x00);
            }
            #endif
            break;
        case 2:
            ergodox_right_led_2_on();
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_setrgb(0x00,0xff,0x00);
            }
            #endif
            break;
        case 3:
            ergodox_right_led_3_on();
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_setrgb(0x00,0x00,0xff);
            }
            #endif
            break;
        default:
            // none
            #ifdef RGBLIGHT_ENABLE
            if (has_layer_changed) {
                rgblight_setrgb(0x00,0x00,0x00);
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
