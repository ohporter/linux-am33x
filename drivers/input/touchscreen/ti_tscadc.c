/*
 * TI Touch Screen driver
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


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/input/ti_tscadc.h>
#include <linux/delay.h>

#define TSCADC_REG_IRQEOI		0x020
#define TSCADC_REG_RAWIRQSTATUS		0x024
#define TSCADC_REG_IRQSTATUS		0x028
#define TSCADC_REG_IRQENABLE		0x02C
#define TSCADC_REG_IRQWAKEUP		0x034
#define TSCADC_REG_CTRL			0x040
#define TSCADC_REG_ADCFSM		0x044
#define TSCADC_REG_CLKDIV		0x04C
#define TSCADC_REG_SE			0x054
#define TSCADC_REG_IDLECONFIG		0x058
#define TSCADC_REG_CHARGECONFIG		0x05C
#define TSCADC_REG_CHARGEDELAY		0x060
#define TSCADC_REG_STEPCONFIG(n)	(0x64 + ((n-1) * 8))
#define TSCADC_REG_STEPDELAY(n)		(0x68 + ((n-1) * 8))
#define TSCADC_REG_FIFO0CNT		0xE4
#define TSCADC_REG_FIFO0THR		0xE8
#define TSCADC_REG_FIFO1CNT		0xF0
#define TSCADC_REG_FIFO1THR		0xF4
#define TSCADC_REG_FIFO0		0x100
#define TSCADC_REG_FIFO1		0x200

/*	Register Bitfields	*/
#define TSCADC_IRQWKUP_ENB		BIT(0)
#define TSCADC_STPENB_STEPENB		0x1fFF
#define TSCADC_IRQENB_FIFO0THRES	BIT(2)
#define TSCADC_IRQENB_FIFO1THRES	BIT(5)
#define TSCADC_IRQENB_PENUP		BIT(9)
#define TSCADC_STEPCONFIG_MODE_HWSYNC	0x2
#define TSCADC_STEPCONFIG_2SAMPLES_AVG	(1 << 4)
#define TSCADC_STEPCONFIG_XPP		BIT(5)
#define TSCADC_STEPCONFIG_XNN		BIT(6)
#define TSCADC_STEPCONFIG_YPP		BIT(7)
#define TSCADC_STEPCONFIG_YNN		BIT(8)
#define TSCADC_STEPCONFIG_XNP		BIT(9)
#define TSCADC_STEPCONFIG_YPN		BIT(10)
#define TSCADC_STEPCONFIG_RFP		(1 << 12)
#define TSCADC_STEPCONFIG_INM		(1 << 18)
#define TSCADC_STEPCONFIG_INP_4		(1 << 19)
#define TSCADC_STEPCONFIG_INP_5		(1 << 21)
#define TSCADC_STEPCONFIG_FIFO1		(1 << 26)
#define TSCADC_STEPCONFIG_IDLE_INP	(1 << 22)
#define TSCADC_STEPCONFIG_OPENDLY	0x018
#define TSCADC_STEPCONFIG_SAMPLEDLY	0x88
#define TSCADC_STEPCHARGE_INM		BIT(16)
#define TSCADC_STEPCHARGE_INP		BIT(20)
#define TSCADC_STEPCHARGE_RFM		(1 << 23)
#define TSCADC_STEPCHARGE_DELAY		0x1
#define TSCADC_CNTRLREG_TSCSSENB	BIT(0)
#define TSCADC_CNTRLREG_STEPID		BIT(1)
#define TSCADC_CNTRLREG_STEPCONFIGWRT	BIT(2)
#define TSCADC_CNTRLREG_TSCENB		BIT(7)
#define TSCADC_CNTRLREG_4WIRE		(0x1 << 5)
#define TSCADC_CNTRLREG_5WIRE		(0x1 << 6)
#define TSCADC_CNTRLREG_8WIRE		(0x3 << 5)
#define TSCADC_ADCFSM_STEPID		0x10
#define TSCADC_ADCFSM_FSM		BIT(5)

#define ADC_CLK				3000000

#define MAX_12BIT                       ((1 << 12) - 1)

int pen = 1;
unsigned int bckup_x = 0, bckup_y = 0;

struct tscadc {
	struct input_dev	*input;
	int			wires;
	struct clk		*clk;
	int			irq;
	void __iomem		*tsc_base;
};

static unsigned int tscadc_readl(struct tscadc *ts, unsigned int reg)
{
	return readl(ts->tsc_base + reg);
}

static void tscadc_writel(struct tscadc *tsc, unsigned int reg,
					unsigned int val)
{
	writel(val, tsc->tsc_base + reg);
}

static void tsc_step_config(struct tscadc *ts_dev)
{
	unsigned int	stepconfigx = 0, stepconfigy = 0;
	unsigned int	delay, chargeconfig = 0;
	int i;

	/* Configure the Step registers */

	delay = TSCADC_STEPCONFIG_SAMPLEDLY | TSCADC_STEPCONFIG_OPENDLY;

	stepconfigx = TSCADC_STEPCONFIG_MODE_HWSYNC |
			TSCADC_STEPCONFIG_2SAMPLES_AVG | TSCADC_STEPCONFIG_XPP |
			TSCADC_STEPCONFIG_YPN;
	switch (ts_dev->wires) {
	case 4:
		stepconfigx |= TSCADC_STEPCONFIG_INP_4;
		break;
	case 5:
		stepconfigx |= TSCADC_STEPCONFIG_YPP |
				TSCADC_STEPCONFIG_YNN |
				TSCADC_STEPCONFIG_INP_5;
		break;
	case 8:
		stepconfigx |= TSCADC_STEPCONFIG_INP_4;
		break;
	}
	for (i = 1; i < 7; i++) {
		tscadc_writel(ts_dev, TSCADC_REG_STEPCONFIG(i), stepconfigx);
		tscadc_writel(ts_dev, TSCADC_REG_STEPDELAY(i), delay);
	}

	stepconfigy = TSCADC_STEPCONFIG_MODE_HWSYNC |
			TSCADC_STEPCONFIG_2SAMPLES_AVG | TSCADC_STEPCONFIG_YNN |
			TSCADC_STEPCONFIG_INM | TSCADC_STEPCONFIG_FIFO1;
	switch (ts_dev->wires) {
	case 4:
		stepconfigy |= TSCADC_STEPCONFIG_XNP;
		break;
	case 5:
		stepconfigy |= TSCADC_STEPCONFIG_XPP |
				TSCADC_STEPCONFIG_XNP |
				TSCADC_STEPCONFIG_YPN |
				TSCADC_STEPCONFIG_INP_5;
		break;
	case 8:
		stepconfigy |= TSCADC_STEPCONFIG_XNP;
		break;
	}
	for (i = 7; i < 13; i++) {
		tscadc_writel(ts_dev, TSCADC_REG_STEPCONFIG(i), stepconfigy);
		tscadc_writel(ts_dev, TSCADC_REG_STEPDELAY(i), delay);
	}

	chargeconfig = TSCADC_STEPCONFIG_XPP |
			TSCADC_STEPCONFIG_YNN |
			TSCADC_STEPCONFIG_RFP |
			TSCADC_STEPCHARGE_INM | TSCADC_STEPCHARGE_INP |
			TSCADC_STEPCHARGE_RFM;
	tscadc_writel(ts_dev, TSCADC_REG_CHARGECONFIG, chargeconfig);
	tscadc_writel(ts_dev, TSCADC_REG_CHARGEDELAY, TSCADC_STEPCHARGE_DELAY);

	tscadc_writel(ts_dev, TSCADC_REG_SE, TSCADC_STPENB_STEPENB);
}

static void tsc_idle_config(struct tscadc *ts_config)
{
	/* Idle mode touch screen config */
	unsigned int	 idleconfig;

	idleconfig = TSCADC_STEPCONFIG_YNN |
				TSCADC_STEPCONFIG_XNN |
				TSCADC_STEPCONFIG_INM;

	switch (ts_config->wires) {
	case 4:
		idleconfig |= TSCADC_STEPCONFIG_IDLE_INP;
		break;
	case 5:
		idleconfig |= TSCADC_STEPCONFIG_INP_5;
		break;
	case 8:
		idleconfig |= TSCADC_STEPCONFIG_INP_4;
		break;
	}
	tscadc_writel(ts_config, TSCADC_REG_IDLECONFIG, idleconfig);
}

static irqreturn_t tscadc_interrupt(int irq, void *dev)
{
	struct tscadc		*ts_dev = (struct tscadc *)dev;
	struct input_dev	*input_dev = ts_dev->input;
	unsigned int		status, irqclr = 0;
	int			i;
	int			fsm = 0, fifo0count = 0, fifo1count = 0;
	unsigned int		readx1 = 0, ready1 = 0;
	unsigned int		prev_val_x = ~0, prev_val_y = ~0;
	unsigned int		prev_diff_x = ~0, prev_diff_y = ~0;
	unsigned int		cur_diff_x = 0, cur_diff_y = 0;
	unsigned int		val_x = 0, val_y = 0, diffx = 0, diffy = 0;

	status = tscadc_readl(ts_dev, TSCADC_REG_IRQSTATUS);

	if (status & TSCADC_IRQENB_FIFO1THRES) {
		fifo0count = tscadc_readl(ts_dev, TSCADC_REG_FIFO0CNT);
		fifo1count = tscadc_readl(ts_dev, TSCADC_REG_FIFO1CNT);

		for (i = 0; i < fifo0count; i++) {
			readx1 = tscadc_readl(ts_dev, TSCADC_REG_FIFO0);
			readx1 = readx1 & 0xfff;
			if (readx1 > prev_val_x)
				cur_diff_x = readx1 - prev_val_x;
			else
				cur_diff_x = prev_val_x - readx1;

			if (cur_diff_x < prev_diff_x) {
				prev_diff_x = cur_diff_x;
				val_x = readx1;
			}

			prev_val_x = readx1;
			ready1 = tscadc_readl(ts_dev, TSCADC_REG_FIFO1);
				ready1 &= 0xfff;
			if (ready1 > prev_val_y)
				cur_diff_y = ready1 - prev_val_y;
			else
				cur_diff_y = prev_val_y - ready1;

			if (cur_diff_y < prev_diff_y) {
				prev_diff_y = cur_diff_y;
				val_y = ready1;
			}

			prev_val_y = ready1;
		}

		if (val_x > bckup_x) {
			diffx = val_x - bckup_x;
			diffy = val_y - bckup_y;
		} else {
			diffx = bckup_x - val_x;
			diffy = bckup_y - val_y;
		}
		bckup_x = val_x;
		bckup_y = val_y;

		if (pen == 0) {
			if ((diffx < 15) && (diffy < 15)) {
				input_report_abs(input_dev, ABS_X, val_x);
				input_report_abs(input_dev, ABS_Y, val_y);
				input_report_key(input_dev, BTN_TOUCH, 1);
				input_sync(input_dev);
			}
		}
		irqclr |= TSCADC_IRQENB_FIFO1THRES;
	}

	udelay(315);

	status = tscadc_readl(ts_dev, TSCADC_REG_RAWIRQSTATUS);
	if (status & TSCADC_IRQENB_PENUP) {
		/* Pen up event */
		fsm = tscadc_readl(ts_dev, TSCADC_REG_ADCFSM);
		if (fsm == 0x10) {
			pen = 1;
			bckup_x = 0;
			bckup_y = 0;
			input_report_key(input_dev, BTN_TOUCH, 0);
			input_sync(input_dev);
		} else {
			pen = 0;
		}
		irqclr |= TSCADC_IRQENB_PENUP;
	}

	tscadc_writel(ts_dev, TSCADC_REG_IRQSTATUS, irqclr);

	/* check pending interrupts */
	tscadc_writel(ts_dev, TSCADC_REG_IRQEOI, 0x0);

	tscadc_writel(ts_dev, TSCADC_REG_SE, TSCADC_STPENB_STEPENB);
	return IRQ_HANDLED;
}

/*
* The functions for inserting/removing driver as a module.
*/

static	int __devinit tscadc_probe(struct platform_device *pdev)
{
	struct tscadc			*ts_dev;
	struct input_dev		*input_dev;
	int				err;
	int				clk_value;
	int				clock_rate, irqenable, ctrl;
	struct	tsc_data		*pdata = pdev->dev.platform_data;
	struct resource			*res;
	struct clk			*tsc_ick;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "no memory resource defined.\n");
		return -EINVAL;
	}

	/* Allocate memory for device */
	ts_dev = kzalloc(sizeof(struct tscadc), GFP_KERNEL);
	if (!ts_dev) {
		dev_err(&pdev->dev, "failed to allocate memory.\n");
		return -ENOMEM;
	}

	ts_dev->irq = platform_get_irq(pdev, 0);
	if (ts_dev->irq < 0) {
		dev_err(&pdev->dev, "no irq ID is specified.\n");
		return -ENODEV;
	}

	input_dev = input_allocate_device();
	if (!input_dev) {
		dev_err(&pdev->dev, "failed to allocate input device.\n");
		err = -ENOMEM;
		goto err_free_mem;
	}
	ts_dev->input = input_dev;

	ts_dev->tsc_base = ioremap(res->start, resource_size(res));
	if (!ts_dev->tsc_base) {
		dev_err(&pdev->dev, "failed to map registers.\n");
		err = -ENOMEM;
		goto err_release_mem;
	}

	err = request_irq(ts_dev->irq, tscadc_interrupt, IRQF_DISABLED,
				pdev->dev.driver->name, ts_dev);
	if (err) {
		dev_err(&pdev->dev, "failed to allocate irq.\n");
		goto err_unmap_regs;
	}

	tsc_ick = clk_get(&pdev->dev, "adc_tsc_ick");
	if (IS_ERR(tsc_ick)) {
		dev_err(&pdev->dev, "failed to get TSC ick\n");
		goto err_free_irq;
	}
	clk_enable(tsc_ick);

	ts_dev->clk = clk_get(&pdev->dev, "adc_tsc_fck");
	if (IS_ERR(ts_dev->clk)) {
		dev_err(&pdev->dev, "failed to get TSC fck\n");
		err = PTR_ERR(ts_dev->clk);
		goto err_free_irq;
	}
	clock_rate = clk_get_rate(ts_dev->clk);
	clk_value = clock_rate / ADC_CLK;
	if (clk_value < 7) {
		dev_err(&pdev->dev, "clock input less than min clock requirement\n");
		goto err_fail;
	}
	/* TSCADC_CLKDIV needs to be configured to the value minus 1 */
	clk_value = clk_value - 1;
	tscadc_writel(ts_dev, TSCADC_REG_CLKDIV, clk_value);

	 /* Enable wake-up of the SoC using touchscreen */
	tscadc_writel(ts_dev, TSCADC_REG_IRQWAKEUP, TSCADC_IRQWKUP_ENB);

	ts_dev->wires = pdata->wires;

	/* Set the control register bits */
	ctrl = TSCADC_CNTRLREG_STEPCONFIGWRT |
			TSCADC_CNTRLREG_TSCENB |
			TSCADC_CNTRLREG_STEPID;
	switch (ts_dev->wires) {
	case 4:
		ctrl |= TSCADC_CNTRLREG_4WIRE;
		break;
	case 5:
		ctrl |= TSCADC_CNTRLREG_5WIRE;
		break;
	case 8:
		ctrl |= TSCADC_CNTRLREG_8WIRE;
		break;
	}
	tscadc_writel(ts_dev, TSCADC_REG_CTRL, ctrl);

	/* Set register bits for Idel Config Mode */
	tsc_idle_config(ts_dev);

	/* IRQ Enable */
	irqenable = TSCADC_IRQENB_FIFO1THRES;
	tscadc_writel(ts_dev, TSCADC_REG_IRQENABLE, irqenable);

	tsc_step_config(ts_dev);

	tscadc_writel(ts_dev, TSCADC_REG_FIFO1THR, 5);

	ctrl |= TSCADC_CNTRLREG_TSCSSENB;
	tscadc_writel(ts_dev, TSCADC_REG_CTRL, ctrl);

	input_dev->name = "ti-tsc-adcc";
	input_dev->dev.parent = &pdev->dev;

	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);

	input_set_abs_params(input_dev, ABS_X, 0, MAX_12BIT, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, MAX_12BIT, 0, 0);

	/* register to the input system */
	err = input_register_device(input_dev);
	if (err)
		goto err_fail;

	return 0;

err_fail:
	clk_disable(ts_dev->clk);
	clk_put(ts_dev->clk);
err_free_irq:
	free_irq(ts_dev->irq, ts_dev);
err_unmap_regs:
	iounmap(ts_dev->tsc_base);
err_release_mem:
	release_mem_region(res->start, resource_size(res));
	input_free_device(ts_dev->input);
err_free_mem:
	kfree(ts_dev);
	return err;
}

static int __devexit tscadc_remove(struct platform_device *pdev)
{
	struct tscadc		*ts_dev = dev_get_drvdata(&pdev->dev);
	struct resource		*res;

	free_irq(ts_dev->irq, ts_dev);

	input_unregister_device(ts_dev->input);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	iounmap(ts_dev->tsc_base);
	release_mem_region(res->start, resource_size(res));

	clk_disable(ts_dev->clk);
	clk_put(ts_dev->clk);

	kfree(ts_dev);

	return 0;
}

static struct platform_driver ti_tsc_driver = {
	.probe	  = tscadc_probe,
	.remove	 = __devexit_p(tscadc_remove),
	.driver	 = {
		.name   = "tsc",
	},
};

static int __init ti_tsc_init(void)
{
	return platform_driver_register(&ti_tsc_driver);
}

static void __exit ti_tsc_exit(void)
{
	platform_driver_unregister(&ti_tsc_driver);
}

module_init(ti_tsc_init);
module_exit(ti_tsc_exit);
