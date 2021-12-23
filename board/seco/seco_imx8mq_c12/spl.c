// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/ddr.h>
#include <asm/arch/imx8mq_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/clock.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <power/pmic.h>
#include <power/pfuze100_pmic.h>
#include <spl.h>
#include "../common/pfuze.h"
#include <seco/seco_imx8mq_c12_setup.h>

DECLARE_GLOBAL_DATA_PTR;

extern struct dram_timing_info dram_timing_b0;
extern struct dram_timing_info dram_timing_d0;

void spl_dram_init(void)
{
	if(c12_get_hw_revision() == 0x2) {
		if(C12_IS_1GB) {
			printf("C12 RAM 1GB revD0\n");
			 /* DDR Parameter */
			dram_timing_d0.ddrc_cfg[6].val = 0x610090;
			dram_timing_d0.ddrc_cfg[22].val = 0x96;
			dram_timing_d0.ddrc_cfg[43].val = 0x0f070707;
			dram_timing_d0.ddrc_cfg[47].val = 0xc0012;
			dram_timing_d0.ddrc_cfg[62].val = 0x13;
			dram_timing_d0.ddrc_cfg[71].val = 0x30005;
			dram_timing_d0.ddrc_cfg[71].val = 0x30005;
			dram_timing_d0.ddrc_cfg[86].val = 0x5;
		}
		if(C12_IS_2GB) {
			printf("C12 RAM 2GB revD0\n");
			/* DDR Parameter */
			/* leave lpdd4_timing_d0.c value */
		}
		if(C12_IS_4GB) {
			printf("C12 RAM 4GB revD0\n");
			/* DDR Parameter */
			dram_timing_d0.ddrc_cfg[2].val = 0xa3080020;
			dram_timing_d0.ddrc_cfg[38].val = 0x17;
			/* Training */
			dram_timing_d0.fsp_msg[0].fsp_cfg[8].val = 0x310;
			dram_timing_d0.fsp_msg[1].fsp_cfg[9].val = 0x310;
			dram_timing_d0.fsp_msg[2].fsp_cfg[9].val = 0x310;
			dram_timing_d0.fsp_msg[3].fsp_cfg[11].val = 0x310;
			dram_timing_d0.fsp_msg[0].fsp_cfg[20].val = 0x3;
			dram_timing_d0.fsp_msg[1].fsp_cfg[21].val = 0x3;
			dram_timing_d0.fsp_msg[2].fsp_cfg[21].val = 0x3;
			dram_timing_d0.fsp_msg[3].fsp_cfg[23].val = 0x3;
		}

		ddr_init(&dram_timing_d0);

	} else {
	
		/* ddr init */
		if ((get_cpu_rev() & 0xfff) == CHIP_REV_2_1) {
			if(C12_IS_1GB) {
				printf("C12 RAM 1GB\n");
				/* DDR Parameter */
				dram_timing.ddrc_cfg[2].val = 0xa1080020;
				dram_timing.ddrc_cfg[36].val = 0x0000001f;
				dram_timing.ddrc_cfg[41].val = 0x0f070707; 
				/* Training */
				dram_timing.fsp_msg[0].fsp_cfg[19].val = 0x110;	
				dram_timing.fsp_msg[1].fsp_cfg[19].val = 0x110;	
				dram_timing.fsp_msg[2].fsp_cfg[19].val = 0x110;	
				dram_timing.fsp_msg[3].fsp_cfg[19].val = 0x110;	
				dram_timing.fsp_msg[0].fsp_cfg[45].val = 0x1;	
				dram_timing.fsp_msg[1].fsp_cfg[45].val = 0x1;	
				dram_timing.fsp_msg[2].fsp_cfg[45].val = 0x1;	
				dram_timing.fsp_msg[3].fsp_cfg[45].val = 0x1;	
				
			}
			if(C12_IS_2GB) {
				printf("C12 RAM 2GB\n");
				/* DDR Parameter */
				dram_timing.ddrc_cfg[2].val = 0xa1080020;
				dram_timing.ddrc_cfg[36].val = 0x0000001f;
				dram_timing.ddrc_cfg[41].val = 0x07070707;
				/* Training */
				dram_timing.fsp_msg[0].fsp_cfg[19].val = 0x110;
				dram_timing.fsp_msg[1].fsp_cfg[19].val = 0x110;
				dram_timing.fsp_msg[2].fsp_cfg[19].val = 0x110;
				dram_timing.fsp_msg[3].fsp_cfg[19].val = 0x110;
				dram_timing.fsp_msg[0].fsp_cfg[45].val = 0x1;	
				dram_timing.fsp_msg[1].fsp_cfg[45].val = 0x1;	
				dram_timing.fsp_msg[2].fsp_cfg[45].val = 0x1;	
				dram_timing.fsp_msg[3].fsp_cfg[45].val = 0x1;	
			}
			if(C12_IS_4GB) {
				printf("C12 RAM 4GB\n");
				/* DDR Parameter */
				dram_timing.ddrc_cfg[2].val = 0xa3080020;
				dram_timing.ddrc_cfg[36].val = 0x00000017;
				dram_timing.ddrc_cfg[41].val = 0x07070707;
				/* Training */
				dram_timing.fsp_msg[0].fsp_cfg[19].val = 0x310;
				dram_timing.fsp_msg[1].fsp_cfg[19].val = 0x310;
				dram_timing.fsp_msg[2].fsp_cfg[19].val = 0x310;
				dram_timing.fsp_msg[3].fsp_cfg[19].val = 0x310;
				dram_timing.fsp_msg[0].fsp_cfg[45].val = 0x3;	
				dram_timing.fsp_msg[1].fsp_cfg[45].val = 0x3;	
				dram_timing.fsp_msg[2].fsp_cfg[45].val = 0x3;	
				dram_timing.fsp_msg[3].fsp_cfg[45].val = 0x3;	
			}
			
			ddr_init(&dram_timing);
		} else {
			if(C12_IS_1GB) {
				printf("C12 RAM 1GB\n");
				/* DDR Parameter */
				dram_timing_b0.ddrc_cfg[2].val = 0xa1080020;
				dram_timing_b0.ddrc_cfg[36].val = 0x0000001f;
				dram_timing_b0.ddrc_cfg[41].val = 0x0f070707; 
				/* Training */
				dram_timing_b0.fsp_msg[0].fsp_cfg[19].val = 0x110;	
				dram_timing_b0.fsp_msg[1].fsp_cfg[19].val = 0x110;	
				dram_timing_b0.fsp_msg[2].fsp_cfg[19].val = 0x110;	
				dram_timing_b0.fsp_msg[0].fsp_cfg[45].val = 0x1;	
				dram_timing_b0.fsp_msg[1].fsp_cfg[44].val = 0x1;	
				dram_timing_b0.fsp_msg[2].fsp_cfg[46].val = 0x1;	
				
			}
			if(C12_IS_2GB) {
				printf("C12 RAM 2GB\n");
				/* DDR Parameter */
				dram_timing_b0.ddrc_cfg[2].val = 0xa3080020;
				dram_timing_b0.ddrc_cfg[36].val = 0x00000016;
				dram_timing_b0.ddrc_cfg[41].val = 0x0f070707;
				/* Training */
				dram_timing_b0.fsp_msg[0].fsp_cfg[19].val = 0x310;
				dram_timing_b0.fsp_msg[1].fsp_cfg[19].val = 0x310;
				dram_timing_b0.fsp_msg[2].fsp_cfg[19].val = 0x310;
				dram_timing_b0.fsp_msg[0].fsp_cfg[45].val = 0x3;	
				dram_timing_b0.fsp_msg[1].fsp_cfg[44].val = 0x3;	
				dram_timing_b0.fsp_msg[2].fsp_cfg[46].val = 0x3;	
			}
			if(C12_IS_4GB) {
				printf("C12 RAM 4GB\n");
				/* DDR Parameter */
				dram_timing_b0.ddrc_cfg[2].val = 0xa3080020;
				dram_timing_b0.ddrc_cfg[36].val = 0x00000017;
				dram_timing_b0.ddrc_cfg[41].val = 0x07070707;
				/* Training */
				dram_timing_b0.fsp_msg[0].fsp_cfg[19].val = 0x310;
				dram_timing_b0.fsp_msg[1].fsp_cfg[19].val = 0x310;
				dram_timing_b0.fsp_msg[2].fsp_cfg[19].val = 0x310;
				dram_timing_b0.fsp_msg[0].fsp_cfg[45].val = 0x3;	
				dram_timing_b0.fsp_msg[1].fsp_cfg[44].val = 0x3;	
				dram_timing_b0.fsp_msg[2].fsp_cfg[46].val = 0x3;	
			}

			ddr_init(&dram_timing_b0);
		}
	}
}

#define I2C_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE)
#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)
struct i2c_pads_info i2c_pad_info1 = {
	.scl = {
		.i2c_mode = IMX8MQ_PAD_I2C1_SCL__I2C1_SCL | PC,
		.gpio_mode = IMX8MQ_PAD_I2C1_SCL__GPIO5_IO14 | PC,
		.gp = IMX_GPIO_NR(5, 14),
	},
	.sda = {
		.i2c_mode = IMX8MQ_PAD_I2C1_SDA__I2C1_SDA | PC,
		.gpio_mode = IMX8MQ_PAD_I2C1_SDA__GPIO5_IO15 | PC,
		.gp = IMX_GPIO_NR(5, 15),
	},
};

/*
        MICROCONTROLLER  
*/

#define FW_ID_REV_REG_L                 0x00
#define FW_ID_REV_REG_H                 0x01
#define JUMP_REG_L			0x02
#define JUMP_REG_H			0x03

#define SYS_I2C_MICRO_ADDR              0x40
#define SYS_I2C_MICRO_BUS_NUM		0

#define USDHC2_CD_GPIO	IMX_GPIO_NR(2, 12)
#define USDHC1_PWR_GPIO IMX_GPIO_NR(2, 10)
#define USDHC2_PWR_GPIO IMX_GPIO_NR(2, 19)

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
	case USDHC1_BASE_ADDR:
		ret = 1;
		break;
	case USDHC2_BASE_ADDR:
		ret = !gpio_get_value(USDHC2_CD_GPIO);
		return ret;
	}

	return 1;
}

#define USDHC_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE | \
			 PAD_CTL_FSEL2 | PAD_CTL_DSE7)
#define USDHC_CLK_PAD_CTRL2	(PAD_CTL_DSE2 | PAD_CTL_HYS | PAD_CTL_PUE | PAD_CTL_DSE4)
#define USDHC_PAD_CTRL2		(PAD_CTL_DSE1 | PAD_CTL_HYS | PAD_CTL_PUE | PAD_CTL_DSE4)
#define USDHC_GPIO_PAD_CTRL (PAD_CTL_PUE | PAD_CTL_DSE1)

static iomux_v3_cfg_t const usdhc1_pads[] = {
	IMX8MQ_PAD_SD1_CLK__USDHC1_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_CMD__USDHC1_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA0__USDHC1_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA1__USDHC1_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA2__USDHC1_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA3__USDHC1_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA4__USDHC1_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA5__USDHC1_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA6__USDHC1_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA7__USDHC1_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_RESET_B__GPIO2_IO10 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc2_pads[] = {
	IMX8MQ_PAD_SD2_CLK__USDHC2_CLK | MUX_PAD_CTRL(USDHC_CLK_PAD_CTRL2), /* 0xc6 */
	IMX8MQ_PAD_SD2_CMD__USDHC2_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL2), /* 0xc5 */
	IMX8MQ_PAD_SD2_DATA0__USDHC2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL2), /* 0xc5 */
	IMX8MQ_PAD_SD2_DATA1__USDHC2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL2), /* 0xc5 */
	IMX8MQ_PAD_SD2_DATA2__USDHC2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL2), /* 0xc5 */
	IMX8MQ_PAD_SD2_DATA3__USDHC2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL2), /* 0xc5 */
	IMX8MQ_PAD_SD2_CD_B__GPIO2_IO12 | MUX_PAD_CTRL(USDHC_GPIO_PAD_CTRL),
	IMX8MQ_PAD_SD2_RESET_B__GPIO2_IO19 | MUX_PAD_CTRL(USDHC_GPIO_PAD_CTRL),
};

static struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC1_BASE_ADDR, 0, 8},
	{USDHC2_BASE_ADDR, 0, 4},
};

int board_mmc_init(bd_t *bis)
{
	int i, ret;
	/*
	 * According to the board_mmc_init() the following map is done:
	 * (U-Boot device node)    (Physical Port)
	 * mmc0                    USDHC1
	 * mmc1                    USDHC2
	 */
	for (i = 0; i < CONFIG_SYS_FSL_USDHC_NUM; i++) {
		switch (i) {
		case 0:
			init_clk_usdhc(0);
			usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC_CLK);
			imx_iomux_v3_setup_multiple_pads(usdhc1_pads,
							 ARRAY_SIZE(usdhc1_pads));
			gpio_request(USDHC1_PWR_GPIO, "usdhc1_reset");
			gpio_direction_output(USDHC1_PWR_GPIO, 0);
			udelay(500);
			gpio_direction_output(USDHC1_PWR_GPIO, 1);
			break;
		case 1:
			init_clk_usdhc(1);
			usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
			imx_iomux_v3_setup_multiple_pads(usdhc2_pads,
							 ARRAY_SIZE(usdhc2_pads));
			gpio_request(USDHC2_PWR_GPIO, "usdhc2_reset");
			gpio_direction_output(USDHC2_PWR_GPIO, 0);
			udelay(500);
			gpio_direction_output(USDHC2_PWR_GPIO, 1);
			break;
		default:
			printf("Warning: you configured more USDHC controllers(%d) than supported by the board\n", i + 1);
			return -EINVAL;
		}

		ret = fsl_esdhc_initialize(bis, &usdhc_cfg[i]);
		if (ret)
			return ret;
	}

	return 0;
}

#ifdef CONFIG_POWER
#define I2C_PMIC	0
int power_init_board(void)
{
	struct pmic *p;
	int ret;
	unsigned int reg;

	ret = power_pfuze100_init(I2C_PMIC);
	if (ret)
		return -ENODEV;

	p = pmic_get("PFUZE100");
	ret = pmic_probe(p);
	if (ret)
		return -ENODEV;

	pmic_reg_read(p, PFUZE100_DEVICEID, &reg);
	printf("SPL PMIC:  PFUZE100 ID=0x%02x\n", reg);

	pmic_reg_read(p, PFUZE100_SW3AVOL, &reg);
	if ((reg & 0x3f) != 0x18) {
		reg &= ~0x3f;
		reg |= 0x18;
		pmic_reg_write(p, PFUZE100_SW3AVOL, reg);
	}
	
	/* C12 revD - increase GPU voltage to 1.0V */
	if(c12_get_hw_revision() == 0x2) {
		pmic_reg_read(p, PFUZE100_SW1ABVOL, &reg);
		if ((reg & 0x3f) != 0x1c) {
			reg &= ~0x3f;
			reg |= 0x1c;
			pmic_reg_write(p, PFUZE100_SW1ABVOL, reg);
		}
	}

	pmic_reg_write(p, PFUZE100_SWBSTCON1, 0x49);

	ret = pfuze_mode_init(p, APS_PFM);
	if (ret < 0)
		return ret;

	/* set SW3A standby mode to off */
	pmic_reg_read(p, PFUZE100_SW3AMODE, &reg);
	reg &= ~0xf;
	reg |= APS_OFF;
	pmic_reg_write(p, PFUZE100_SW3AMODE, reg);

	return 0;
}
#endif

void micro_init(void) {

	u8 fw_version_l,fw_version_h;
	u8 pwd_l=0xc0,pwd_h=0x5e,jmp=1;
        printf("C12 Micro fw: ");
	/* Share Micro information beetwen SPL and u-Boot */
	struct mx8_spl_data *data = (struct mx8_spl_data *)
		((CONFIG_SPL_TEXT_BASE - sizeof(struct mx8_spl_data)) & ~0xf);

	i2c_read(SYS_I2C_MICRO_ADDR, FW_ID_REV_REG_L, 1, &fw_version_l, sizeof(fw_version_l));
	i2c_read(SYS_I2C_MICRO_ADDR, FW_ID_REV_REG_H, 1, &fw_version_h, sizeof(fw_version_h));
        printf("0x%x\n",fw_version_l | fw_version_h << 8);
	data->fw_revision = fw_version_l | fw_version_h << 8;

	if(fw_version_h == 0x0b) {
		printf("C12 Micro: detected bootloader mode, jumping to firmware mode");
		i2c_write(SYS_I2C_MICRO_ADDR, JUMP_REG_L, 1, &pwd_l, 1);
		i2c_write(SYS_I2C_MICRO_ADDR, JUMP_REG_H, 1, &pwd_h, 1);
		i2c_write(SYS_I2C_MICRO_ADDR, 0x31, 1, &jmp, 1);
	}
		
	
}

void spl_board_init(void)
{
#ifndef CONFIG_SPL_USB_SDP_SUPPORT
	/* Serial download mode */
	if (is_usb_boot()) {
		puts("Back to ROM, SDP\n");
		restore_boot_params();
	}
#endif

	init_usb_clk();

	puts("Normal Boot\n");
}

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	/* Just empty function now - can't decide what to choose */
	debug("%s: %s\n", __func__, name);

	return 0;
}
#endif

void board_init_f(ulong dummy)
{
	int ret;

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	arch_cpu_init();

	init_uart_clk(1);

	board_early_init_f();

	timer_init();

	preloader_console_init();

	ret = spl_init();
	if (ret) {
		debug("spl_init() failed: %d\n", ret);
		hang();
	}

	enable_tzc380();

	/* Adjust pmic voltage to 1.0V for 800M */
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);

	power_init_board();

	/*Micro Check*/
	micro_init();

	/* DDR initialization */
	spl_dram_init();

	board_init_r(NULL, 0);
}
