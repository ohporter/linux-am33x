/*
 * AM335X clock function prototypes and macros.
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

#ifndef __ARCH_ARM_MACH_OMAP2_CLOCKAM335X_H
#define __ARCH_ARM_MACH_OMAP2_CLOCKAM335X_H

/*
 * XXX Missing values for the OMAP4 DPLL_USB
 * XXX Missing min_multiplier values for all OMAP4 DPLLs
 */
#define AM335X_MAX_DPLL_MULT  2047
#define AM335X_MAX_DPLL_DIV   128


int am335x_clk_init(void);

extern const struct clkops clkops_am335x_dflt_wait;
extern const struct clkops clkops_am335x_sgx;

#endif
