// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2017-2019 NXP
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
#include <spl.h>
#include <power/pmic.h>
#include <power/pfuze100_pmic.h>
#include "../common/pfuze.h"
#include <usb.h>
#include <dwc3-uboot.h>
#include <seco_imx8_env.h>

DECLARE_GLOBAL_DATA_PTR;

#define NAND_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_HYS)
#define NAND_PAD_READY0_CTRL (PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_PUE)

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)

#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE)

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE)

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MQ_PAD_GPIO1_IO02__WDOG1_WDOG_B | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};
static iomux_v3_cfg_t const misc_pads[] = {
	IMX8MQ_PAD_NAND_CLE__GPIO3_IO5 | MUX_PAD_CTRL(PULLUP_PAD_CTRL), /* RTC_INT_CPU  */
};

static iomux_v3_cfg_t const video_disable_pads[] = {
	IMX8MQ_PAD_SAI5_RXFS__GPIO3_IO19 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),
};

void video_bridge_disable ( void ) {

        imx_iomux_v3_setup_multiple_pads(video_disable_pads, ARRAY_SIZE(video_disable_pads));

        gpio_request(IMX_GPIO_NR(3, 19), "PD-BRIDGE");
        gpio_direction_output(IMX_GPIO_NR(3, 19), 0);
}




#ifdef CONFIG_USB_DWC3

static iomux_v3_cfg_t const usb_c25_pads[] = {

	IMX8MQ_PAD_SAI2_RXC__GPIO4_IO22 | MUX_PAD_CTRL(NO_PAD_CTRL),
	IMX8MQ_PAD_SAI1_TXD7__GPIO4_IO19 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

#endif

#ifdef CONFIG_FSL_QSPI
int board_qspi_init(void)
{
	set_clk_qspi();

	return 0;
}
#endif

#ifdef CONFIG_NAND_MXS
#ifdef CONFIG_SPL_BUILD
static iomux_v3_cfg_t const gpmi_pads[] = {
	IMX8MQ_PAD_NAND_ALE__RAWNAND_ALE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_CE0_B__RAWNAND_CE0_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_CLE__RAWNAND_CLE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA00__RAWNAND_DATA00 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA01__RAWNAND_DATA01 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA02__RAWNAND_DATA02 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA03__RAWNAND_DATA03 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA04__RAWNAND_DATA04 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA05__RAWNAND_DATA05	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA06__RAWNAND_DATA06	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_DATA07__RAWNAND_DATA07	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_RE_B__RAWNAND_RE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_READY_B__RAWNAND_READY_B | MUX_PAD_CTRL(NAND_PAD_READY0_CTRL),
	IMX8MQ_PAD_NAND_WE_B__RAWNAND_WE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MQ_PAD_NAND_WP_B__RAWNAND_WP_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
};
#endif

static void setup_gpmi_nand(void)
{
#ifdef CONFIG_SPL_BUILD
	imx_iomux_v3_setup_multiple_pads(gpmi_pads, ARRAY_SIZE(gpmi_pads));
#endif

	init_nand_clk();
}
#endif

static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MQ_PAD_UART1_RXD__UART1_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MQ_PAD_UART1_TXD__UART1_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

	set_wdog_reset(wdog);

	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

#ifdef CONFIG_NAND_MXS
	setup_gpmi_nand(); /* SPL will call the board_early_init_f */
#endif
	/* Misc Pads Init */
	imx_iomux_v3_setup_multiple_pads(misc_pads, ARRAY_SIZE(misc_pads));

	return 0;
}
#ifdef CONFIG_USB_DWC3

void usb_seco_c25_init(void) {

        imx_iomux_v3_setup_multiple_pads(usb_c25_pads, ARRAY_SIZE(usb_c25_pads));


        gpio_request(IMX_GPIO_NR(4,22),"usb-hub-reset-c25");
	gpio_request(IMX_GPIO_NR(4,19),"sd2_p_en-c25");


        /* USB hub reset*/
        gpio_direction_output(IMX_GPIO_NR(4,22),1);
	gpio_direction_output(IMX_GPIO_NR(4,19),0);

}

#endif

int dram_init(void)
{

	unsigned long long sdram_size;
        if(C25_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C25_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C25_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ;
        /* rom_pointer[1] contains the size of TEE occupies */
        if (rom_pointer[1])
                gd->ram_size = sdram_size - rom_pointer[1];
        else
                gd->ram_size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
	if(C25_IS_4GB)
		gd->ram_size += PHYS_SDRAM_2_SIZE;
#endif

	return 0;
}

int dram_init_banksize(void)
{
	unsigned long long sdram_size;
        if(C25_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C25_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C25_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ; /* Actually the Kernel supports MAX 3GB of RAM */
	if(C25_IS_8GB)
                sdram_size = PHYS_DRAM_IS_3GB ; /* Actually the Kernel supports MAX 3GB of RAM */

	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	if (rom_pointer[1])
		gd->bd->bi_dram[0].size = sdram_size -rom_pointer[1];
	else
		gd->bd->bi_dram[0].size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
	if(C25_IS_4GB) {
		gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
		gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
	}
#endif

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
        c25_fdt_ram_setup(blob, bd);
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
#endif

static int setup_fec(void)
{
	setup_iomux_fec();

	return set_clk_enet(ENET_125MHZ);

}


int board_phy_config(struct phy_device *phydev)
{

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}


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

	printf("BD:    0x%x\n", c25_get_board_configuration());

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_IMX8M)
	init_usb_clk();
	usb_seco_c25_init();
#endif

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_IMX8M)
	init_usb_clk();
#endif

	video_bridge_disable();

	return 0;
}

int board_late_init(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

	env_set("board_name", "SECO C25");
	env_set("board_rev", "iMX8MQ");
#endif

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	return 0;
}

phys_size_t get_effective_memsize(void)
{
        unsigned long long sdram_size;
        if(C25_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C25_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C25_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ;
        if (rom_pointer[1])
                return (sdram_size - rom_pointer[1]);
        else
                return sdram_size;

}
