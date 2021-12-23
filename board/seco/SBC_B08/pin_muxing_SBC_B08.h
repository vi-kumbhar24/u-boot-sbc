/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _BOARD_MX6_SECO_SBC_B08_H__
#define _BOARD_MX6_SECO_SBC_B08_H__

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

static iomux_v3_cfg_t const uart2_pads[] = {
	MX6_PAD_GPIO1_IO06__UART2_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_GPIO1_IO07__UART2_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const uart5_pads[] = {
	MX6_PAD_KEY_COL3__UART5_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_KEY_ROW3__UART5_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
};


/*   __________________________________________________________________________
 * |                                                                          |
 * |	                            BOARD CODE	                              |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const gpio_board_code_pad[] = {
    MX6_PAD_ENET2_COL__GPIO2_IO_6 | MUX_PAD_CTRL(NO_PAD_CTRL),  // BOARD_REV_D0
    MX6_PAD_ENET2_CRS__GPIO2_IO_7 | MUX_PAD_CTRL(NO_PAD_CTRL),  // BOARD_REV_D1
   // MX6_PAD_KEY_ROW3__GPIO2_IO_18 | BOARD_REV_PAD_CFG,  // BOARD_REV_D2
};

static iomux_v3_cfg_t const board_recognition_pads[] = {
    MX6_PAD_RGMII1_RD3__GPIO5_IO_3 | MUX_PAD_CTRL(NO_PAD_CTRL),  // ENET1_COL conn to ENET2_RST
//    MX6_PAD_RGMII1_RD3__GPIO5_IO_3 | DIO_PAD_CFG, // GPIO TO DETECT RAM SIZE STARTING FROM REV C (included)
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
 * |                                   QSPI                                   |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const quadspi_pads[] = {
	MX6_PAD_NAND_WP_B__QSPI2_A_DATA_0	    | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_READY_B__QSPI2_A_DATA_1	| MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_CE0_B__QSPI2_A_DATA_2	    | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_CE1_B__QSPI2_A_DATA_3	    | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_ALE__QSPI2_A_SS0_B		    | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_CLE__QSPI2_A_SCLK		    | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
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







// /*  __________________________________________________________________________
//  * |                                                                          |
//  * |                                WI-FI	                                 |
//  * |__________________________________________________________________________|
//  */

static iomux_v3_cfg_t const wl1831_control_pads[] = {
	/* WL1831 - WiFi enable */
	MX6_PAD_KEY_COL2__GPIO2_IO_12 | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* WL1831 - Bt enable */
	MX6_PAD_KEY_ROW2__GPIO2_IO_17 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

// /*  __________________________________________________________________________
//  * |                                                                          |
//  * |                                GPIO		                                 |
//  * |__________________________________________________________________________|
//  */

static iomux_v3_cfg_t const gpio_pads[] = {

    MX6_PAD_NAND_DATA02__GPIO4_IO_6 | MUX_PAD_CTRL(NO_PAD_CTRL), // LED
    MX6_PAD_SD1_CLK__GPIO6_IO_0   | MUX_PAD_CTRL(NO_PAD_CTRL),   // LED

    MX6_PAD_KEY_COL3__GPIO2_IO_13 | MUX_PAD_CTRL(NO_PAD_CTRL),     // Gyro interrupt
    MX6_PAD_NAND_WE_B__GPIO4_IO_14 | MUX_PAD_CTRL(NO_PAD_CTRL),    // Mag interrupt
    MX6_PAD_QSPI1B_DATA2__GPIO4_IO_26 | MUX_PAD_CTRL(NO_PAD_CTRL), // Accel interrupt

    MX6_PAD_SD1_DATA2__GPIO6_IO_4 | MUX_PAD_CTRL(NO_PAD_CTRL),  // Touch interrupt
    MX6_PAD_SD1_DATA3__GPIO6_IO_5 | MUX_PAD_CTRL(NO_PAD_CTRL),  // Touch reset

    MX6_PAD_NAND_READY_B__GPIO4_IO_13 | MUX_PAD_CTRL(NO_PAD_CTRL),  // recognition GPIOs - Connected to R184
    MX6_PAD_NAND_ALE__GPIO4_IO_0 | MUX_PAD_CTRL(NO_PAD_CTRL),       // recognition GPIOs - Connected to R185

    // Multiplexer pins for GPIO/ADC (J5)
    MX6_PAD_RGMII2_TXC__GPIO5_IO_23 | MUX_PAD_CTRL(NO_PAD_CTRL),    // MUX_A
    MX6_PAD_RGMII2_RX_CTL__GPIO5_IO_16 | MUX_PAD_CTRL(NO_PAD_CTRL), // MUX_B
    MX6_PAD_QSPI1A_SCLK__GPIO4_IO_21 | MUX_PAD_CTRL(NO_PAD_CTRL),   // MUX_C
    MX6_PAD_QSPI1A_SS0_B__GPIO4_IO_22 | MUX_PAD_CTRL(NO_PAD_CTRL),  // MUX_D
    MX6_PAD_QSPI1A_DATA2__GPIO4_IO_18 | MUX_PAD_CTRL(NO_PAD_CTRL),  // MUX_E
    MX6_PAD_QSPI1A_DATA3__GPIO4_IO_19 | MUX_PAD_CTRL(NO_PAD_CTRL),  // MUX_F
};

/*  __________________________________________________________________________
 * |                                                                          |
 * |                                WATCHDOG APX                              |
 * |__________________________________________________________________________|
 */

static iomux_v3_cfg_t const wdog_b_pad = {
	MX6_PAD_GPIO1_IO13__GPIO1_IO_13 | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

// // /*  __________________________________________________________________________
// //  * |                                                                          |
// //  * |                                PERI_3V3                              |
// //  * |__________________________________________________________________________|
// //  */

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



// // /*  __________________________________________________________________________
// //  * |                                                                          |
// //  * |                                GPIO		                                 |
// //  * |__________________________________________________________________________|
// //  */

// static iomux_v3_cfg_t const gpio_pads[] = {

//     MX6_PAD_NAND_DATA02__GPIO4_IO_6 | MUX_PAD_CTRL(NO_PAD_CTRL), // LED
//     MX6_PAD_SD1_CLK__GPIO6_IO_0   | MUX_PAD_CTRL(NO_PAD_CTRL),   // LED

//     MX6_PAD_KEY_COL3__GPIO2_IO_13 | MUX_PAD_CTRL(NO_PAD_CTRL),     // Gyro interrupt
//     MX6_PAD_NAND_WE_B__GPIO4_IO_14 | MUX_PAD_CTRL(NO_PAD_CTRL),    // Mag interrupt
//     MX6_PAD_QSPI1B_DATA2__GPIO4_IO_26 | MUX_PAD_CTRL(NO_PAD_CTRL), // Accel interrupt

//     MX6_PAD_SD1_DATA2__GPIO6_IO_4 | MUX_PAD_CTRL(NO_PAD_CTRL),  // Touch interrupt
//     MX6_PAD_SD1_DATA3__GPIO6_IO_5 | MUX_PAD_CTRL(NO_PAD_CTRL),  // Touch reset

//     MX6_PAD_NAND_READY_B__GPIO4_IO_13 | MUX_PAD_CTRL(NO_PAD_CTRL),  // recognition GPIOs - Connected to R184
//     MX6_PAD_NAND_ALE__GPIO4_IO_0 | MUX_PAD_CTRL(NO_PAD_CTRL),       // recognition GPIOs - Connected to R185

//     // Multiplexer pins for GPIO/ADC (J5)
//     MX6_PAD_RGMII2_TXC__GPIO5_IO_23 | MUX_PAD_CTRL(NO_PAD_CTRL),    // MUX_A
//     MX6_PAD_RGMII2_RX_CTL__GPIO5_IO_16 | MUX_PAD_CTRL(NO_PAD_CTRL), // MUX_B
//     MX6_PAD_QSPI1A_SCLK__GPIO4_IO_21 | MUX_PAD_CTRL(NO_PAD_CTRL),   // MUX_C
//     MX6_PAD_QSPI1A_SS0_B__GPIO4_IO_22 | MUX_PAD_CTRL(NO_PAD_CTRL),  // MUX_D
//     MX6_PAD_QSPI1A_DATA2__GPIO4_IO_18 | MUX_PAD_CTRL(NO_PAD_CTRL),  // MUX_E
//     MX6_PAD_QSPI1A_DATA3__GPIO4_IO_19 | MUX_PAD_CTRL(NO_PAD_CTRL),  // MUX_F
// };

// iomux_v3_cfg_t const wdt_trigger_pads[] = {
// 	IOMUX_PADS(PAD_EIM_D25__GPIO3_IO25    | MUX_PAD_CTRL(USDHC_PAD_CTRL)),     // WDT_TRIG
// 	IOMUX_PADS(PAD_KEY_ROW2__GPIO4_IO11    | MUX_PAD_CTRL(USDHC_PAD_CTRL)),    // WDT_EN
// };

// iomux_v3_cfg_t const enet_pads1[] = {
// 	IOMUX_PADS(PAD_ENET_MDIO__ENET_MDIO         | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_ENET_MDC__ENET_MDC           | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_TXC__RGMII_TXC	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_TD0__RGMII_TD0	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_TD1__RGMII_TD1	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_TD2__RGMII_TD2	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_TD3__RGMII_TD3	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_TX_CTL__RGMII_TX_CTL   | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_ENET_REF_CLK__ENET_TX_CLK    | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_RXC__RGMII_RXC	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	/* RGMII reset */
// 	IOMUX_PADS(PAD_EIM_D23__GPIO3_IO23          | MUX_PAD_CTRL(NO_PAD_CTRL | PAD_CTL_PUS_47K_UP)),
// 	/* alimentazione ethernet*/
// 	//IOMUX_PADS(PAD_EIM_EB3__GPIO2_IO31		| MUX_PAD_CTRL(NO_PAD_CTRL),
// 	/* pin 32 - 1 - (MODE0) all */
// 	IOMUX_PADS(PAD_RGMII_RD0__GPIO6_IO25        | MUX_PAD_CTRL(NO_PAD_CTRL | PAD_CTL_PUS_47K_UP)),
// 	/* pin 31 - 1 - (MODE1) all */
// 	IOMUX_PADS(PAD_RGMII_RD1__GPIO6_IO27        | MUX_PAD_CTRL(NO_PAD_CTRL | PAD_CTL_PUS_47K_UP)),
// 	/* pin 28 - 1 - (MODE2) all */
// 	IOMUX_PADS(PAD_RGMII_RD2__GPIO6_IO28        | MUX_PAD_CTRL(NO_PAD_CTRL | PAD_CTL_PUS_47K_UP)),
// 	/* pin 27 - 1 - (MODE3) all */
// 	IOMUX_PADS(PAD_RGMII_RD3__GPIO6_IO29        | MUX_PAD_CTRL(NO_PAD_CTRL | PAD_CTL_PUS_47K_UP)),
// 	/* pin 3 - 1 - (CLK125_EN) 125Mhz clockout enabled */
// 	IOMUX_PADS(PAD_RGMII_RX_CTL__GPIO6_IO24     | MUX_PAD_CTRL(NO_PAD_CTRL | PAD_CTL_PUS_47K_UP)),
// };

// iomux_v3_cfg_t const enet_pads2[] = {
// 	IOMUX_PADS(PAD_RGMII_RXC__RGMII_RXC         | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_RD0__RGMII_RD0	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_RD1__RGMII_RD1	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_RD2__RGMII_RD2	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_RD3__RGMII_RD3	        | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// 	IOMUX_PADS(PAD_RGMII_RX_CTL__RGMII_RX_CTL   | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// };



// iomux_v3_cfg_t const eth_pads[] = {
// 	IOMUX_PADS(PAD_KEY_COL2__GPIO4_IO10 | MUX_PAD_CTRL(ENET_PAD_CTRL)),
// };


// /*  __________________________________________________________________________
//  * |                                                                          |
//  * |                                  MMC                                     |
//  * |__________________________________________________________________________|
//  */
// iomux_v3_cfg_t const usdhc3_pads[] = {
// 	IOMUX_PADS(PAD_SD4_CLK__SD4_CLK    | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_CMD__SD4_CMD    | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT0__SD4_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT1__SD4_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT2__SD4_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT3__SD4_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT4__SD4_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT5__SD4_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT6__SD4_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD4_DAT7__SD4_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// };


// /*  __________________________________________________________________________
//  * |                                                                          |
//  * |                               EXTERNAL uSD                               |
//  * |__________________________________________________________________________|
//  */
// iomux_v3_cfg_t const usdhc4_pads[] = {
// 	IOMUX_PADS(PAD_SD3_CLK__SD3_CLK     | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD3_CMD__SD3_CMD     | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD3_DAT0__SD3_DATA0  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD3_DAT1__SD3_DATA1  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD3_DAT2__SD3_DATA2  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD3_DAT3__SD3_DATA3  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
// 	IOMUX_PADS(PAD_SD3_DAT5__GPIO7_IO00 | MUX_PAD_CTRL(NO_PAD_CTRL)),
// 	IOMUX_PADS(PAD_NANDF_D5__GPIO2_IO05 | MUX_PAD_CTRL(NO_PAD_CTRL)),
// };


// /*  __________________________________________________________________________
//  * |                                                                          |
//  * |                                   SPI                                    |
//  * |__________________________________________________________________________|
//  */
// iomux_v3_cfg_t const ecspi1_pads[] = {
// 	IOMUX_PADS(PAD_EIM_D17__ECSPI1_MISO  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
// 	IOMUX_PADS(PAD_EIM_D18__ECSPI1_MOSI  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
// 	IOMUX_PADS(PAD_EIM_D16__ECSPI1_SCLK  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
// 	IOMUX_PADS(PAD_EIM_EB2__GPIO2_IO30   | MUX_PAD_CTRL(NO_PAD_CTRL)),

// };

// iomux_v3_cfg_t const ecspi2_pads[] = {
// 	IOMUX_PADS(PAD_DISP0_DAT0__ECSPI3_SCLK  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
// 	IOMUX_PADS(PAD_DISP0_DAT1__ECSPI3_MOSI  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
// 	IOMUX_PADS(PAD_DISP0_DAT2__ECSPI3_MISO  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
// 	IOMUX_PADS(PAD_DISP0_DAT3__GPIO4_IO24  | MUX_PAD_CTRL(NO_PAD_CTRL)),

// };


// /*  __________________________________________________________________________
//  * |                                                                          |
//  * |                                    USB                                   |
//  * |__________________________________________________________________________|
//  */
// iomux_v3_cfg_t const usb_otg_pads[] = {
// 	IOMUX_PADS(PAD_EIM_D22__USB_OTG_PWR      | MUX_PAD_CTRL(NO_PAD_CTRL)),
// 	IOMUX_PADS(PAD_GPIO_1__USB_OTG_ID | MUX_PAD_CTRL(NO_PAD_CTRL)),
// };


// iomux_v3_cfg_t const usb_hc1_pads[] = {
// 	IOMUX_PADS(PAD_GPIO_17__GPIO7_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL)),
// 	IOMUX_PADS(PAD_NANDF_CS2__CCM_CLKO2 | MUX_PAD_CTRL(NO_PAD_CTRL)),
// };



#endif   /* __BOARD_MX6_SECO_SBC_B08_H__ */
