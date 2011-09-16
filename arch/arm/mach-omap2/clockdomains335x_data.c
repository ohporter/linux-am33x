/*
 * AM335X Clock Domain data.
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/io.h>

#include "clockdomain.h"
#include "cm.h"
#include "cm335x.h"
#include "cm-regbits-335x.h"


static struct clockdomain l4ls_am335x_clkdm = {
	.name		= "l4ls_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_L4LS_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l3s_am335x_clkdm = {
	.name		= "l3s_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_L3S_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l4fw_am335x_clkdm = {
	.name		= "l4fw_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_L4FW_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l3_am335x_clkdm = {
	.name		= "l3_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_L3_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l4hs_am335x_clkdm = {
	.name		= "l4hs_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_L4HS_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain ocpwp_l3_am335x_clkdm = {
	.name		= "ocpwp_l3_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_OCPWP_L3_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain icss_ocp_am335x_clkdm = {
	.name		= "icss_ocp_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_ICSS_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain cpsw_125mhz_am335x_clkdm = {
	.name		= "cpsw_125mhz_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_CPSW_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain lcdc_am335x_clkdm = {
	.name		= "lcdc_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_LCDC_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain clk_24mhz_am335x_clkdm = {
	.name		= "clk_24mhz_clkdm",
	.pwrdm		= { .name = "per_pwrdm" },
	.cm_inst	= AM335X_CM_PER_MOD,
	.clkdm_offs	= AM335X_CM_PER_CLK_24MHZ_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l4_wkup_am335x_clkdm = {
	.name		= "l4_wkup_clkdm",
	.pwrdm		= { .name = "wkup_pwrdm" },
	.cm_inst	= AM335X_CM_WKUP_MOD,
	.clkdm_offs	= AM335X_CM_WKUP_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l3_aon_am335x_clkdm = {
	.name		= "l3_aon_clkdm",
	.pwrdm		= { .name = "wkup_pwrdm" },
	.cm_inst	= AM335X_CM_WKUP_MOD,
	.clkdm_offs	= AM335X_CM_L3_AON_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l4_wkup_aon_am335x_clkdm = {
	.name		= "l4_wkup_aon_clkdm",
	.pwrdm		= { .name = "wkup_pwrdm" },
	.cm_inst	= AM335X_CM_WKUP_MOD,
	.clkdm_offs	= AM335X_CM_L4_WKUP_AON_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain mpu_am335x_clkdm = {
	.name		= "mpu_clkdm",
	.pwrdm		= { .name = "mpu_pwrdm" },
	.cm_inst	= AM335X_CM_MPU_MOD,
	.clkdm_offs	= AM335X_CM_MPU_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l4_rtc_am335x_clkdm = {
	.name		= "l4_rtc_clkdm",
	.pwrdm		= { .name = "rtc_pwrdm" },
	.cm_inst	= AM335X_CM_RTC_MOD,
	.clkdm_offs	= AM335X_CM_RTC_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain gfx_l3_am335x_clkdm = {
	.name		= "gfx_l3_clkdm",
	.pwrdm		= { .name = "gfx_pwrdm" },
	.cm_inst	= AM335X_CM_GFX_MOD,
	.clkdm_offs	= AM335X_CM_GFX_L3_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain gfx_l4ls_gfx_am335x_clkdm = {
	.name		= "gfx_l4ls_gfx_clkdm",
	.pwrdm		= { .name = "gfx_pwrdm" },
	.cm_inst	= AM335X_CM_GFX_MOD,
	.clkdm_offs	= AM335X_CM_GFX_L4LS_GFX_CLKSTCTRL__1_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain l4_cefuse_am335x_clkdm = {
	.name		= "l4_cefuse_clkdm",
	.pwrdm		= { .name = "cefuse_pwrdm" },
	.cm_inst	= AM335X_CM_CEFUSE_MOD,
	.clkdm_offs	= AM335X_CM_CEFUSE_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain wkup_usb_335x_clkdm = {
	.name		= "wkup_usb_clkdm",
	.pwrdm		= { .name = "wkup_pwrdm" },
	.cm_inst	= AM335X_CM_WKUP_MOD,
	.clkdm_offs	= AM335X_CM_CLKDCOLDO_DPLL_PER_OFFSET,
	.clktrctrl_mask	= AM335X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_AM335X),
};

static struct clockdomain *clockdomains_am335x[] __initdata = {
	&l4ls_am335x_clkdm,
	&l3s_am335x_clkdm,
	&l4fw_am335x_clkdm,
	&l3_am335x_clkdm,
	&l4hs_am335x_clkdm,
	&ocpwp_l3_am335x_clkdm,
	&icss_ocp_am335x_clkdm,
	&cpsw_125mhz_am335x_clkdm,
	&lcdc_am335x_clkdm,
	&clk_24mhz_am335x_clkdm,
	&l4_wkup_am335x_clkdm,
	&l3_aon_am335x_clkdm,
	&l4_wkup_aon_am335x_clkdm,
	&mpu_am335x_clkdm,
	&l4_rtc_am335x_clkdm,
	&gfx_l3_am335x_clkdm,
	&gfx_l4ls_gfx_am335x_clkdm,
	&l4_cefuse_am335x_clkdm,
	&wkup_usb_335x_clkdm,
	NULL,
};

void __init am335x_clockdomains_init(void)
{
	clkdm_init(clockdomains_am335x, NULL, &omap3_clkdm_operations);
}
