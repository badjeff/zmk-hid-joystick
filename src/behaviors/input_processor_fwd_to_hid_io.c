/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_input_processor_fwd_to_hid_joystick

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <drivers/input_processor.h>
#include <zephyr/input/input.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/keymap.h>
#include <zmk/behavior.h>

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
#include <zmk/hid-joystick/endpoints.h>
#include <zmk/hid-joystick/hid.h>
#endif

// #if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct zip_fwd_to_hid_io_config {
};

enum zip_fwd_to_hid_joystick_data_mode {
    HID_JOYSTICK_DATA_MODE_NONE,
    HID_JOYSTICK_DATA_MODE_REL,
    HID_JOYSTICK_DATA_MODE_ABS,
};

struct zip_fwd_to_hid_joystick_data {
    enum zip_fwd_to_hid_joystick_data_mode mode;
    int16_t x, y, z;
    int16_t rx, ry, rz;
};

struct zip_fwd_to_hid_io_data {
    const struct device *dev;
    union {
        struct {
            struct zip_fwd_to_hid_joystick_data data;
            uint8_t button_set;
            uint8_t button_clear;
        } fwdr;
    };
};

static void handle_rel_code(const struct zip_fwd_to_hid_io_config *config,
                            struct zip_fwd_to_hid_io_data *data, struct input_event *event) {
    switch (event->code) {
    case INPUT_REL_X:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_REL;
        data->fwdr.data.x += event->value;
        break;
    case INPUT_REL_Y:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_REL;
        data->fwdr.data.y += event->value;
        break;
    case INPUT_REL_Z:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_REL;
        data->fwdr.data.z += event->value;
        break;
    case INPUT_REL_RX:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_REL;
        data->fwdr.data.rx += event->value;
        break;
    case INPUT_REL_RY:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_REL;
        data->fwdr.data.ry += event->value;
        break;
    case INPUT_REL_RZ:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_REL;
        data->fwdr.data.rz += event->value;
        break;
    default:
        break;
    }
}

static void handle_abs_code(const struct zip_fwd_to_hid_io_config *config,
                            struct zip_fwd_to_hid_io_data *data, struct input_event *event) {
    switch (event->code) {
    case INPUT_ABS_X:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_ABS;
        data->fwdr.data.x = event->value;
        break;
    case INPUT_ABS_Y:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_ABS;
        data->fwdr.data.y = event->value;
        break;
    case INPUT_ABS_Z:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_ABS;
        data->fwdr.data.z = event->value;
        break;
    case INPUT_ABS_RX:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_ABS;
        data->fwdr.data.rx = event->value;
        break;
    case INPUT_ABS_RY:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_ABS;
        data->fwdr.data.ry = event->value;
        break;
    case INPUT_ABS_RZ:
        data->fwdr.data.mode = HID_JOYSTICK_DATA_MODE_ABS;
        data->fwdr.data.rz = event->value;
        break;
    default:
        break;
    }
}

static void handle_key_code(const struct zip_fwd_to_hid_io_config *config,
                            struct zip_fwd_to_hid_io_data *data, struct input_event *event) {
    int8_t btn;

    switch (event->code) {
    case INPUT_BTN_0:
    case (INPUT_BTN_0 + 1):
    case (INPUT_BTN_0 + 2):
    case (INPUT_BTN_0 + 3):
    case (INPUT_BTN_0 + 4):
    case (INPUT_BTN_0 + 5):
    case (INPUT_BTN_0 + 6):
    case (INPUT_BTN_0 + 7):
        btn = event->code - INPUT_BTN_0;
        if (event->value > 0) {
            WRITE_BIT(data->fwdr.button_set, btn, 1);
        } else {
            WRITE_BIT(data->fwdr.button_clear, btn, 1);
        }
        break;
    default:
        break;
    }
}

static void clear_movement_data(struct zip_fwd_to_hid_joystick_data *data) {
    data->x = data->y = data->z = data->rx = data->ry = data->rz = 0;
    data->mode = HID_JOYSTICK_DATA_MODE_NONE;
}

static int zip_handle_event(const struct device *dev, struct input_event *event, uint32_t param1,
                            uint32_t param2, struct zmk_input_processor_state *state) {

    struct zip_fwd_to_hid_io_data *data = (struct zip_fwd_to_hid_io_data *)dev->data;
    const struct zip_fwd_to_hid_io_config *config = dev->config;
    
    switch (event->type) {
    case INPUT_EV_REL:
        handle_rel_code(config, data, event);
        break;
    case INPUT_EV_ABS:
        handle_abs_code(config, data, event);
        break;
    case INPUT_EV_KEY:
        handle_key_code(config, data, event);
        break;
    }

    if (event->sync) {

#if IS_ENABLED(CONFIG_ZMK_HID_JOYSTICK)
        if (data->fwdr.data.mode == HID_JOYSTICK_DATA_MODE_REL) {
            zmk_hid_joy2_movement_set(data->fwdr.data.x, data->fwdr.data.y, data->fwdr.data.z,
                                      data->fwdr.data.rx, data->fwdr.data.ry, data->fwdr.data.rz);
        }
        if (data->fwdr.button_set != 0) {
            for (int i = 0; i < ZMK_HID_JOYSTICK_NUM_BUTTONS; i++) {
                if ((data->fwdr.button_set & BIT(i)) != 0) {
                    zmk_hid_joy2_button_press(i);
                }
            }
        }
        if (data->fwdr.button_clear != 0) {
            for (int i = 0; i < ZMK_HID_JOYSTICK_NUM_BUTTONS; i++) {
                if ((data->fwdr.button_clear & BIT(i)) != 0) {
                    zmk_hid_joy2_button_release(i);
                }
            }
        }
        zmk_endpoints_send_joystick_report_alt();
        zmk_hid_joy2_movement_set(0, 0, 0, 0, 0, 0);
#endif

        clear_movement_data(&data->fwdr.data);

        data->fwdr.button_set = data->fwdr.button_clear = 0;
    }

    event->value = 0;
    event->sync = false;

    return 0;
}

static struct zmk_input_processor_driver_api zip_driver_api = {
    .handle_event = zip_handle_event,
};

static int zip_init(const struct device *dev) {
    struct zip_fwd_to_hid_io_data *data = dev->data;
    data->dev = dev;
    return 0;
};

#define KP_INST(n)                                                                         \
    static struct zip_fwd_to_hid_io_data zip_fwd_to_hid_io_data_##n = {};                  \
    static struct zip_fwd_to_hid_io_config zip_fwd_to_hid_io_config_##n = {};              \
    DEVICE_DT_INST_DEFINE(n, zip_init, NULL,                                               \
                          &zip_fwd_to_hid_io_data_##n,                                     \
                          &zip_fwd_to_hid_io_config_##n,                                   \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                \
                          &zip_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)

// #endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
