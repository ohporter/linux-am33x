/*
 * omap_hwmod_ti816x_data.c - hardware modules data for TI816X chips
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
 *
 */
#include <plat/omap_hwmod.h>
#include <mach/irqs.h>
#include <plat/cpu.h>
#include <plat/dma.h>
#include <plat/serial.h>
#include <plat/l4_3xxx.h>
#include <plat/ti81xx.h>

#include "omap_hwmod_common_data.h"

#include "control.h"
#include "cm816x.h"
#include "cm-regbits-816x.h"

/*
 * TI816X hardware modules integration data
 */

static struct omap_hwmod ti816xx_mpu_hwmod;
static struct omap_hwmod ti816x_l3_fast_hwmod;
static struct omap_hwmod ti816x_l3_med_hwmod;
static struct omap_hwmod ti816x_l4_fast_hwmod;
static struct omap_hwmod ti816x_l3_slow_hwmod;
static struct omap_hwmod ti816x_l4_slow_hwmod;

/* MPU -> L3 FAST */
static struct omap_hwmod_ocp_if ti816x_mpu__l3_fast = {
	.master = &ti816xx_mpu_hwmod,
	.slave	= &ti816x_l3_fast_hwmod,
	.user	= OCP_USER_MPU,
};

/* MPU -> L3 MEDIUM */
static struct omap_hwmod_ocp_if ti816x_mpu__l3_med = {
	.master = &ti816xx_mpu_hwmod,
	.slave	= &ti816x_l3_med_hwmod,
	.user	= OCP_USER_MPU,
};

/* MPU -> L3 SLOW interface */
static struct omap_hwmod_ocp_if ti816x_mpu__l3_slow = {
	.master = &ti816xx_mpu_hwmod,
	.slave	= &ti816x_l3_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* L3 MED -> L4 FAST */
static struct omap_hwmod_ocp_if ti816x_l3_med__l4_fast = {
	.master	= &ti816x_l3_med_hwmod,
	.slave	= &ti816x_l4_fast_hwmod,
	.user	= OCP_USER_MPU,
};

/* L3 SLOW -> L4_SLOW Peripheral interface */
static struct omap_hwmod_ocp_if ti816x_l3_slow__l4_slow = {
	.master	= &ti816x_l3_slow_hwmod,
	.slave	= &ti816x_l4_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* Slave interfaces on the L3 FAST interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_fast_slaves[] = {
	&ti816x_mpu__l3_fast,
};

/* Slave interfaces on the L3 MED interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_med_slaves[] = {
	&ti816x_mpu__l3_med,
};

/* Slave interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_slow_slaves[] = {
	&ti816x_mpu__l3_slow,
};

/* L3 FAST */
static struct omap_hwmod ti816x_l3_fast_hwmod = {
	.name		= "l3_fast",
	.class		= &l3_hwmod_class,
	.slaves		= ti816x_l3_fast_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l3_fast_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.flags		= HWMOD_NO_IDLEST,
};

/* L3 MED */
static struct omap_hwmod ti816x_l3_med_hwmod = {
	.name		= "l3_med",
	.class		= &l3_hwmod_class,
	.slaves		= ti816x_l3_med_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l3_med_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.flags		= HWMOD_NO_IDLEST,
};

/* Slave interfaces on the L4 FAST interconnect */
static struct omap_hwmod_ocp_if *ti816x_l4_fast_slaves[] = {
	&ti816x_l3_med__l4_fast,
};

/* L4 FAST */
static struct omap_hwmod ti816x_l4_fast_hwmod = {
	.name		= "l4_fast",
	.class		= &l4_hwmod_class,
	.slaves		= ti816x_l4_fast_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l4_fast_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.flags		= HWMOD_NO_IDLEST,
};

/* L3 SLOW */
static struct omap_hwmod ti816x_l3_slow_hwmod = {
	.name		= "l3_slow",
	.class		= &l3_hwmod_class,
	.slaves		= ti816x_l3_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l3_slow_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X),
	.flags		= HWMOD_NO_IDLEST,
};

static struct omap_hwmod ti816x_uart1_hwmod;
static struct omap_hwmod ti816x_uart2_hwmod;
static struct omap_hwmod ti816x_uart3_hwmod;

/* L4 SLOW -> UART1 interface */
static struct omap_hwmod_addr_space ti816x_uart1_addr_space[] = {
	{
		.pa_start	= TI81XX_UART1_BASE,
		.pa_end		= TI81XX_UART1_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart1_hwmod,
	.clk		= "uart1_ick",
	.addr		= ti816x_uart1_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART2 interface */
static struct omap_hwmod_addr_space ti816x_uart2_addr_space[] = {
	{
		.pa_start	= TI81XX_UART2_BASE,
		.pa_end		= TI81XX_UART2_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart2_hwmod,
	.clk		= "uart2_ick",
	.addr		= ti816x_uart2_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART3 interface */
static struct omap_hwmod_addr_space ti816x_uart3_addr_space[] = {
	{
		.pa_start	= TI81XX_UART3_BASE,
		.pa_end		= TI81XX_UART3_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart3_hwmod,
	.clk		= "uart3_ick",
	.addr		= ti816x_uart3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod ti816x_timer1_hwmod;
static struct omap_hwmod ti816x_timer2_hwmod;
static struct omap_hwmod ti816x_timer3_hwmod;
static struct omap_hwmod ti816x_timer4_hwmod;
static struct omap_hwmod ti816x_timer5_hwmod;
static struct omap_hwmod ti816x_timer6_hwmod;
static struct omap_hwmod ti816x_timer7_hwmod;

/* L4 SLOW -> TIMER1 interface */
static struct omap_hwmod_addr_space ti816x_timer1_addr_space[] = {
	{
		.pa_start	= 0x4802E000,
		.pa_end		= 0x4802E000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer1_hwmod,
	.clk		= "gpt1_ick",
	.addr		= ti816x_timer1_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> TIMER2 interface */
static struct omap_hwmod_addr_space ti816x_timer2_addr_space[] = {
	{
		.pa_start	= 0x48040000,
		.pa_end		= 0x48040000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer2_hwmod,
	.clk		= "gpt2_ick",
	.addr		= ti816x_timer2_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> TIMER3 interface */
static struct omap_hwmod_addr_space ti816x_timer3_addr_space[] = {
	{
		.pa_start	= 0x48042000,
		.pa_end		= 0x48042000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer3_hwmod,
	.clk		= "gpt3_ick",
	.addr		= ti816x_timer3_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> TIMER2 interface */
static struct omap_hwmod_addr_space ti816x_timer4_addr_space[] = {
	{
		.pa_start	= 0x48044000,
		.pa_end		= 0x48044000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer4 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer4_hwmod,
	.clk		= "gpt4_ick",
	.addr		= ti816x_timer4_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> TIMER5 interface */
static struct omap_hwmod_addr_space ti816x_timer5_addr_space[] = {
	{
		.pa_start	= 0x48046000,
		.pa_end		= 0x48046000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer5 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer5_hwmod,
	.clk		= "gpt5_ick",
	.addr		= ti816x_timer5_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> TIMER6 interface */
static struct omap_hwmod_addr_space ti816x_timer6_addr_space[] = {
	{
		.pa_start	= 0x48048000,
		.pa_end		= 0x48048000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer6 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer6_hwmod,
	.clk		= "gpt6_ick",
	.addr		= ti816x_timer6_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> TIMER7 interface */
static struct omap_hwmod_addr_space ti816x_timer7_addr_space[] = {
	{
		.pa_start	= 0x4804A000,
		.pa_end		= 0x4804A000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer7 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer7_hwmod,
	.clk		= "gpt7_ick",
	.addr		= ti816x_timer7_addr_space,
	.user		= OCP_USER_MPU,
};

/* Slave interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l4_slow_slaves[] = {
	&ti816x_l3_slow__l4_slow,
};

/* L4 SLOW */
static struct omap_hwmod ti816x_l4_slow_hwmod = {
	.name		= "l4_slow",
	.class		= &l4_hwmod_class,
	.slaves		= ti816x_l4_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l4_slow_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X),
	.flags		= HWMOD_NO_IDLEST,
};

/* Master interfaces on the MPU device */
static struct omap_hwmod_ocp_if *ti816x_mpu_masters[] = {
	&ti816x_mpu__l3_fast,
	&ti816x_mpu__l3_med,
	&ti816x_mpu__l3_slow,
};

/* MPU */
static struct omap_hwmod ti816xx_mpu_hwmod = {
	.name		= "mpu",
	.class		= &mpu_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
	.main_clk	= "mpu_ck",
	.masters	= ti816x_mpu_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_mpu_masters),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X),
};

/* UART common */
static struct omap_hwmod_class_sysconfig uart_sysc = {
	.rev_offs	= 0x50,
	.sysc_offs	= 0x54,
	.syss_offs	= 0x58,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class uart_class = {
	.name = "uart",
	.sysc = &uart_sysc,
};

/* UART1 */
static struct omap_hwmod_irq_info uart1_mpu_irqs[] = {
	{ .irq = 72, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info uart1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 26, },
	{ .name = "rx",	.dma_req = 27, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_uart1_slaves[] = {
	&ti816x_l4_slow__uart1,
};

static struct omap_hwmod ti816x_uart1_hwmod = {
	.name		= "uart1",
	.mpu_irqs	= uart1_mpu_irqs,
	.sdma_reqs	= uart1_edma_reqs,
	.main_clk	= "uart1_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_UART_0_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_uart1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart1_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X),
};

/* UART2 */
static struct omap_hwmod_irq_info uart2_mpu_irqs[] = {
	{ .irq = 73, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info uart2_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 28, },
	{ .name = "rx",	.dma_req = 29, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_uart2_slaves[] = {
	&ti816x_l4_slow__uart2,
};

static struct omap_hwmod ti816x_uart2_hwmod = {
	.name		= "uart2",
	.mpu_irqs	= uart2_mpu_irqs,
	.sdma_reqs	= uart2_edma_reqs,
	.main_clk	= "uart2_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_UART_1_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_uart2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart2_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X),
};

/* UART3 */
static struct omap_hwmod_irq_info uart3_mpu_irqs[] = {
	{ .irq = 74, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info uart3_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 30, },
	{ .name = "rx",	.dma_req = 31, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_uart3_slaves[] = {
	&ti816x_l4_slow__uart3,
};

static struct omap_hwmod ti816x_uart3_hwmod = {
	.name		= "uart3",
	.mpu_irqs	= uart3_mpu_irqs,
	.sdma_reqs	= uart3_edma_reqs,
	.main_clk	= "uart3_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_UART_2_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_uart3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart3_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X),
};

/* Timers common */
static struct omap_hwmod_class_sysconfig ti816x_timer_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0010,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type2,
};

static struct omap_hwmod_class ti816x_timer_hwmod_class = {
	.name = "timer",
	.sysc = &ti816x_timer_sysc,
};

/* TIMER1 */
static struct omap_hwmod_irq_info ti816x_timer1_mpu_irqs[] = {
	{ .irq = 67, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer1_slaves[] = {
	&ti816x_l4_slow__timer1,
};

static struct omap_hwmod ti816x_timer1_hwmod = {
	.name		= "timer1",
	.mpu_irqs	= ti816x_timer1_mpu_irqs,
	.main_clk	= "gpt1_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_1_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer1_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};

/* TIMER2 */
static struct omap_hwmod_irq_info ti816x_timer2_mpu_irqs[] = {
	{ .irq = 68, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer2_slaves[] = {
	&ti816x_l4_slow__timer2,
};

static struct omap_hwmod ti816x_timer2_hwmod = {
	.name		= "timer2",
	.mpu_irqs	= ti816x_timer2_mpu_irqs,
	.main_clk	= "gpt2_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_2_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer2_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};

/* TIMER3 */
static struct omap_hwmod_irq_info ti816x_timer3_mpu_irqs[] = {
	{ .irq = 69, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer3_slaves[] = {
	&ti816x_l4_slow__timer3,
};

static struct omap_hwmod ti816x_timer3_hwmod = {
	.name		= "timer3",
	.mpu_irqs	= ti816x_timer3_mpu_irqs,
	.main_clk	= "gpt3_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_3_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer3_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};

/* TIMER4 */
static struct omap_hwmod_irq_info ti816x_timer4_mpu_irqs[] = {
	{ .irq = 92, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer4_slaves[] = {
	&ti816x_l4_slow__timer4,
};

static struct omap_hwmod ti816x_timer4_hwmod = {
	.name		= "timer4",
	.mpu_irqs	= ti816x_timer4_mpu_irqs,
	.main_clk	= "gpt4_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_4_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer4_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};

/* TIMER5 */
static struct omap_hwmod_irq_info ti816x_timer5_mpu_irqs[] = {
	{ .irq = 93, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer5_slaves[] = {
	&ti816x_l4_slow__timer5,
};

static struct omap_hwmod ti816x_timer5_hwmod = {
	.name		= "timer5",
	.mpu_irqs	= ti816x_timer5_mpu_irqs,
	.main_clk	= "gpt5_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_5_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer5_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};

/* TIMER6 */
static struct omap_hwmod_irq_info ti816x_timer6_mpu_irqs[] = {
	{ .irq = 94, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer6_slaves[] = {
	&ti816x_l4_slow__timer6,
};

static struct omap_hwmod ti816x_timer6_hwmod = {
	.name		= "timer6",
	.mpu_irqs	= ti816x_timer6_mpu_irqs,
	.main_clk	= "gpt6_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_6_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer6_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};

/* TIMER7 */
static struct omap_hwmod_irq_info ti816x_timer7_mpu_irqs[] = {
	{ .irq = 95, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_timer7_slaves[] = {
	&ti816x_l4_slow__timer7,
};

static struct omap_hwmod ti816x_timer7_hwmod = {
	.name		= "timer7",
	.mpu_irqs	= ti816x_timer7_mpu_irqs,
	.main_clk	= "gpt7_fck",
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_7_CLKCTRL_OFFSET,
		},
	},
	.slaves		= ti816x_timer7_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer7_slaves),
	.class		= &ti816x_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X)
};


static __initdata struct omap_hwmod *ti816x_hwmods[] = {
	&ti816x_l3_fast_hwmod,
	&ti816x_l3_med_hwmod,
	&ti816x_l4_fast_hwmod,
	&ti816x_l3_slow_hwmod,
	&ti816x_l4_slow_hwmod,
	&ti816xx_mpu_hwmod,
	&ti816x_uart1_hwmod,
	&ti816x_uart2_hwmod,
	&ti816x_uart3_hwmod,
	&ti816x_timer1_hwmod,
	&ti816x_timer2_hwmod,
	&ti816x_timer3_hwmod,
	&ti816x_timer4_hwmod,
	&ti816x_timer5_hwmod,
	&ti816x_timer6_hwmod,
	&ti816x_timer7_hwmod,
	NULL,
};

int __init ti816x_hwmod_init(void)
{
	return omap_hwmod_register(ti816x_hwmods);
}
