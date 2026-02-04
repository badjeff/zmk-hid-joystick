/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zmk/endpoints.h>

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
int zmk_endpoints_send_joystick_report_alt();
#endif // IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
