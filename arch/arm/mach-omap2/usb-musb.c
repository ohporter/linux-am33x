/*
 * linux/arch/arm/mach-omap2/usb-musb.c
 *
 * This file will contain the board specific details for the
 * MENTOR USB OTG controller on OMAP3430
 *
 * Copyright (C) 2007-2008 Texas Instruments
 * Copyright (C) 2008 Nokia Corporation
 * Author: Vikram Pandita
 *
 * Generalization by:
 * Felipe Balbi <felipe.balbi@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>

#include <linux/usb/musb.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/am35xx.h>
#include <plat/usb.h>
#include <plat/omap_device.h>
#include "mux.h"

static struct musb_hdrc_config musb_config = {
	.fifo_mode	= 4,
	.multipoint	= 1,
	.dyn_fifo	= 1,
	.num_eps	= 16,
	.ram_bits	= 12,
};

static struct musb_hdrc_platform_data musb_plat[] = {
	{
		.config         = &musb_config,
		.clock          = "ick",
	},
	{
		.config         = &musb_config,
		.clock          = "ick",
	},
};

static u64 musb_dmamask = DMA_BIT_MASK(32);

static struct omap_device_pm_latency omap_musb_latency[] = {
	{
		.deactivate_func	= omap_device_idle_hwmods,
		.activate_func		= omap_device_enable_hwmods,
		.flags			= OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

/*
static void usb_musb_mux_init(struct omap_musb_board_data *board_data)
{
	switch (board_data->interface_type) {
	case MUSB_INTERFACE_UTMI:
		omap_mux_init_signal("usba0_otg_dp", OMAP_PIN_INPUT);
		omap_mux_init_signal("usba0_otg_dm", OMAP_PIN_INPUT);
		break;
	case MUSB_INTERFACE_ULPI:
		omap_mux_init_signal("usba0_ulpiphy_clk",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_stp",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dir",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_nxt",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat0",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat1",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat2",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat3",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat4",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat5",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat6",
						OMAP_PIN_INPUT_PULLDOWN);
		omap_mux_init_signal("usba0_ulpiphy_dat7",
						OMAP_PIN_INPUT_PULLDOWN);
		break;
	default:
		break;
	}
}*/

static struct omap_musb_board_data musb_default_board_data = {
	.interface_type		= MUSB_INTERFACE_ULPI,
	.mode			= MUSB_OTG,
	.power			= 100,
};

void __init usb_musb_init(struct omap_musb_board_data *musb_board_data)
{
	struct omap_hwmod		*oh;
	struct platform_device		*pdev;
	struct device			*dev;
	int				bus_id = -1;
	const char			*oh_name, *name;
	struct omap_musb_board_data	*board_data;

	if (musb_board_data)
		board_data = musb_board_data;
	else
		board_data = &musb_default_board_data;

	/*
	 * REVISIT: This line can be removed once all the platforms using
	 * musb_core.c have been converted to use use clkdev.
	 */
	musb_plat[0].clock = "ick";
	musb_plat[0].board_data = board_data;
	musb_plat[0].power = board_data->power >> 1;
	musb_plat[0].mode = board_data->mode;
	musb_plat[0].extvbus = board_data->extvbus;

	/*
	 * OMAP3630/AM35x platform has MUSB RTL-1.8 which has the fix for the
	 * issue restricting active endpoints to use first 8K of FIFO space.
	 * This issue restricts OMAP35x platform to use fifo_mode '5'.
	 */
	if (cpu_is_omap3430())
		musb_config.fifo_mode = 5;

	if (cpu_is_omap44xx())
		omap4430_phy_init(dev);

	if (cpu_is_omap3517() || cpu_is_omap3505()) {
		oh_name = "am35x_otg_hs";
		name = "musb-am35x";
	} else if (cpu_is_ti816x() || cpu_is_am33xx()) {
		musb_config.fifo_mode = 4;

		/* only usb0 port enabled in peripheral mode*/
		if (board_data->mode == MUSB_PERIPHERAL) {
			board_data->instances = 0;
			musb_config.fifo_mode = 6;
		}

		board_data->set_phy_power = ti81xx_musb_phy_power;
		oh_name = "usb_otg_hs";
		name = "ti81xx-usbss";
	} else {
		oh_name = "usb_otg_hs";
		name = "musb-omap2430";
	}

	oh = omap_hwmod_lookup(oh_name);
	if (!oh) {
		pr_err("Could not look up %s\n", oh_name);
		return;
	}

	pdev = omap_device_build(name, bus_id, oh, &musb_plat,
			       sizeof(musb_plat), omap_musb_latency,
			       ARRAY_SIZE(omap_musb_latency), false);
	if (IS_ERR(pdev)) {
		pr_err("Could not build omap_device for %s %s\n",
						name, oh_name);
		return;
	}

	dev = &pdev->dev;
	get_device(dev);
	dev->dma_mask = &musb_dmamask;
	dev->coherent_dma_mask = musb_dmamask;
	put_device(dev);

	if (cpu_is_omap44xx())
		omap4430_phy_init(dev);
}
