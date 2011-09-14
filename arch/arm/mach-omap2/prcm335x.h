/*
 * AM33XX PRCM definitions
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * Vaibhav Hiremath <hvaibhav@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file contains macros and functions that are common to all of
 * the PRM/CM/PRCM blocks on the OMAP4 devices: PRM and CM
 */

#ifndef __ARCH_ARM_MACH_OMAP2_PRCM33XX_H
#define __ARCH_ARM_MACH_OMAP2_PRCM33XX_H

/*
 * AM33XX PRCM partition IDs
 *
 * The numbers and order are arbitrary, but 0 is reserved for the
 * 'invalid' partition in case someone forgets to add a
 * .prcm_partition field.
 */
#define AM335X_INVALID_PRCM_PARTITION		0
#define AM335X_PRM_PARTITION			1
#define AM335X_CM_PARTITION			2

/*
 * AM335X_MAX_PRCM_PARTITIONS: set to the highest value of the PRCM partition
 * IDs, plus one
 */
#define AM335X_MAX_PRCM_PARTITIONS		3

#endif
