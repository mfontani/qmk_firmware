#ifndef CONFIG_MFONTANI_H
#define CONFIG_MFONTANI_H

#define LEADER_TIMEOUT 300

// I want quick tap dances
#undef TAPPING_TERM
#define TAPPING_TERM 175

// Ensure each key pressed after leader resets the timeout
#define LEADER_PER_KEY_TIMING

// Allow processing more than one key per scan
// You may want to enable QMK_KEYS_PER_SCAN because the Ergodox has a relatively slow scan rate.
#define QMK_KEYS_PER_SCAN 4

// #define MFONTANI_OSX_RALT_UNICODE 1

// #define MFONTANI_UPRINTF 1

#endif
