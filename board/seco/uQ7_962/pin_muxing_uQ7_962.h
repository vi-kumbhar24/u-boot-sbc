/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _BOARD_MX6_SECO_962_H__
#define _BOARD_MX6_SECO_962_H__

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
 * |		                  BOOT VALIDATE		                      |
 * |__________________________________________________________________________|
 */
static iomux_v3_cfg_t const gpio_boot_validate_pad[] = {
	IOMUX_PADS(PAD_NANDF_D4__GPIO2_IO04 | MUX_PAD_CTRL(NO_PAD_CTRL)),
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
 * |                               EXTERNAL uSD                               |
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
 * |                                   SPI                                    |
 * |__________________________________________________________________________|
 */
iomux_v3_cfg_t const ecspi1_pads[] = {

	IOMUX_PADS(PAD_EIM_D17__ECSPI1_MISO  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D18__ECSPI1_MOSI  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D16__ECSPI1_SCLK  | MUX_PAD_CTRL(SPI_PAD_CTRL)),
	IOMUX_PADS(PAD_EIM_D19__GPIO3_IO19   | MUX_PAD_CTRL(NO_PAD_CTRL)),
};


#endif   /* _BOARD_MX6_SECO_962_H__ */
