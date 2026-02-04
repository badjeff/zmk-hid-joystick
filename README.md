# HID Joystick for ZMK

This module add new Joystick HID Usage Page for ZMK.

## What it does

It allow develop to add a joystick hid usage page to ZMK.

## Installation

Include this project on your ZMK's west manifest in `config/west.yml`:

```yaml
manifest:
  ...
  projects:
    ...
    - name: zmk-hid-joystick
      remote: badjeff
      revision: main
    ...
```

Now enable the config in your `<shield>.config` file (read the Kconfig file to find out all possible options):

```conf
# Zephyr Config
CONFIG_USB_DEVICE_HID=y

# Add one more USB_HID_DEVICE. 
# Hence, zmk default uses 1 HID, HID_0 for keyboard, 
# we use HID_1 for this joystick module.
CONFIG_USB_HID_DEVICE_COUNT=2
CONFIG_ZMK_HID_JOYSTICK_HID_BINDING_NUM=1

# Module Config
CONFIG_ZMK_HID_JOYSTICK=y

# Enable logging
CONFIG_ZMK_HID_JOYSTICK_LOG_LEVEL_DBG=y
```

While module is enabling, a new HID interface shall available from usage page `0xFF0C`. The actual value of usage page and report id could be modified in `include/zmk/hid-joystick/hid.h`.


## How it actually works

This module is only convert input events form input subsystem into hid input/output reports.
See below config to explain how it cooperates with other input driver modules to setup a joystick/gamepad hid device.

ZMK modules that used and should add to your `config/west.yml` for below `shield.keymap` sample.
- [zmk-analog-input-driver](https://github.com/badjeff/zmk-analog-input-driver)

```keymap
/ {
        /* Setup input-processor to intecept input and forward to new usage page  */
        zip_fwd_to_hid_joystick: zip_forward_fwd_to_hid_joystick {
                compatible = "zmk,input-processor-fwd-to-hid-joystick";
                #input-processor-cells = <0>;
        };

        /* input listener config for joystick move mode */
	analog_input_il {
		compatible = "zmk,input-listener";
		device = <&anin0>;
                input-processors = <&zip_fwd_to_hid_joystick>;
	};

        /* Setup joystick key press behavior and listener */
        /* listerer shall forward to JOYSTICK usage page */
        jskp: hid_joystick_key_press {
                compatible = "zmk,behavior-hid-joystick-key-press";
                #binding-cells = <1>;
        };
	jskp_il {
		compatible = "zmk,input-listener";
		device = <&jskp>;
                input-processors = <&zip_fwd_to_hid_joystick>;
	};

        keymap {
                compatible = "zmk,keymap";
                DEF_layer {
                        bindings = < .... 
                        
                        &jskp 0 /* joystick 1st button */
                        &jskp 1 /* joystick 2nd button */

                        ... >;
                };
       };

};
```
