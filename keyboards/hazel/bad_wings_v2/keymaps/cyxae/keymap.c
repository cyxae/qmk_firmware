// Copyright 2024-2025 cyxae (@cyxae)
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include QMK_KEYBOARD_H
#include "features/achordion.h"

#define HRM_A     LGUI_T(KC_A)
#define HRM_S     LALT_T(KC_S)
#define HRM_D     LCTL_T(KC_D)
#define HRM_F     LSFT_T(KC_F)
#define HRM_J     RSFT_T(KC_J)
#define HRM_K     RCTL_T(KC_K)
#define HRM_L     LALT_T(KC_L)
#define HRM_SCLN  RGUI_T(KC_SCLN)
#define CKC_BSPC  LT(L_NAV, KC_BSPC)
#define CKC_SPC   LT(L_NUM, KC_SPC)
#define ALT_F4    LALT(KC_F4)

enum layers {
  L_ALPHA,
  L_NUM,
  L_NAV,
  LAYER_LENGTH
};

enum custom_keycodes {
  ALT_TAB = SAFE_RANGE,
};

void matrix_scan_user(void) {
  achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {

  switch (tap_hold_keycode) {
    // Allow almost all Ctrl+<letter> as a same-hand chord on the left.
    case CKC_D:
      if (other_keycode == KC_W) { return true; }
      if (other_keycode == KC_R) { return true; }
      if (other_keycode == KC_T) { return true; }
      if (other_keycode == CKC_A) { return true; }
      if (other_keycode == CKC_S) { return true; }
      if (other_keycode == KC_G) { return true; }
      if (other_keycode == KC_Z) { return true; }
      if (other_keycode == KC_X) { return true; }
      if (other_keycode == KC_V) { return true; }
      if (other_keycode == KC_B) { return true; }
      if (other_keycode == KC_LSFT) { return true; }
      if (other_keycode == CKC_BSPC) { return true; }
      if (other_keycode == MS_BTN1) { return true; }
      break;
    case CKC_BSPC:
    case CKC_SPC:
      return true;
      break;
    break;
  }

  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case CKC_SPC:
    case HRM_J:
      return QUICK_TAP_TERM;  // Enable key repeating.
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HRM_A:
    case HRM_S:
    case HRM_L:
    case HRM_SCLN:
      return LONG_TAPPING_TERM;
    default:
      return TAPPING_TERM;
  }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case CKC_BSPC:
      // Immediately select the hold action when another key is pressed.
      return true;
    default:
      // Do not select the hold action when another key is pressed.
      return false;
  }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case CKC_SPC:
      // Immediately select the hold action when another key is tapped.
      return true;
    default:
      // Do not select the hold action when another key is tapped.
      return false;
  }
}

// NumRow 1 + Clic gauche = Download mode to allow upgrade firmware with qmk flash
const uint16_t PROGMEM dfu_cmb[] = {KC_1, MS_BTN1, COMBO_END};

combo_t key_combos[] = {
  COMBO(dfu_cmb, QK_BOOTLOADER),
};

// Ctrl + PgDn = Ctrl + PgUp
const key_override_t ctrl_pgdn_override = ko_make_basic(MOD_MASK_CTRL, KC_PGDN, C(KC_PGUP));

// Ctrl + PgUp = Ctrl + PgDn
const key_override_t ctrl_pgup_override = ko_make_basic(MOD_MASK_CTRL, KC_PGUP, C(KC_PGDN));

// Left Ctrl + G = Ctrl + A
const key_override_t ctrl_g_override = ko_make_basic(MOD_BIT(KC_LCTL), KC_G, C(KC_SCLN));

const key_override_t *key_overrides[] = {
  &ctrl_pgdn_override,
  &ctrl_pgup_override,
  &ctrl_g_override,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_ALPHA] = LAYOUT_split_3x5_3(
    KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,             KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,
    HRM_A,      HRM_S,      HRM_D,      HRM_F,      KC_G,             KC_H,       HRM_J,      HRM_K,      HRM_L,      HRM_SCLN,
    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,             KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,
                            KC_LSFT,    CKC_BSPC,   MS_BTN1,          MS_BTN2,    CKC_SPC,    KC_RALT
  ),
  [L_NUM] = LAYOUT_split_3x5_3(
    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,            KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,
    KC_1,       KC_2,       KC_3,       KC_4,       KC_5,             KC_6,       KC_7,       KC_8,       KC_9,       KC_0,
    KC_F11,     KC_F12,     KC_TRNS,    KC_SPC,     KC_TRNS,          KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_KP_DOT,
                            KC_TRNS,    KC_TRNS,    KC_TRNS,          KC_TRNS,    KC_TRNS,    KC_TRNS
  ),
  [L_NAV] = LAYOUT_split_3x5_3(
    KC_WBAK,    ALT_TAB,    C(KC_N),    KC_WFWD,    ALT_F4,           KC_TRNS,    KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,
    KC_ESC,     KC_ENT,     KC_TAB,     S(KC_TAB),  KC_DEL,           KC_TRNS,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,
    KC_CAPS,    KC_DOWN,    KC_UP,      KC_SPC,     KC_BSPC,          KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                            KC_TRNS,    KC_TRNS,    KC_TRNS,          MS_BTN3,    KC_TRNS,    KC_TRNS
  )
};

bool is_alt_tab_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_achordion(keycode, record)) { return false; }

  switch(keycode) {
    case ALT_TAB:
      if (record->event.pressed) {
        if (!(get_mods() & MOD_MASK_CTRL)) {
          if (!is_alt_tab_active) {
            is_alt_tab_active = true;
            register_code(KC_LALT);
          }
        }
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      return false;

    case CKC_BSPC:
      if (!record->event.pressed) {
        if (is_alt_tab_active) {
          unregister_code(KC_LALT);
          is_alt_tab_active = false;
        }
      }
      return true;
  }
  return true;
}
