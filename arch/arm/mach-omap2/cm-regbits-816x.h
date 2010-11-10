/*
 * TI816X CM register bits.
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

#ifndef __ARCH_ARM_MACH_OMAP2_CM_REGBITS_816X_H
#define __ARCH_ARM_MACH_OMAP2_CM_REGBITS_816X_H

#include "cm.h"

#define TI816X_CLKSEL_0_0_SHIFT					0
#define TI816X_CLKSEL_0_0_MASK					(1 << 0)

#define TI816X_CLKSEL_0_1_SHIFT					0
#define TI816X_CLKSEL_0_1_MASK					(3 << 0)

#define TI816X_CLKSEL_0_2_SHIFT					0
#define TI816X_CLKSEL_0_2_MASK					(7 << 0)

/* Modulemode bit */
#define TI816X_MODULEMODE_SWCTRL				1

/* IDLEST bit */
#define TI816X_IDLEST_SHIFT					16
#define TI816X_IDLEST_MASK					(3 << 16)
#define TI816X_IDLEST_VAL					3

/* Used for clockdomain control */
#define TI816X_CLKTRCTRL_SHIFT					0
#define TI816X_CLKTRCTRL_MASK					(3 << 0)

/* TODO: Add other specific CM register bits */

#endif
