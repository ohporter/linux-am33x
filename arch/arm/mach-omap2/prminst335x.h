/*
 * AM335X Power/Reset Management (PRM) function prototypes
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef __ARCH_ASM_MACH_OMAP2_PRMINST335X_H
#define __ARCH_ASM_MACH_OMAP2_PRMINST335X_H

/*
 * In an ideal world, we would not export these low-level functions,
 * but this will probably take some time to fix properly
 */
extern u32 am335x_prminst_read_inst_reg(s16 inst, u16 idx);
extern void am335x_prminst_write_inst_reg(u32 val, s16 inst, u16 idx);
extern u32 am335x_prminst_rmw_inst_reg_bits(u32 mask, u32 bits,
					   s16 inst, s16 idx);

extern void am335x_prm_global_warm_sw_reset(void);

#endif
