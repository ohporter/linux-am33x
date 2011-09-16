/*
 * Code for AM335X EVM.
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
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c/at24.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>

#include "mux.h"

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	AM335X_MUX(I2C0_SDA, OMAP_MUX_MODE0 | AM335X_SLEWCTRL_SLOW |
			AM335X_INPUT_EN | AM335X_PIN_OUTPUT),
	AM335X_MUX(I2C0_SCL, OMAP_MUX_MODE0 | AM335X_SLEWCTRL_SLOW |
			AM335X_INPUT_EN | AM335X_PIN_OUTPUT),
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define	board_mux	NULL
#endif

/* module pin mux structure */
struct pinmux_config {
	const char *string_name; /* signal name format */
	int val; /* Options for the mux register value */
};

/*
* @pin_mux - single module pin-mux structure which defines pin-mux
*			details for all its pins.
*/
static void setup_pin_mux(struct pinmux_config *pin_mux)
{
	int i;

	for (i = 0; pin_mux->string_name != NULL; pin_mux++)
		omap_mux_init_signal(pin_mux->string_name, pin_mux->val);

}

static struct omap_board_config_kernel am335x_evm_config[] __initdata = {
};

static void __init am335x_init_early(void)
{
	omap2_init_common_infrastructure();
}

#define BASEBOARD_I2C_ADDR	0x50
#define DAUG_BOARD_I2C_ADDR	0x51
#define LCD_BOARD_I2C_ADDR	0x52

/*
* EVM Config held in On-Board eeprom device.
*
* Header Format
*
*  Name			Size	Contents
*			(Bytes)
*-------------------------------------------------------------
*  Header		4	0xAA, 0x55, 0x33, 0xEE
*
*  Board Name		8	Name for board in ASCII.
*				example "A33515BB" = "AM335X
				Low Cost EVM board"
*
*  Version		4	Hardware version code for board in
*				in ASCII. "1.0A" = rev.01.0A
*
*  Serial Number	12	Serial number of the board. This is a 12
*				character string which is WWYY4P16nnnn, where
*				WW = 2 digit week of the year of production
*				YY = 2 digit year of production
*				nnnn = incrementing board number
*
*  Configuration option	32	Codes(TBD) to show the configuration
*				setup on this board.
*
*  Available		32720	Available space for other non-volatile
*				data.
*/
struct am335x_evm_eeprom_config {
	u32	header;
	u8	name[8];
	u32	version;
	u8	serial[12];
	u8	opt[32];
};

static struct am335x_evm_eeprom_config config;
static bool daughter_brd_detected;

#define AM335X_EEPROM_HEADER		0xEE3355AA

static void am335x_setup_daughter_board(struct memory_accessor *m, void *c)
{
	u8 tmp;
	int ret;

	/*
	 * try reading a byte from the EEPROM to see if it is
	 * present. We could read a lot more, but that would
	 * just slow the boot process and we have all the information
	 * we need from the EEPROM on the base board anyway.
	 */
	ret = m->read(m, &tmp, 0, sizeof(u8));
	if (ret == sizeof(u8)) {
		pr_info("Detected a daughter card on AM335x EVM..");
		daughter_brd_detected = true;
	} else {
		pr_info("No daughter card found\n");
		daughter_brd_detected = false;
	}
}

static void am335x_evm_setup(struct memory_accessor *mem_acc, void *context)
{
	int ret;
	char tmp[10];

	/* get board specific data */
	ret = mem_acc->read(mem_acc, (char *)&config, 0, sizeof(config));
	if (ret != sizeof(config)) {
		pr_warning("AM335X EVM config read fail, read %d bytes\n", ret);
		return;
	}

	if (config.header != AM335X_EEPROM_HEADER) {
		pr_warning("AM335X: wrong header 0x%x, expected 0x%x\n",
			config.header, AM335X_EEPROM_HEADER);
		return;
	}

	if (strncmp("A335", config.name, 4)) {
		pr_err("Board %s doesn't look like an AM335x board\n",
			config.name);
		return;
	}

	snprintf(tmp, sizeof(config.name), "%s", config.name);
	pr_info("Board name: %s\n", tmp);
	/* only 6 characters of options string used for now */
	snprintf(tmp, 7, "%s", config.opt);
	pr_info("SKU: %s\n", tmp);

	return;
}

static struct at24_platform_data am335x_daughter_board_eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
	.setup          = am335x_setup_daughter_board,
	.context        = (void *)NULL,
};

static struct at24_platform_data am335x_baseboard_eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
	.setup          = am335x_evm_setup,
	.context        = (void *)NULL,
};

/*
* Daughter board Detection.
* Every board has a ID memory (EEPROM) on board. We probe these devices at
* machine init, starting from daughter board and ending with baseboard.
* Assumptions :
*	1. probe for i2c devices are called in the order they are included in
*	   the below struct. Daughter boards eeprom are probed 1st. Baseboard
*	   eeprom probe is called last.
*/
static struct i2c_board_info __initdata am335x_i2c_boardinfo[] = {
	{
		/* Daughter Board EEPROM */
		I2C_BOARD_INFO("24c256", DAUG_BOARD_I2C_ADDR),
		.platform_data  = &am335x_daughter_board_eeprom_info,
	},
	{
		/* Baseboard board EEPROM */
		I2C_BOARD_INFO("24c256", BASEBOARD_I2C_ADDR),
		.platform_data  = &am335x_baseboard_eeprom_info,
	},
	{
		I2C_BOARD_INFO("cpld_reg", 0x35),
	},
	{
		I2C_BOARD_INFO("tlc59108", 0x40),
	},

};

static void __init am335x_evm_i2c_init(void)
{
	omap_register_i2c_bus(1, 100, am335x_i2c_boardinfo,
				ARRAY_SIZE(am335x_i2c_boardinfo));
}

static void __init am335x_evm_init(void)
{
	am335x_mux_init(board_mux);
	omap_serial_init();
	am335x_evm_i2c_init();
	omap_sdrc_init(NULL, NULL);
	omap_board_config = am335x_evm_config;
	omap_board_config_size = ARRAY_SIZE(am335x_evm_config);
}

static void __init am335x_evm_map_io(void)
{
	omap2_set_globals_am33xx();
	omapam33xx_map_common_io();
}

MACHINE_START(AM335XEVM, "am335xevm")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= am335x_evm_map_io,
	.init_early	= am335x_init_early,
	.init_irq	= ti81xx_init_irq,
	.timer		= &omap3_am33xx_timer,
	.init_machine	= am335x_evm_init,
MACHINE_END
