/*
 * TI816X Clock Domain data.
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
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
#include "cm816x.h"
#include "cm-regbits-816x.h"

static struct clockdomain alwon_mpu_816x_clkdm = {
	.name		= "alwon_mpu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI816X_CM_ALWON_MOD,
	.clkdm_offs	= TI816X_CM_ALWON_MPU_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain alwon_l3_slow_816x_clkdm = {
	.name		= "alwon_l3_slow_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI816X_CM_ALWON_MOD,
	.clkdm_offs	= TI816X_CM_ALWON_L3_SLOW_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain alwon_l3_fast_816x_clkdm = {
	.name		= "alwon_l3_fast_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI816X_CM_ALWON_MOD,
	.clkdm_offs	= TI816X_CM_ALWON_L3_FAST_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain alwon_ethernet_816x_clkdm = {
	.name		= "alwon_ethernet_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI816X_CM_ALWON_MOD,
	.clkdm_offs	= TI816X_CM_ETHERNET_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain mmu_816x_clkdm = {
	.name		= "mmu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI816X_CM_ALWON_MOD,
	.clkdm_offs	= TI816X_CM_MMU_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain mmu_cfg_816x_clkdm = {
	.name		= "mmu_cfg_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI816X_CM_ALWON_MOD,
	.clkdm_offs	= TI816X_CM_MMUCFG_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain active_gem_816x_clkdm = {
	.name		= "active_gem_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI816X_CM_ACTIVE_MOD,
	.clkdm_offs	= TI816X_CM_ACTIVE_GEM_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain hdvicp0_816x_clkdm = {
	.name		= "hdvicp0_clkdm",
	.pwrdm		= { .name = "hdvicp0_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD0_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD0_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain hdvicp1_816x_clkdm = {
	.name		= "hdvicp1_clkdm",
	.pwrdm		= { .name = "hdvicp1_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD1_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD1_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain hdvicp2_816x_clkdm = {
	.name		= "hdvicp2_clkdm",
	.pwrdm		= { .name = "hdvicp2_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD2_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD2_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain sgx_816x_clkdm = {
	.name		= "sgx_clkdm",
	.pwrdm		= { .name = "sgx_pwrdm" },
	.cm_inst	= TI816X_CM_SGX_MOD,
	.clkdm_offs	= TI816X_CM_SGX_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain default_l3_med_816x_clkdm = {
	.name		= "default_l3_med_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_MED_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain default_ducati_816x_clkdm = {
	.name		= "default_ducati_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_DUCATI_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain default_pcie_816x_clkdm = {
	.name		= "default_pcie_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_PCI_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain default_usb_816x_clkdm = {
	.name		= "default_usb_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_SLOW_CLKDM,
	.clktrctrl_mask	= TI816X_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct clockdomain *clockdomains_ti816x[] __initdata = {
	&alwon_mpu_816x_clkdm,
	&alwon_l3_slow_816x_clkdm,
	&alwon_l3_fast_816x_clkdm,
	&alwon_ethernet_816x_clkdm,
	&mmu_816x_clkdm,
	&mmu_cfg_816x_clkdm,
	&active_gem_816x_clkdm,
	&hdvicp0_816x_clkdm,
	&hdvicp1_816x_clkdm,
	&hdvicp2_816x_clkdm,
	&sgx_816x_clkdm,
	&default_l3_med_816x_clkdm,
	&default_ducati_816x_clkdm,
	&default_pcie_816x_clkdm,
	&default_usb_816x_clkdm,
	NULL,
};

void __init ti816x_clockdomains_init(void)
{
	clkdm_init(clockdomains_ti816x, NULL, &omap3_clkdm_operations);
}
