/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __BOARD_MX6_SECO_SBC_C23_H__
#define __BOARD_MX6_SECO_SBC_C23_H__

#include "../common/muxing.h"


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   UART                                   |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const uart1_pads[] = {
	MX6_PAD_GPIO1_IO04__UART1_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_GPIO1_IO05__UART1_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

/*   __________________________________________________________________________
 * |                                                                          |
 * |	                            BOARD CODE	                              |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const board_revision_pads[] = {
    MX6_PAD_LCD1_HSYNC__GPIO3_IO_26 | BOARD_REV_PAD_CFG,  // BOARD_REV_BIT0
    MX6_PAD_LCD1_ENABLE__GPIO3_IO_25 | BOARD_REV_PAD_CFG, // BOARD_REV_BIT1
    MX6_PAD_LCD1_RESET__GPIO3_IO_27 | BOARD_REV_PAD_CFG,  // BOARD_REV_BIT2
};

iomux_v3_cfg_t const board_recognition_pads[] = {
    MX6_PAD_ENET1_COL__GPIO2_IO_0 | DIO_PAD_CFG,  // ENET1_COL conn to ENET2_RST
    MX6_PAD_RGMII1_RD3__GPIO5_IO_3 | DIO_PAD_CFG, // GPIO TO DETECT RAM SIZE STARTING FROM REV C (included)
};

iomux_v3_cfg_t const board_ram_recognition_pads[] = {
    MX6_PAD_LCD1_VSYNC__GPIO3_IO_28 | DIO_PAD_CFG,
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                 ETHERNET                                 |
 * |__________________________________________________________________________|
 */

static iomux_v3_cfg_t const phy_control_pads[] = {
	/* 25MHz Ethernet PHY Clock */
	MX6_PAD_ENET2_RX_CLK__ENET2_REF_CLK_25M | MUX_PAD_CTRL(ENET_CLK_PAD_CTRL),
	MX6_PAD_ENET1_MDC__ENET1_MDC 			| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET1_MDIO__ENET1_MDIO 			| MUX_PAD_CTRL(ENET_PAD_CTRL),
};


static iomux_v3_cfg_t const enet_pads1[] = {
	MX6_PAD_ENET1_TX_CLK__ENET1_REF_CLK1    | MUX_PAD_CTRL(ENET_PAD_CTRL),

	MX6_PAD_RGMII1_TX_CTL__ENET1_TX_EN  	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TD0__ENET1_TX_DATA_0 	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TD1__ENET1_TX_DATA_1 	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_RXC__ENET1_RX_ER     	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_RX_CTL__ENET1_RX_EN 		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_RD0__ENET1_RX_DATA_0 	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_RD1__ENET1_RX_DATA_1 	| MUX_PAD_CTRL(ENET_PAD_CTRL),

	/*Reset */
	MX6_PAD_ENET1_CRS__GPIO2_IO_1 			| MUX_PAD_CTRL(ENET_PAD_CTRL),
	/* Interrupt */
	MX6_PAD_KEY_COL0__GPIO2_IO_10			| MUX_PAD_CTRL(ENET_PAD_CTRL),
};


static iomux_v3_cfg_t const enet_pads2[] = {
	MX6_PAD_ENET2_TX_CLK__ENET2_REF_CLK2 	| MUX_PAD_CTRL(ENET_PAD_CTRL),

	MX6_PAD_RGMII2_TX_CTL__ENET2_TX_EN  	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII2_TD0__ENET2_TX_DATA_0 	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII2_TD1__ENET2_TX_DATA_1 	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII2_RXC__ENET2_RX_ER     	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII2_RX_CTL__ENET2_RX_EN 		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII2_RD0__ENET2_RX_DATA_0 	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII2_RD1__ENET2_RX_DATA_1 	| MUX_PAD_CTRL(ENET_PAD_CTRL),

	/*Reset */
	MX6_PAD_ENET1_COL__GPIO2_IO_0 			| MUX_PAD_CTRL(ENET_PAD_CTRL),
	/* Interrupt */
	MX6_PAD_KEY_ROW0__GPIO2_IO_15 			| MUX_PAD_CTRL(ENET_PAD_CTRL),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                  eMMC                                    |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const usdhc4_pads[] = {
    /* Configured for eMMC */
	MX6_PAD_SD4_CLK__USDHC4_CLK 	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_CMD__USDHC4_CMD 	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA0__USDHC4_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA1__USDHC4_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA2__USDHC4_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA3__USDHC4_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA4__USDHC4_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA5__USDHC4_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA6__USDHC4_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA7__USDHC4_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};



/*  __________________________________________________________________________
 * |                                                                          |
 * |                               uSD on board                               |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const usdhc2_pads[] = {
	MX6_PAD_SD2_CLK__USDHC2_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_CMD__USDHC2_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DATA0__USDHC2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DATA1__USDHC2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DATA2__USDHC2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DATA3__USDHC2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	/* CD pin */    
	MX6_PAD_SD1_DATA0__GPIO6_IO_2 | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* Power */
	MX6_PAD_SD1_CMD__GPIO6_IO_1 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc2_pads_reset[] = {
	MX6_PAD_SD2_CLK__GPIO6_IO_6    | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD2_CMD__GPIO6_IO_7    | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD2_DATA0__GPIO6_IO_8  | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD2_DATA1__GPIO6_IO_9  | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD2_DATA2__GPIO6_IO_10 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD2_DATA3__GPIO6_IO_11 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD1_DATA0__GPIO6_IO_2  | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD1_CMD__GPIO6_IO_1    | MUX_PAD_CTRL(NO_PAD_CTRL),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                  WiFi                                    |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const usdhc3_pads[] = {
    /* Configured for WLAN */
	MX6_PAD_SD3_CLK__USDHC3_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__USDHC3_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DATA0__USDHC3_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DATA1__USDHC3_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DATA2__USDHC3_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DATA3__USDHC3_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                WI-FI	                              |
 * |__________________________________________________________________________|
 */

static iomux_v3_cfg_t const wl1831_control_pads[] = {
	/* WL1831 - WiFi enable */
	MX6_PAD_ENET2_COL__GPIO2_IO_6 | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* WL1831 - Bt enable */
	MX6_PAD_ENET2_CRS__GPIO2_IO_7 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

/*  __________________________________________________________________________
 * |                                                                          |
 * |                                GPIO		                      |
 * |__________________________________________________________________________|
 */

static iomux_v3_cfg_t const gpio_pads[] = {

    MX6_PAD_LCD1_DATA23__GPIO3_IO_24 | MUX_PAD_CTRL(NO_PAD_CTRL), // LED BLUE
    MX6_PAD_LCD1_DATA21__GPIO3_IO_22 | MUX_PAD_CTRL(NO_PAD_CTRL), // LED RED
    MX6_PAD_LCD1_DATA17__GPIO3_IO_18 | MUX_PAD_CTRL(NO_PAD_CTRL), // LED GREEN
    MX6_PAD_LCD1_DATA11__GPIO3_IO_12 | MUX_PAD_CTRL(NO_PAD_CTRL), // LED PWR GREEN
    MX6_PAD_LCD1_DATA16__GPIO3_IO_17 | MUX_PAD_CTRL(NO_PAD_CTRL), // HUB RESET
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                MODEM + PCIE                              |
 * |__________________________________________________________________________|
 */


static iomux_v3_cfg_t const pcie_keye_pads[] = {
    MX6_PAD_NAND_DATA02__GPIO4_IO_6 | MUX_PAD_CTRL(NO_PAD_CTRL), // PCIE_RST
    MX6_PAD_GPIO1_IO12__GPIO1_IO_12 | MUX_PAD_CTRL(NO_PAD_CTRL), // W_DISABLE_1
    MX6_PAD_GPIO1_IO11__GPIO1_IO_11 | MUX_PAD_CTRL(NO_PAD_CTRL), // W_DISABLE_2
};

static iomux_v3_cfg_t const pcie_keyb_pads[] = {
    MX6_PAD_RGMII2_TXC__GPIO5_IO_23 | MUX_PAD_CTRL(NO_PAD_CTRL), // M2_RST
    MX6_PAD_RGMII1_RD2__GPIO5_IO_2 | MUX_PAD_CTRL(NO_PAD_CTRL),  // W_DISABLE_1_1v8
    MX6_PAD_RGMII1_RD3__GPIO5_IO_3 | MUX_PAD_CTRL(NO_PAD_CTRL),  // W_DISABLE_2_1v8
};

static iomux_v3_cfg_t const modem_pads[] = {
    MX6_PAD_LCD1_DATA10__GPIO3_IO_11 | MUX_PAD_CTRL(NO_PAD_CTRL), // EN_RESET_N
    MX6_PAD_LCD1_DATA09__GPIO3_IO_10 | MUX_PAD_CTRL(NO_PAD_CTRL), // EN_EMERG
    MX6_PAD_LCD1_DATA14__GPIO3_IO_15 | MUX_PAD_CTRL(NO_PAD_CTRL), // EN_PWRKEY
    MX6_PAD_NAND_DATA07__GPIO4_IO_11 | MUX_PAD_CTRL(NO_PAD_CTRL), // EN_3v8
};





/*  __________________________________________________________________________
 * |                                                                          |
 * |                                WATCHDOG APX                              |
 * |__________________________________________________________________________|
 */

static iomux_v3_cfg_t const wdog_b_pad = {
	MX6_PAD_GPIO1_IO13__GPIO1_IO_13 | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

/*  __________________________________________________________________________
 * |                                                                          |
 * |                                PERI_3V3                                  |
 * |__________________________________________________________________________|
 */

static iomux_v3_cfg_t const peri_3v3_pads[] = {
	MX6_PAD_QSPI1A_DATA0__GPIO4_IO_16 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

/*  __________________________________________________________________________
 * |                                                                          |
 * |                                    OTG                                   |
 * |__________________________________________________________________________|
 */

#ifdef CONFIG_USB_EHCI_MX6
/* OGT1 */
static iomux_v3_cfg_t const usb_otg1_pads[] = {
	MX6_PAD_GPIO1_IO09__USB_OTG1_PWR | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_GPIO1_IO10__ANATOP_OTG1_ID | MUX_PAD_CTRL(OTG_ID_PAD_CTRL),
};

/* OTG2 */
static iomux_v3_cfg_t const usb_otg2_pads[] = {
	MX6_PAD_QSPI1A_SS0_B__USB_OTG2_PWR | MUX_PAD_CTRL(NO_PAD_CTRL),
};

/* USB HSIC */
static iomux_v3_cfg_t const usb_hsic_pads[] = {
	MX6_PAD_USB_H_STROBE__USB_H_STROBE | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_USB_H_DATA__USB_H_DATA | MUX_PAD_CTRL(NO_PAD_CTRL),

	/* HSIC_RST# */
	MX6_PAD_RGMII1_RD2__GPIO5_IO_2 | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* HSIC_INT# */
	MX6_PAD_KEY_COL1__GPIO2_IO_11 | MUX_PAD_CTRL(NO_PAD_CTRL),

	/* USB_OTG3 PWR */
	MX6_PAD_NAND_DATA04__GPIO4_IO_8 | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* USB_OTG4 PWR */
	MX6_PAD_NAND_DATA05__GPIO4_IO_9 | MUX_PAD_CTRL(NO_PAD_CTRL),
};
#endif

#endif   /* __BOARD_MX6_SECO_SBC_C23_H__ */
