#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define BASE 0 // default layer
#define MDIA 1 // media keys
#define SYMB 2 // symbols

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD,
  EMOJI_DISFACE,
  EMOJI_SHRUG,
  M_BREW_UPDATE,
  M_GIT_REBASE_I_MASTER,
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
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  ~L2 |           |  \   |   Y  |   U  |   I  |   O  |   P  |   '"   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |   `~   |A / L1|   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L1|Ent/Cmd |
 * |--------+------+------+------+------+------|  [   |           |  ]   |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |LCtrl |Ctrl/`|  '"  | Alt  |  Cmd |                                       |  Up  | Down |   [  |   ]  | AltGr|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Left |Right |       | Del  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp | Space  |      |
 *                                 | Space| Cmd  |------|       |------| L1     |Enter |
 *                                 |      |      | End  |       | PgDn | toggle |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_ESC,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_6,
        KC_TAB,         KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   KC_FN1,
        KC_GRV,         LT(MDIA,KC_A),KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSFT,        KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   KC_LBRC,
        KC_LCTRL,       CTL_T(KC_GRV),KC_QUOT,      KC_LALT, KC_LGUI,
                                                      KC_LEFT,KC_RIGHT,
                                                              KC_HOME,
                                               KC_SPC,KC_LGUI,KC_END,
        // right hand
             KC_7,        KC_8,   KC_9,  KC_0,   KC_MINS,KC_EQL,           KC_BSPC,
             KC_BSLS,     KC_Y,   KC_U,  KC_I,   KC_O,   KC_P,             KC_QUOT,
                          KC_H,   KC_J,  KC_K,   KC_L,   LT(MDIA, KC_SCLN),GUI_T(KC_ENT),
             KC_RBRC,     KC_N,   KC_M,  KC_COMM,KC_DOT, CTL_T(KC_SLSH),   KC_RSFT,
                                  KC_UP, KC_DOWN,KC_LBRC,KC_RBRC,          KC_RALT,
             KC_DELT,        CTL_T(KC_ESC),
             KC_PGUP,
             KC_PGDN,LT(MDIA,KC_SPC),KC_ENT
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
 * |        |      |MSAcc0|MSAcc1|MSAcc2|      |      |           |      |      | Mute | Prev | Next |  Up  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Left | Down | Right|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WhUP | WhDN |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = LAYOUT_ergodox(
       KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_ACL0, KC_ACL1, KC_ACL2, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                                           KC_WH_U, KC_WH_D,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_F7,    KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,  VRSN,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,
                 KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_TRNS, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_MUTE, KC_MPRV, KC_MNXT,  KC_UP,   KC_TRNS,
                          KC_VOLU, KC_VOLD, KC_RIGHT, KC_DOWN, KC_RIGHT,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
/* Keymap 2: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * | Esc     | web01| web02| web03| web04|      |      |           |      |      |      |      |      |      | Version|
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      | app01|      |git   | ops01|      |           |      |      |      |      |      |      | EPRM   |
 * |         |      |      |      |rebase|      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |  -i  |      |      |           |      |      |      |      |      |      |        |
 * |         |      |      |      |master|      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         | app02| ops03|      | ops02|      |------|           |------|happy |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      | brew |¯(ツ)¯|           | ಠ_ಠ  |      |      |      |      |      |        |
 * |         |      |      |      |      |update|      |           |      |      |      |      |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Animat|      |       |Toggle|Solid |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |Bright|Bright|      |       |      |Hue-  |Hue+  |
 *                                 |ness- |ness+ |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = LAYOUT_ergodox(
       // left hand
       KC_ESC,  M_SSH_WEB01, M_SSH_WEB02, M_SSH_WEB03, M_SSH_WEB04,           KC_TRNS,       KC_TRNS,
       KC_TRNS, KC_TRNS,     M_SSH_APP01, KC_TRNS,     M_GIT_REBASE_I_MASTER, M_SSH_OPS01,   KC_TRNS,
       KC_TRNS, M_SSH_APP02, M_SSH_OPS03, KC_TRNS,     M_SSH_OPS02,           KC_TRNS,
       KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,               M_BREW_UPDATE, EMOJI_SHRUG,
       KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,
                                       RGB_MOD,KC_TRNS,
                                               KC_TRNS,
                               RGB_VAD,RGB_VAI,KC_TRNS,
       // right hand
       KC_TRNS,       KC_TRNS,     KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, VRSN,
       KC_TRNS,       KC_TRNS,     KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, EPRM,
                      M_SSH_HAPPY, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       EMOJI_DISFACE, KC_TRNS,     KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                      KC_TRNS,     KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS,
       RGB_TOG, RGB_SLD,
       KC_TRNS,
       KC_TRNS, RGB_HUD, RGB_HUI
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // dynamically generate these.
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
    case M_BREW_UPDATE:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("brew update ; brew upgrade ; brew cleanup");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_GIT_REBASE_I_MASTER:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("git rebase -i master");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_WEB01:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh web01.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_WEB02:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh web02.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_WEB03:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh web03.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_WEB04:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh web04.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_APP01:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh app01.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_APP02:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh app02.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_OPS01:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh ops01.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_OPS02:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh ops02.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_OPS03:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh ops03.theregister.co.uk");
            tap(KC_ENT);
        }
        return false;
        break;
    case M_SSH_HAPPY:
        if (record->event.pressed) {
            register_code(KC_LCTRL);
            tap(KC_A);
            tap(KC_K);
            unregister_code(KC_LCTRL);
            SEND_STRING("ssh happy.sitpub.com");
            tap(KC_ENT);
        }
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
};


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
};
