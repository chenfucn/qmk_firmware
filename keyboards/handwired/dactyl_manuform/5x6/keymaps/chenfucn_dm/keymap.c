#include QMK_KEYBOARD_H


// Tap Dance keycodes
enum td_keycodes {
    ALT_LP, // `LALT` when held, `(` when tapped.
    CTL_RP
};

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP
} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare your tapdance functions:

// Function to determine the current tapdance state
td_state_t cur_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void altlp_finished(qk_tap_dance_state_t *state, void *user_data);
void altlp_reset(qk_tap_dance_state_t *state, void *user_data);
void ctlrp_finished(qk_tap_dance_state_t *state, void *user_data);
void ctlrp_reset(qk_tap_dance_state_t *state, void *user_data);


// Determine the tapdance state to return
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

// Handle the possible states for each tapdance keycode you define:

void altlp_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LALT)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            tap_code16(KC_LPRN);
            register_code16(KC_LPRN);
            break;
        default:
            break;
    }
}

void altlp_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LALT)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_LPRN);
            break;
        default:
            break;
    }
}

void ctlrp_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_RPRN);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_RCTL)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            tap_code16(KC_RPRN);
            register_code16(KC_RPRN);
            break;
        default:
            break;
    }
}

void ctlrp_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_RPRN);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_RCTL)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_RPRN);
            break;
        default:
            break;
    }
}

// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
    [ALT_LP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, altlp_finished, altlp_reset),
    [CTL_RP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctlrp_finished, ctlrp_reset)
};



bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_AUDIO_VOL_DOWN);
        } else {
            tap_code(KC_AUDIO_VOL_UP);
        }
    }/* else if (index == 1) { // Second encoder
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }*/
    return true;
}



#define _QWERTY 0
#define _SYMBOL 1
#define _NAVI 2

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x6(
        KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                 KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_PSCR,
        KC_GESC,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                 KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
        KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                 KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,   KC_ENT,
        KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                 KC_N,    KC_M,  KC_COMM,  KC_DOT,KC_SLSH,  KC_RSFT,
                  LCTL_T(KC_LBRC), LALT_T(KC_RBRC),                                   RALT_T(KC_MINS), RCTL_T(KC_EQL),
                          LT(_NAVI,KC_SPC), TD(ALT_LP),                 TD(CTL_RP), LT(_SYMBOL,KC_ENT),
                                               KC_DEL,  KC_QUOT,    KC_BSLS,  KC_BSPC,
                                               KC_MUTE, KC_CAPS,    TG(_NAVI), KC_RGUI
    ),

    [_SYMBOL] = LAYOUT_5x6(
        KC_GRV, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,                    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TRNS,
        KC_TRNS, KC_QUOT, KC_DQUO, KC_BSLS, KC_PIPE, KC_LBRC,                KC_RBRC, KC_7, KC_8, KC_9, KC_TRNS, KC_TRNS, 
        KC_TRNS, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LCBR,                KC_RCBR, KC_4, KC_5, KC_6, KC_MINS, KC_TRNS,
        KC_TRNS, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_LPRN,                   KC_RPRN, KC_1, KC_2, KC_3, KC_UNDS, KC_TRNS,
                          KC_TRNS, KC_TRNS,                                                 KC_0, KC_DOT,
                                            KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS,
                                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [_NAVI] = LAYOUT_5x6(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_HOME, KC_UP, KC_END, KC_PGUP, KC_LBRC,                   KC_RBRC, KC_F1, KC_F2, KC_F3, KC_F4, KC_TRNS,
        KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_LCBR,                KC_RCBR, KC_F5, KC_F6, KC_F7, KC_F8, KC_TRNS,
        KC_TRNS, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_INS), LSFT(KC_INS), KC_LPRN, KC_RPRN, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS,
                          KC_TRNS, KC_TRNS,                                                  KC_TRNS, KC_TRNS, 
                                            KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS,
                                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
