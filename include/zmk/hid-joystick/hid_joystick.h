/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zmk/keys.h>
#include <zmk/hid.h>
#include <zmk/endpoints_types.h>

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)

#include <zmk/hid-joystick/joystick.h>

struct zmk_hid_joystick_report_body_alt {
    uint8_t d_x;
    uint8_t d_y;
    uint8_t d_z;
    uint8_t d_rx;
    uint8_t d_ry;
    uint8_t d_rz;
    zmk_joystick_button_flags_t buttons;
} __packed;
struct zmk_hid_joystick_report_alt {
    uint8_t report_id;
    struct zmk_hid_joystick_report_body_alt body;
} __packed;
int zmk_hid_joy2_button_press(zmk_joystick_button_t button);
int zmk_hid_joy2_button_release(zmk_joystick_button_t button);
int zmk_hid_joy2_buttons_press(zmk_joystick_button_flags_t buttons);
int zmk_hid_joy2_buttons_release(zmk_joystick_button_flags_t buttons);
void zmk_hid_joy2_movement_set(int16_t x, int16_t y, int16_t z, int16_t rx, int16_t ry, int16_t rz);
void zmk_hid_joy2_movement_update(int16_t x, int16_t y, int16_t z, int16_t rx, int16_t ry, int16_t rz);
void zmk_hid_joy2_clear(void);
struct zmk_hid_joystick_report_alt *zmk_hid_get_joystick_report_alt();

#endif // IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
