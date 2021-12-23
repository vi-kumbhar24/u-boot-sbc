// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018 NXP
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/imx8mq_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/clock.h>
#include <asm/mach-imx/video.h>
#include <asm/arch/video_common.h> 
#include <spl.h>
#include <power/pmic.h>
#include <power/pfuze100_pmic.h>
#include "../common/pfuze.h"
#include <usb.h>
#include <dwc3-uboot.h>
#include <seco_imx8_env.h>

DECLARE_GLOBAL_DATA_PTR;

#define QSPI_PAD_CTRL	(PAD_CTL_DSE2 | PAD_CTL_HYS)

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)

#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE)

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MQ_PAD_GPIO1_IO02__WDOG1_WDOG_B | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

#define HDMI_GPIO IMX_GPIO_NR(4, 15)
#define BOOST_GPIO IMX_GPIO_NR(4, 19)

static iomux_v3_cfg_t const regulator_enable_pads[] = {
		IMX8MQ_PAD_SAI1_TXD3__GPIO4_IO15 | MUX_PAD_CTRL(WDOG_PAD_CTRL), 
		IMX8MQ_PAD_SAI1_TXD7__GPIO4_IO19 | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};


#ifdef CONFIG_FSL_QSPI
static iomux_v3_cfg_t const qspi_pads[] = {
	IMX8MQ_PAD_NAND_ALE__QSPI_A_SCLK | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MQ_PAD_NAND_CE0_B__QSPI_A_SS0_B | MUX_PAD_CTRL(QSPI_PAD_CTRL),

	IMX8MQ_PAD_NAND_DATA00__QSPI_A_DATA0 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA01__QSPI_A_DATA1 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA02__QSPI_A_DATA2 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA03__QSPI_A_DATA3 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
};

int board_qspi_init(void)
{
	imx_iomux_v3_setup_multiple_pads(qspi_pads, ARRAY_SIZE(qspi_pads));

	set_clk_qspi();

	return 0;
}
#endif

int regulator_enable_init(void)
{
	imx_iomux_v3_setup_multiple_pads(regulator_enable_pads, ARRAY_SIZE(regulator_enable_pads));

	gpio_request(BOOST_GPIO, "boost_reg_en");

	gpio_direction_output(BOOST_GPIO, 1);

	return 0;
}

static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MQ_PAD_UART2_RXD__UART2_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MQ_PAD_UART2_TXD__UART2_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

int dram_init(void)
{
	unsigned long long sdram_size;
	if(C12_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C12_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C12_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ;
	/* rom_pointer[1] contains the size of TEE occupies */
	if (rom_pointer[1])
		gd->ram_size = sdram_size - rom_pointer[1];
	else
		gd->ram_size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
        if(C12_IS_4GB)
                gd->ram_size += PHYS_SDRAM_2_SIZE;
#endif

	return 0;
}

int dram_init_banksize(void)
{
        unsigned long long sdram_size;
        if(C12_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C12_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C12_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ; /* Actually the Kernel supports MAX 3GB of RAM */

        gd->bd->bi_dram[0].start = PHYS_SDRAM;
        if (rom_pointer[1])
                gd->bd->bi_dram[0].size = sdram_size -rom_pointer[1];
        else
                gd->bd->bi_dram[0].size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
        if(C12_IS_4GB) {
                gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
                gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
        }
#endif

        return 0;
}


#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	c12_fdt_ram_setup(blob, bd);
	if(!C12_HAS_PCIE)
		c12_fdt_pcie_setup(blob, bd);

	return 0;
}
#endif

#ifdef CONFIG_FEC_MXC
#define FEC_RST_PAD IMX_GPIO_NR(4, 3)

static iomux_v3_cfg_t const fec1_rst_pads[] = {
	IMX8MQ_PAD_SAI1_RXD1__GPIO4_IO3 | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

static void setup_iomux_fec(void)
{
	imx_iomux_v3_setup_multiple_pads(fec1_rst_pads, ARRAY_SIZE(fec1_rst_pads));

	gpio_request(FEC_RST_PAD, "fec1_rst");
	gpio_direction_output(FEC_RST_PAD, 0);
	udelay(10);
	gpio_direction_output(FEC_RST_PAD, 1);
}

static int setup_fec(void)
{
	struct iomuxc_gpr_base_regs *gpr =
		(struct iomuxc_gpr_base_regs *)IOMUXC_GPR_BASE_ADDR;

	setup_iomux_fec();

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&gpr->gpr[1],
		IOMUXC_GPR_GPR1_GPR_ENET1_TX_CLK_SEL_MASK, 0);
	return set_clk_enet(ENET_125MHZ);
}


int board_phy_config(struct phy_device *phydev)
{

	if (phydev->drv->config)
		phydev->drv->config(phydev);
	return 0;
}
#endif

#ifdef CONFIG_USB_DWC3

#define USB_PHY_CTRL0			0xF0040
#define USB_PHY_CTRL0_REF_SSP_EN	BIT(2)

#define USB_PHY_CTRL1			0xF0044
#define USB_PHY_CTRL1_RESET		BIT(0)
#define USB_PHY_CTRL1_COMMONONN		BIT(1)
#define USB_PHY_CTRL1_ATERESET		BIT(3)
#define USB_PHY_CTRL1_VDATSRCENB0	BIT(19)
#define USB_PHY_CTRL1_VDATDETENB0	BIT(20)

#define USB_PHY_CTRL2			0xF0048
#define USB_PHY_CTRL2_TXENABLEN0	BIT(8)

static struct dwc3_device dwc3_device_data = {
#ifdef CONFIG_SPL_BUILD
	.maximum_speed = USB_SPEED_HIGH,
#else
	.maximum_speed = USB_SPEED_SUPER,
#endif
	.base = USB1_BASE_ADDR,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.power_down_scale = 2,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

static void dwc3_nxp_usb_phy_init(struct dwc3_device *dwc3)
{
	u32 RegData;

	RegData = readl(dwc3->base + USB_PHY_CTRL1);
	RegData &= ~(USB_PHY_CTRL1_VDATSRCENB0 | USB_PHY_CTRL1_VDATDETENB0 |
			USB_PHY_CTRL1_COMMONONN);
	RegData |= USB_PHY_CTRL1_RESET | USB_PHY_CTRL1_ATERESET;
	writel(RegData, dwc3->base + USB_PHY_CTRL1);

	RegData = readl(dwc3->base + USB_PHY_CTRL0);
	RegData |= USB_PHY_CTRL0_REF_SSP_EN;
	writel(RegData, dwc3->base + USB_PHY_CTRL0);

	RegData = readl(dwc3->base + USB_PHY_CTRL2);
	RegData |= USB_PHY_CTRL2_TXENABLEN0;
	writel(RegData, dwc3->base + USB_PHY_CTRL2);

	RegData = readl(dwc3->base + USB_PHY_CTRL1);
	RegData &= ~(USB_PHY_CTRL1_RESET | USB_PHY_CTRL1_ATERESET);
	writel(RegData, dwc3->base + USB_PHY_CTRL1);
}
#endif

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_IMX8M)
int board_usb_init(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, true);

	if (index == 0 && init == USB_INIT_DEVICE) {
		dwc3_nxp_usb_phy_init(&dwc3_device_data);
		return dwc3_uboot_init(&dwc3_device_data);
	}

	return 0;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	if (index == 0 && init == USB_INIT_DEVICE) {
		dwc3_uboot_exit(index);
	}

	imx8m_usb_power(index, false);

	return 0;
}
#endif

int board_init(void)
{

	 /* Import SPL Micro information from spl struct */
        struct mx8_spl_data *data = (struct mx8_spl_data *)
             ((CONFIG_SPL_TEXT_BASE - sizeof(struct mx8_spl_data)) & ~0xf);
        gd->fwrev = data->fw_revision;
#ifdef CONFIG_FSL_QSPI
	board_qspi_init();
#endif

#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_IMX8M)
	init_usb_clk();
#endif

	if(C12_FW_03)
                printf("Hw Rev: 0x%x\n",c12_get_hw_revision());
	
	return 0;
}

static void do_enable_hdmi(struct display_info_t const *dev)
{
        gpio_request(HDMI_GPIO, "hdmi_osc_en");
        gpio_direction_output(HDMI_GPIO, 1);
        return;
}

int board_late_init(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("board_name", "C12");
	env_set("board_rev", "iMX8MQ");
#endif

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	return 0;
}

#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_ANDROID_RECOVERY
int is_recovery_key_pressing(void)
{
	return 0; /*TODO*/
}
#endif /*CONFIG_ANDROID_RECOVERY*/
#endif /*CONFIG_FSL_FASTBOOT*/

#if defined(CONFIG_VIDEO_IMXDCSS)

struct display_info_t const displays[] = {{
	.bus	= 0, /* Unused */
	.addr	= 0, /* Unused */
	.pixfmt	= GDF_32BIT_X888RGB,
	.detect	= NULL,
	.enable	= do_enable_hdmi,
#ifndef CONFIG_VIDEO_IMXDCSS_1080P
	.mode	= {
		.name           = "HDMI", /* 720P60 */
		.refresh        = 60,
		.xres           = 1280,
		.yres           = 720,
		.pixclock       = 13468, /* 74250  kHz */
		.left_margin    = 110,
		.right_margin   = 220,
		.upper_margin   = 5,
		.lower_margin   = 20,
		.hsync_len      = 40,
		.vsync_len      = 5,
		.sync           = FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		.vmode          = FB_VMODE_NONINTERLACED
	}
#else
	.mode	= {
		.name           = "HDMI", /* 1080P60 */
		.refresh        = 60,
		.xres           = 1920,
		.yres           = 1080,
		.pixclock       = 6734, /* 148500 kHz */
		.left_margin    = 148,
		.right_margin   = 88,
		.upper_margin   = 36,
		.lower_margin   = 4,
		.hsync_len      = 44,
		.vsync_len      = 5,
		.sync           = FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		.vmode          = FB_VMODE_NONINTERLACED
	}
#endif
} };

size_t display_count = ARRAY_SIZE(displays);

#endif /* CONFIG_VIDEO_IMXDCSS */

size_t display_count;

int board_early_init_f(void)
{
        struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

        imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

        set_wdog_reset(wdog);

        imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

        regulator_enable_init();

	if(C12_HAS_HDMI)	
		display_count = ARRAY_SIZE(displays);
	else		
		display_count = 0;

        return 0;
}

phys_size_t get_effective_memsize(void)
{
	unsigned long long sdram_size;
        if(C12_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C12_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C12_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ;
        if (rom_pointer[1])
	        return (sdram_size - rom_pointer[1]);
        else
                return sdram_size;
}
