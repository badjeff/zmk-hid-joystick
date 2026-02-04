/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
int zmk_usb_hid_send_joystick_report_alt(void);
#endif // IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
