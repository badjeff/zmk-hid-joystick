/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zmk/keys.h>
#include <zmk/hid.h>

#include <zmk/hid-joystick/hid.h>

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
int zmk_hog_send_joystick_report_alt(struct zmk_hid_joystick_report_body_alt *body);
#endif // IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
