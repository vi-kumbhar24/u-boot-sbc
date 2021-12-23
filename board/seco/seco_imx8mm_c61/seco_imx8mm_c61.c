/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
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
#include <asm/arch/imx8mm_pins.h>
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
#include <seco/seco_imx8mm_c61_setup.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MM_PAD_UART2_RXD_UART2_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MM_PAD_UART2_TXD_UART2_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MM_PAD_GPIO1_IO02_WDOG1_WDOG_B	| MUX_PAD_CTRL(WDOG_PAD_CTRL),
	IMX8MM_PAD_SAI5_RXD0_GPIO3_IO21		| MUX_PAD_CTRL(WDOG_PAD_CTRL),
	IMX8MM_PAD_SAI5_RXD1_GPIO3_IO22		| MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

static iomux_v3_cfg_t const pca6416_pads[] = {
        IMX8MM_PAD_SAI2_RXFS_GPIO4_IO21 | MUX_PAD_CTRL(PAD_CTL_PE | PAD_CTL_PUE),
	IMX8MM_PAD_SAI2_RXC_GPIO4_IO22 | MUX_PAD_CTRL(0x0),
};

static iomux_v3_cfg_t const usb_hub_pads[] = {
        IMX8MM_PAD_SAI1_RXD4_GPIO4_IO6 | MUX_PAD_CTRL(PAD_CTL_PE | PAD_CTL_PUE),
        IMX8MM_PAD_SAI1_RXD0_GPIO4_IO2 | MUX_PAD_CTRL(PAD_CTL_PE | PAD_CTL_PUE),
};

static iomux_v3_cfg_t const video_disable_pads[] = {
        IMX8MM_PAD_GPIO1_IO11_GPIO1_IO11 | MUX_PAD_CTRL(PAD_CTL_PE | PAD_CTL_PUE),
	IMX8MM_PAD_SAI1_RXD2_GPIO4_IO4 | MUX_PAD_CTRL(PAD_CTL_PE | PAD_CTL_PUE),
	IMX8MM_PAD_SAI1_RXD3_GPIO4_IO5 | MUX_PAD_CTRL(PAD_CTL_PE | PAD_CTL_PUE),
};

void pca6416_reset ( void ) {
	
	gpio_request(IMX_GPIO_NR(4, 21), "PCA6416_RST");	
	gpio_direction_output(IMX_GPIO_NR(4, 21), 1);

}

void usb_hub_init ( void ) {
	
	imx_iomux_v3_setup_multiple_pads(usb_hub_pads, ARRAY_SIZE(usb_hub_pads));

	gpio_request(IMX_GPIO_NR(4, 2), "USB_HUB_RST");	

	gpio_direction_output(IMX_GPIO_NR(4, 2), 0);

}

void video_bridge_disable ( void ) {

	struct gpio_desc desc_enet0, desc_enet1;
        int ret;

	imx_iomux_v3_setup_multiple_pads(video_disable_pads, ARRAY_SIZE(video_disable_pads));	

	gpio_request(IMX_GPIO_NR(1, 11), "PD-BRIDGE");
	gpio_request(IMX_GPIO_NR(4, 4), "BKL_ON");
	gpio_request(IMX_GPIO_NR(4, 5), "PANEL_ON");
	gpio_direction_output(IMX_GPIO_NR(1, 11), 0);
	gpio_direction_output(IMX_GPIO_NR(4, 4), 0);
	gpio_direction_output(IMX_GPIO_NR(4, 5), 0);


        ret = dm_gpio_lookup_name("gpio@20_5", &desc_enet0);
        if(ret)
                printf("Error requesting gpio@20_5");

	ret = dm_gpio_lookup_name("gpio@20_9", &desc_enet1);
        if(ret)
                printf("Error requesting gpio@20_9");

        ret = dm_gpio_request(&desc_enet0, "gpio20_bkl");
        if(ret)
                printf("Error configuring gpio@20_5");

        ret = dm_gpio_request(&desc_enet1, "gpio20_lcd");
        if(ret)
                printf("Error configuring gpio@20_9");
	

        dm_gpio_set_dir_flags(&desc_enet0, GPIOD_IS_OUT);
        dm_gpio_set_value(&desc_enet0, 0);

	dm_gpio_set_dir_flags(&desc_enet1, GPIOD_IS_OUT);
        dm_gpio_set_value(&desc_enet1, 0);
}

#ifdef CONFIG_FSL_FSPI

int board_qspi_init(void)
{

	set_clk_qspi();

	return 0;
}
#endif

#ifdef CONFIG_NAND_MXS
#ifdef CONFIG_SPL_BUILD
#define NAND_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_HYS)
#define NAND_PAD_READY0_CTRL (PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_PUE)
static iomux_v3_cfg_t const gpmi_pads[] = {
	IMX8MM_PAD_NAND_ALE_RAWNAND_ALE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_CE0_B_RAWNAND_CE0_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_CE1_B_RAWNAND_CE1_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_CLE_RAWNAND_CLE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA00_RAWNAND_DATA00 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA01_RAWNAND_DATA01 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA02_RAWNAND_DATA02 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA03_RAWNAND_DATA03 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA04_RAWNAND_DATA04 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA05_RAWNAND_DATA05	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA06_RAWNAND_DATA06	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_DATA07_RAWNAND_DATA07	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_RE_B_RAWNAND_RE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_READY_B_RAWNAND_READY_B | MUX_PAD_CTRL(NAND_PAD_READY0_CTRL),
	IMX8MM_PAD_NAND_WE_B_RAWNAND_WE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MM_PAD_NAND_WP_B_RAWNAND_WP_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
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

	imx_iomux_v3_setup_multiple_pads(pca6416_pads, ARRAY_SIZE(pca6416_pads));
	
	pca6416_reset();	

	usb_hub_init();

	return 0;
}

int dram_init(void)
{
        unsigned long long sdram_size;
        if(C61_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C61_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C61_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ;
        /* rom_pointer[1] contains the size of TEE occupies */
        if (rom_pointer[1])
                gd->ram_size = sdram_size - rom_pointer[1];
        else
                gd->ram_size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
	if(C61_IS_4GB)
		gd->ram_size += PHYS_SDRAM_2_SIZE;
#endif

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	return 0;
}
#endif

int dram_init_banksize(void)
{

	unsigned long long sdram_size;
        if(C61_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C61_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C61_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ; /* Actually the Kernel supports MAX 3GB of RAM */

        gd->bd->bi_dram[0].start = PHYS_SDRAM;
        if (rom_pointer[1])
                gd->bd->bi_dram[0].size = sdram_size -rom_pointer[1];
        else
                gd->bd->bi_dram[0].size = sdram_size;

#if CONFIG_NR_DRAM_BANKS > 1
	if(C61_IS_4GB) {
		gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
		gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
	}
#endif

	return 0;
}


#ifdef CONFIG_FEC_MXC

static void setup_iomux_fec(void)
{

	struct gpio_desc desc_enet0;
        int ret;

	ret = dm_gpio_lookup_name("gpio@20_12", &desc_enet0);
	if(ret)	
		printf("Error requesting gpio@20_12");

	ret = dm_gpio_request(&desc_enet0, "gpio20_ethrst");
	if(ret)
                printf("Error configuring gpio@20_12");

	dm_gpio_set_dir_flags(&desc_enet0, GPIOD_IS_OUT);	
	dm_gpio_set_value(&desc_enet0, 0);
	udelay(500);
	dm_gpio_set_value(&desc_enet0, 1);
}

static int setup_fec(void)
{
	struct iomuxc_gpr_base_regs *const iomuxc_gpr_regs
		= (struct iomuxc_gpr_base_regs *) IOMUXC_GPR_BASE_ADDR;

	setup_iomux_fec();

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&iomuxc_gpr_regs->gpr[1],
			IOMUXC_GPR_GPR1_GPR_ENET1_TX_CLK_SEL_SHIFT, 0);
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
        if(C61_IS_1GB)
                sdram_size = PHYS_DRAM_IS_1GB ;
        if(C61_IS_2GB)
                sdram_size = PHYS_DRAM_IS_2GB ;
        if(C61_IS_4GB)
                sdram_size = PHYS_DRAM_IS_3GB ;
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
