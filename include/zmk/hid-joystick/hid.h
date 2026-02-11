/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

#include <zmk/keys.h>
#include <zmk/hid.h>
#include <zmk/endpoints_types.h>

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
#include <zmk/hid-joystick/joystick.h>
#include <zmk/hid-joystick/hid_joystick.h>
#define ZMK_HID_JOYSTICK_NUM_BUTTONS 0x08
#define ZMK_HID_REPORT_ID__JOYSTICK 0x01
#endif // IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)

#include <dt-bindings/zmk/hid_usage.h>
#include <dt-bindings/zmk/hid_usage_pages.h>

// See https://www.usb.org/sites/default/files/hid1_11.pdf section 6.2.2.4 Main Items

#ifndef HID_USAGE_PAGE16
#define HID_USAGE_PAGE16(page, page2)                                                              \
    HID_ITEM(HID_ITEM_TAG_USAGE_PAGE, HID_ITEM_TYPE_GLOBAL, 2), page, page2
#endif

#define HID_USAGE16(a, b) HID_ITEM(HID_ITEM_TAG_USAGE, HID_ITEM_TYPE_LOCAL, 2), a, b

#define HID_USAGE16_SINGLE(a) HID_USAGE16((a & 0xFF), ((a >> 8) & 0xFF))

static const uint8_t zmk_hid_report_desc_alt[] = {
    
    // HID_USAGE_PAGE16(0x0C, 0xFF),
    // HID_USAGE(0x01),
    // HID_COLLECTION(HID_COLLECTION_APPLICATION),

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
    HID_USAGE_PAGE(HID_USAGE_GD),
    HID_USAGE(HID_USAGE_GD_JOYSTICK),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(ZMK_HID_REPORT_ID__JOYSTICK),
    HID_COLLECTION(HID_COLLECTION_LOGICAL),
    HID_USAGE_PAGE(HID_USAGE_GEN_DESKTOP),
    HID_USAGE(HID_USAGE_GD_X),
    HID_USAGE(HID_USAGE_GD_Y),
    HID_USAGE(HID_USAGE_GD_Z),
    HID_USAGE(HID_USAGE_GD_RX),
    HID_USAGE(HID_USAGE_GD_RY),
    HID_USAGE(HID_USAGE_GD_RZ),
    HID_LOGICAL_MIN8(0x81), // -127
    HID_LOGICAL_MAX8(0x7F), // +127
    HID_REPORT_SIZE(0x08),
    HID_REPORT_COUNT(0x06),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_REL),
    HID_USAGE_PAGE(HID_USAGE_BUTTON),
    HID_USAGE_MIN8(0x1),
    HID_USAGE_MAX8(ZMK_HID_JOYSTICK_NUM_BUTTONS),
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX8(0x01),
    HID_REPORT_SIZE(0x01),
    HID_REPORT_COUNT(0x8),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),
    HID_END_COLLECTION,
    HID_END_COLLECTION,
#endif // IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)

    // HID_END_COLLECTION,
};
