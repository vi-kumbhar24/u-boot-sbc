// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018-2019 NXP
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
#include <asm/arch/imx8mn_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/clock.h>
#include <spl.h>
#include <asm/mach-imx/dma.h>
#include <power/pmic.h>
#include <power/bd71837.h>
#include "../common/tcpc.h"
#include <usb.h>
#include <sec_mipi_dsim.h>
#include <imx_mipi_dsi_bridge.h>
#include <mipi_dsi_panel.h>
#include <asm/mach-imx/video.h>
#ifdef CONFIG_APX_WATCHDOG
#include "../common/wdt.h"
#endif
#include <seco_imx8_env.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

static iomux_v3_cfg_t const uart_pads[] = {
        IMX8MN_PAD_UART2_RXD__UART2_DCE_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
        IMX8MN_PAD_UART2_TXD__UART2_DCE_TX | MUX_PAD_CTRL(UART_PAD_CTRL),

};

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MN_PAD_GPIO1_IO02__WDOG1_WDOG_B	| MUX_PAD_CTRL(WDOG_PAD_CTRL),
	IMX8MN_PAD_SAI5_RXD0__GPIO3_IO21		| MUX_PAD_CTRL(WDOG_PAD_CTRL),
	IMX8MN_PAD_SAI5_RXD1__GPIO3_IO22		| MUX_PAD_CTRL(WDOG_PAD_CTRL),
};


void video_bridge_disable ( void ) {

}

#ifdef CONFIG_FSL_FSPI
#define QSPI_PAD_CTRL	(PAD_CTL_DSE2 | PAD_CTL_HYS)
static iomux_v3_cfg_t const qspi_pads[] = {
	IMX8MN_PAD_NAND_ALE__QSPI_A_SCLK | MUX_PAD_CTRL(QSPI_PAD_CTRL | PAD_CTL_PE | PAD_CTL_PUE),
	IMX8MN_PAD_NAND_CE0_B__QSPI_A_SS0_B | MUX_PAD_CTRL(QSPI_PAD_CTRL),

	IMX8MN_PAD_NAND_DATA00__QSPI_A_DATA0 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA01__QSPI_A_DATA1 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA02__QSPI_A_DATA2 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA03__QSPI_A_DATA3 | MUX_PAD_CTRL(QSPI_PAD_CTRL),
};

int board_qspi_init(void)
{
	imx_iomux_v3_setup_multiple_pads(qspi_pads, ARRAY_SIZE(qspi_pads));

	set_clk_qspi();

	return 0;
}
#endif

#ifdef CONFIG_MXC_SPI
#define SPI_PAD_CTRL	(PAD_CTL_DSE2 | PAD_CTL_HYS)
static iomux_v3_cfg_t const ecspi1_pads[] = {
	IMX8MN_PAD_ECSPI1_SCLK__ECSPI1_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MN_PAD_ECSPI1_MOSI__ECSPI1_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MN_PAD_ECSPI1_MISO__ECSPI1_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MN_PAD_ECSPI1_SS0__GPIO5_IO9 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const ecspi2_pads[] = {
	IMX8MN_PAD_ECSPI2_SCLK__ECSPI2_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MN_PAD_ECSPI2_MOSI__ECSPI2_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MN_PAD_ECSPI2_MISO__ECSPI2_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	IMX8MN_PAD_ECSPI2_SS0__GPIO5_IO13 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_spi(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi1_pads, ARRAY_SIZE(ecspi1_pads));
	imx_iomux_v3_setup_multiple_pads(ecspi2_pads, ARRAY_SIZE(ecspi2_pads));
	gpio_request(IMX_GPIO_NR(5, 9), "ECSPI1 CS");
	gpio_request(IMX_GPIO_NR(5, 13), "ECSPI2 CS");
}

int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
	if (bus == 0)
		return IMX_GPIO_NR(5, 9);
	else
		return IMX_GPIO_NR(5, 13);
}
#endif

#ifdef CONFIG_NAND_MXS
#ifdef CONFIG_SPL_BUILD
#define NAND_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_HYS)
#define NAND_PAD_READY0_CTRL (PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_PUE)
static iomux_v3_cfg_t const gpmi_pads[] = {
	IMX8MN_PAD_NAND_ALE__RAWNAND_ALE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_CE0_B__RAWNAND_CE0_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_CE1_B__RAWNAND_CE1_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_CLE__RAWNAND_CLE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA00__RAWNAND_DATA00 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA01__RAWNAND_DATA01 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA02__RAWNAND_DATA02 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA03__RAWNAND_DATA03 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA04__RAWNAND_DATA04 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA05__RAWNAND_DATA05	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA06__RAWNAND_DATA06	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA07__RAWNAND_DATA07	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_RE_B__RAWNAND_RE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_READY_B__RAWNAND_READY_B | MUX_PAD_CTRL(NAND_PAD_READY0_CTRL),
	IMX8MN_PAD_NAND_WE_B__RAWNAND_WE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_WP_B__RAWNAND_WP_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
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

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

	set_wdog_reset(wdog);
	
	hw_watchdog_init();

	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

	init_uart_clk(1);

#ifdef CONFIG_NAND_MXS
	setup_gpmi_nand(); /* SPL will call the board_early_init_f */
#endif


	return 0;
}

int dram_init(void)
{
        unsigned long long sdram_size;
	sdram_size = PHYS_DRAM_IS_512MB; // default to 512MB
        if(C72_IS_512MB)
                sdram_size = PHYS_DRAM_IS_512MB ;
        if(C72_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C72_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        /* rom_pointer[1] contains the size of TEE occupies */
        if (rom_pointer[1])
                gd->ram_size = sdram_size - rom_pointer[1];
        else
                gd->ram_size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
	if(C72_IS_4GB)
		gd->ram_size += PHYS_SDRAM_2_SIZE;
#endif

	return 0;
}

int dram_init_banksize(void)
{

	unsigned long long sdram_size;
	sdram_size = PHYS_DRAM_IS_512MB; // default to 512MB
        if(C72_IS_512MB)
                sdram_size = PHYS_DRAM_IS_512MB ;
        if(C72_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C72_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;

        gd->bd->bi_dram[0].start = PHYS_SDRAM;
        if (rom_pointer[1])
                gd->bd->bi_dram[0].size = sdram_size -rom_pointer[1];
        else
                gd->bd->bi_dram[0].size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
	if(C72_IS_4GB) {
		gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
		gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
	}
#endif

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	return 0;
}
#endif

#ifdef CONFIG_FEC_MXC

static void setup_iomux_fec(void)
{

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


#ifdef CONFIG_USB_EHCI_HCD
int board_usb_init(int index, enum usb_init_type init)
{
        int ret = 0;
#ifdef CONFIG_USB_TCPC
        struct tcpc_port *port_ptr;
#endif

        debug("board_usb_init %d, type %d\n", index, init);

        imx8m_usb_power(index, true);

#ifdef CONFIG_USB_TCPC
        if (index == 0)
                port_ptr = &port1;
        else
                port_ptr = &port2;

        if (init == USB_INIT_HOST)
                tcpc_setup_dfp_mode(port_ptr);
        else
                tcpc_setup_ufp_mode(port_ptr);
#endif

        return ret;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
        int ret = 0;

        debug("board_usb_cleanup %d, type %d\n", index, init);

#ifdef CONFIG_USB_TCPC
        if (init == USB_INIT_HOST) {
                if (index == 0)
                        ret = tcpc_disable_src_vbus(&port1);
                else
                        ret = tcpc_disable_src_vbus(&port2);
        }
#endif

        imx8m_usb_power(index, false);
        return ret;
}

#ifdef CONFIG_USB_TCPC
int board_ehci_usb_phy_mode(struct udevice *dev)
{
        int ret = 0;
        enum typec_cc_polarity pol;
        enum typec_cc_state state;
        struct tcpc_port *port_ptr;

        if (dev->seq == 0)
                port_ptr = &port1;
        else
                port_ptr = &port2;

        tcpc_setup_ufp_mode(port_ptr);

        ret = tcpc_get_cc_status(port_ptr, &pol, &state);
        if (!ret) {
                if (state == TYPEC_STATE_SRC_RD_RA || state == TYPEC_STATE_SRC_RD)
                        return USB_INIT_HOST;
        }

        return USB_INIT_DEVICE;
}
#endif
#endif


int board_init(void)
{
#ifdef CONFIG_USB_TCPC
	setup_typec();
#endif

#ifdef CONFIG_MXC_SPI
	setup_spi();
#endif

#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif

#ifdef CONFIG_FSL_FSPI
	board_qspi_init();
#endif

	video_bridge_disable();

	return 0;
}

int board_late_init(void)
{
#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	return 0;
}

phys_size_t get_effective_memsize(void)
{
	unsigned long long sdram_size;
	sdram_size = PHYS_DRAM_IS_512MB; // default to 512MB
        if(C72_IS_512MB)
                sdram_size = PHYS_DRAM_IS_512MB ;
        if(C72_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C72_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if (rom_pointer[1])
                return (sdram_size - rom_pointer[1]);
        else
                return sdram_size;
}

#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_ANDROID_RECOVERY
int is_recovery_key_pressing(void)
{
	return 0; /*TODO*/
}
#endif /*CONFIG_ANDROID_RECOVERY*/
#endif /*CONFIG_FSL_FASTBOOT*/
