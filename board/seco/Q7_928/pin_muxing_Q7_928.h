/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _BOARD_MX6_SECO_928_H__
#define _BOARD_MX6_SECO_928_H__

#include "../common/muxing.h"

/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   UART                                   |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const uart_debug_pads[] = {
	IOMUX_PADS(PAD_EIM_D26__UART2_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D27__UART2_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL)),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                               BOOT VALIDATE		                      |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const gpio_boot_validate_pad[] = {
	IOMUX_PADS(PAD_SD4_DAT5__GPIO2_IO13 | MUX_PAD_CTRL(NO_PAD_CTRL)),
};



/*  __________________________________________________________________________
 * |                                                                          |
 * |                                  MMC                                     |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const usdhc3_pads[] = {
	IOMUX_PADS(PAD_SD3_CLK__SD3_CLK    | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_CMD__SD3_CMD    | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT0__SD3_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT1__SD3_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT2__SD3_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT3__SD3_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT4__SD3_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT5__SD3_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT6__SD3_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD3_DAT7__SD3_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                On Board uSD		                      |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const usdhc4_pads[] = {
	IOMUX_PADS(PAD_SD4_CLK__SD4_CLK     | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD4_CMD__SD4_CMD     | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD4_DAT0__SD4_DATA0  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD4_DAT1__SD4_DATA1  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD4_DAT2__SD4_DATA2  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD4_DAT3__SD4_DATA3  | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_NANDF_D6__GPIO2_IO06 | MUX_PAD_CTRL(NO_PAD_CTRL)),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                               EXTERNAL uSD                               |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const usdhc1_pads[] = {
	IOMUX_PADS(PAD_SD1_CLK__SD1_CLK      | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD1_CMD__SD1_CMD      | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD1_DAT0__SD1_DATA0   | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD1_DAT1__SD1_DATA1   | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD1_DAT2__SD1_DATA2   | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_SD1_DAT3__SD1_DATA3   | MUX_PAD_CTRL(USDHC_PAD_CTRL)),
	IOMUX_PADS(PAD_NANDF_CS2__GPIO6_IO15 | MUX_PAD_CTRL(NO_PAD_CTRL)),

};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   SPI                                    |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const ecspi1_pads[] = {

	IOMUX_PADS(PAD_EIM_D17__ECSPI1_MISO  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D18__ECSPI1_MOSI  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D16__ECSPI1_SCLK  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D19__GPIO3_IO19   | MUX_PAD_CTRL(NO_PAD_CTRL)),
};


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                    EIM                                   |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const eimnor_pads[] = {
	IOMUX_PADS(PAD_EIM_DA0__EIM_AD00	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA1__EIM_AD01	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA2__EIM_AD02	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA3__EIM_AD03	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA4__EIM_AD04	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA5__EIM_AD05	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA6__EIM_AD06	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA7__EIM_AD07	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA8__EIM_AD08	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA9__EIM_AD09	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA10__EIM_AD10	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA11__EIM_AD11	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA12__EIM_AD12	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA13__EIM_AD13	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA14__EIM_AD14	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA15__EIM_AD15	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),

	IOMUX_PADS(PAD_EIM_LBA__EIM_LBA_B   | MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_OE__EIM_OE_B		| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_RW__EIM_RW		| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_BCLK__EIM_BCLK   | MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_CS0__EIM_CS0_B	| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_CS1__EIM_CS1_B	| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_WAIT__EIM_WAIT_B | MUX_PAD_CTRL(NO_PAD_CTRL)),

	IOMUX_PADS(PAD_EIM_DA0__EIM_AD00	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA1__EIM_AD01	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA2__EIM_AD02	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA3__EIM_AD03	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA4__EIM_AD04	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA5__EIM_AD05	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA6__EIM_AD06	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA7__EIM_AD07	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA8__EIM_AD08	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA9__EIM_AD09	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA10__EIM_AD10	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA11__EIM_AD11	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA12__EIM_AD12	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA13__EIM_AD13	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA14__EIM_AD14	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_DA15__EIM_AD15	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL)),

	IOMUX_PADS(PAD_EIM_LBA__EIM_LBA_B   | MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_OE__EIM_OE_B		| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_RW__EIM_RW		| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_BCLK__EIM_BCLK   | MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_CS0__EIM_CS0_B	| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_CS1__EIM_CS1_B	| MUX_PAD_CTRL(NO_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_WAIT__EIM_WAIT_B | MUX_PAD_CTRL(NO_PAD_CTRL)),


};


#endif   /* _BOARD_MX6_SECO_928_H__ */
