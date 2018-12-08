/*
 * D-Link DCH-G020
 *
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
 * Copyright (c) 2017 Nicolò Veronese <nicveronese@gmail.com>
 * Copyright (c) 2017 Federico Cappon <dududede371@gmail.com>
 * Copyright (c) 2018 Sebastian Schaper <openwrt@sebastianschaper.net>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/i2c-gpio.h>
#include <linux/platform_data/pca953x.h>
#include <linux/platform_device.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "nvram.h"

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/string.h>

#define DCH_G020_GPIO_LED_GREEN_POWER	13
#define DCH_G020_GPIO_LED_AMBER_POWER	14

#define DCH_G020_GPIO_BTN_WPS		3
#define DCH_G020_GPIO_BTN_RESET     17

#define DCH_G020_GPIO_I2C_SDA            0
#define DCH_G020_GPIO_I2C_SCL            1

#define DCH_G020_PCA9554_GPIO_BASE  32
#define DCH_G020_PCA9554_GPIO_IO_0  (0 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_ZWAVE  (1 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_IO_2  (2 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_USB_HUB_RESET  (3 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_IO_4  (4 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_IO_5  (5 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_IO_6  (6 + DCH_G020_PCA9554_GPIO_BASE)
#define DCH_G020_PCA9554_GPIO_IO_7  (7 + DCH_G020_PCA9554_GPIO_BASE)



#define DCH_G020_KEYS_POLL_INTERVAL	20	/* msecs */
#define DCH_G020_KEYS_DEBOUNCE_INTERVAL	(3 * DCH_G020_KEYS_POLL_INTERVAL)

#define DCH_G020_MAC_ADDR		0x1f020001

#define DCH_G020_WMAC_CALDATA_ADDR	0x1f010000
#define DCH_G020_WMAC_CALDATA_OFFSET	0x1000

static struct gpio_led dch_g020_leds_gpio[] __initdata = {
		{
				.name		= "d-link:green:power",
				.gpio		= DCH_G020_GPIO_LED_GREEN_POWER,
				.active_low	= 1,
		},
		{
				.name		= "d-link:amber:power",
				.gpio		= DCH_G020_GPIO_LED_AMBER_POWER,
				.active_low	= 0,
		}
};

static struct gpio_keys_button dch_g020_gpio_keys[] __initdata = {
	{
		.desc		= "WPS button",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = DCH_G020_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DCH_G020_GPIO_BTN_WPS,
		.active_low	= 1,
	},
	{
		.desc		= "Reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = DCH_G020_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DCH_G020_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};


static struct i2c_gpio_platform_data dch_g020_i2c_gpio_data = {
	.sda_pin	= DCH_G020_GPIO_I2C_SDA,
	.scl_pin	= DCH_G020_GPIO_I2C_SCL,
	.udelay		= 10,
};

static struct platform_device dch_g020_i2c_device = {
	.name	= "i2c-gpio",
	.id	= 0,
	.dev	= {
		.platform_data = &dch_g020_i2c_gpio_data,
	},
};

static struct pca953x_platform_data dch_g020_pca9554_data = {
	.gpio_base	= DCH_G020_PCA9554_GPIO_BASE,
	.irq_base	= -1,
};

static struct i2c_board_info dch_g020_i2c_devs[] __initdata = {
	{
		I2C_BOARD_INFO("pca9554a", 0x38),
		.platform_data = &dch_g020_pca9554_data,
	},
};

static void __init dch_g020_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(DCH_G020_WMAC_CALDATA_ADDR);
	u8 *mac_ptr = (u8 *) KSEG1ADDR(DCH_G020_MAC_ADDR);
	u8 mac[ETH_ALEN];

	ath79_parse_ascii_mac((char *) mac_ptr, mac);

	ath79_register_m25p80(NULL);

	platform_device_register(&dch_g020_i2c_device);
	i2c_register_board_info(0, dch_g020_i2c_devs,
				ARRAY_SIZE(dch_g020_i2c_devs));

	ath79_register_leds_gpio(-1, ARRAY_SIZE(dch_g020_leds_gpio),
		dch_g020_leds_gpio);

	ath79_register_gpio_keys_polled(-1, DCH_G020_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(dch_g020_gpio_keys),
			dch_g020_gpio_keys);

	gpio_request_one(DCH_G020_PCA9554_GPIO_USB_HUB_RESET,
			GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
			"Enable USB Hub");
	gpio_request_one(DCH_G020_PCA9554_GPIO_ZWAVE,
			GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
			"Enable Z-Wave");

	ath79_register_usb();

	ath79_register_wmac(art + DCH_G020_WMAC_CALDATA_OFFSET, mac);

	ath79_register_mdio(0, 0x0);

	ath79_init_mac(ath79_eth1_data.mac_addr, mac, 0);

	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;
	ath79_switch_data.phy_poll_mask |= BIT(4);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_DCH_G020_A1, "DCH-G020-A1",
             "D-Link DCH-G020 Rev. A1", dch_g020_setup);
