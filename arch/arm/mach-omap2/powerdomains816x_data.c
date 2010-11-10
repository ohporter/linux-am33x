/*
 * TI816X Power Domain data.
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

#include "powerdomain.h"
#include "prcm-common.h"
#include "prm2xxx_3xxx.h"

static struct powerdomain alwon_816x_pwrdm = {
	.name		  = "alwon_pwrdm",
	.prcm_offs	  = TI816X_PRM_ALWON_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct powerdomain active_816x_pwrdm = {
	.name		  = "active_pwrdm",
	.prcm_offs	  = TI816X_PRM_ACTIVE_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain default_816x_pwrdm = {
	.name		  = "default_pwrdm",
	.prcm_offs	  = TI816X_PRM_DEFAULT_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain hdvicp0_816x_pwrdm = {
	.name		  = "hdvicp0_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD0_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain hdvicp1_816x_pwrdm = {
	.name		  = "hdvicp1_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD1_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain hdvicp2_816x_pwrdm = {
	.name		  = "hdvicp2_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD2_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain sgx_816x_pwrdm = {
	.name		  = "sgx_pwrdm",
	.prcm_offs	  = TI816X_PRM_SGX_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain *powerdomains_ti816x[] __initdata = {
	&alwon_816x_pwrdm,
	&active_816x_pwrdm,
	&default_816x_pwrdm,
	&hdvicp0_816x_pwrdm,
	&hdvicp1_816x_pwrdm,
	&hdvicp2_816x_pwrdm,
	&sgx_816x_pwrdm,
	NULL
};

void __init ti816x_powerdomains_init(void)
{
	pwrdm_init(powerdomains_ti816x, &ti816x_pwrdm_operations);
}
