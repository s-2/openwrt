/*
 *  DLink DAP 1320 A1 experimental support
 *
 *  Copyright (C) 2019 Sebastian Schaper <openwrt@sebastianschaper.net>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define DAP_1320A1_GPIO_BTN_WPS         16
#define DAP_1320A1_GPIO_BTN_RESET       17

#define DAP_1320A1_GPIO_LED_RED         11
#define DAP_1320A1_GPIO_LED_GREEN       14

#define DAP_1320A1_KEYS_POLL_INTERVAL   20      /* msecs */
#define DAP_1320A1_KEYS_DEBOUNCE_INTERVAL (3 * DAP_1320A1_KEYS_POLL_INTERVAL)

#define DAP_1320A1_ART_ADDRESS          0x9f7f0000
#define DAP_1320A1_CALDATA_OFFSET       0x1000

#define DAP_1320A1_MAC_ADDRESS	        0x9f7fffa0

/* todo: set wifi credentials via uci-defaults */
#define DAP_1320A1_ESSID    	        0x9f7fffbc
#define DAP_1320A1_PSK      	        0x9f7fffdd

static struct gpio_led dap_1320_a1_leds_gpio[] __initdata = {
	{
		.name		= "d-link:green:power",
		.gpio		= DAP_1320A1_GPIO_LED_GREEN,
		.active_low	= 1,
	}, {
		.name		= "d-link:red:status",
		.gpio		= DAP_1320A1_GPIO_LED_RED,
		.active_low	= 1,
	},
};

static struct gpio_keys_button dap_1320_a1_gpio_keys[] __initdata = {
	{
		.desc		= "Reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = DAP_1320A1_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DAP_1320A1_GPIO_BTN_RESET,
		.active_low	= 0,
	}, {
		.desc		= "WPS button",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = DAP_1320A1_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DAP_1320A1_GPIO_BTN_WPS,
		.active_low	= 1,
	}
};

static void __init dap_1320_a1_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(DAP_1320A1_ART_ADDRESS);
	u8 *mac_ptr = (u8 *) KSEG1ADDR(DAP_1320A1_MAC_ADDRESS);
	u8 mac[ETH_ALEN];

	ath79_parse_ascii_mac((char *) mac_ptr, mac);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(dap_1320_a1_leds_gpio),
		dap_1320_a1_leds_gpio);

	ath79_register_gpio_keys_polled(1, DAP_1320A1_KEYS_POLL_INTERVAL,
		ARRAY_SIZE(dap_1320_a1_gpio_keys),
		dap_1320_a1_gpio_keys);

	ath79_register_m25p80(NULL);

	ath79_register_mdio(0, 0x0);

	ath79_register_wmac(art + DAP_1320A1_CALDATA_OFFSET, mac);
}

MIPS_MACHINE(ATH79_MACH_DAP_1320_A1, "DAP-1320-A1",
		"D-Link DAP-1320 rev. A1", dap_1320_a1_setup);
