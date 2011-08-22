/*
 * AM335X specific clock ops.
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
 *
 * Part of this code are based on code by Hemant Padenkar.
 */

#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <plat/clock.h>
#include <plat/prcm.h>

#include "clock.h"
#include "cm-regbits-335x.h"

#include "prm335x.h"
#include "prm-regbits-335x.h"

#include "prm2xxx_3xxx.h"

/**
 * am335x_dflt_wait_clk_enable() - Enable a am335x module clock
 * @clk: Pointer to the clock to be enabled
 *
 * This function just wraps omap2_dflt_clk_enable with a check for module idle
 * status. We loop till module goes to funcitonal state as the immediate access
 * to module space will not work otherwise.
 */
int am335x_dflt_wait_clk_enable(struct clk *clk)
{
	omap2_dflt_clk_enable(clk);

	omap2_cm_wait_idlest(clk->enable_reg, AM335X_IDLEST_MASK,
			AM335X_IDLEST_VAL, clk->name);

	return 0;
}

int am335x_sgx_clk_enable(struct clk *clk)
{
	omap2_dflt_clk_enable(clk);

	/* De-assert local reset after module enable */
	omap2_prm_clear_mod_reg_bits(AM335X_GFX_RST_MASK,
			AM335X_PRM_GFX_MOD,
			AM335X_RM_GFX_RSTCTRL_OFFSET);

	omap2_cm_wait_idlest(clk->enable_reg, AM335X_IDLEST_MASK,
			AM335X_IDLEST_VAL, clk->name);

	return 0;
}

void am335x_sgx_clk_disable(struct clk *clk)
{
	/* Assert local reset */
	omap2_prm_set_mod_reg_bits(AM335X_GFX_RST_MASK,
			AM335X_PRM_GFX_MOD,
			AM335X_RM_GFX_RSTCTRL_OFFSET);

	omap2_dflt_clk_disable(clk);
}

const struct clkops clkops_am335x_dflt_wait = {
	.enable         = am335x_dflt_wait_clk_enable,
	.disable	= omap2_dflt_clk_disable,
};

const struct clkops clkops_am335x_sgx = {
	.enable         = am335x_sgx_clk_enable,
	.disable        = am335x_sgx_clk_disable,
};
