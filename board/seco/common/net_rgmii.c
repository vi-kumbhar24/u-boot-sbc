/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/crm_regs.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/mach-imx/boot_mode.h>
#include <micrel.h>

#include "proto_seco.h"
#include "muxing.h"

/*  __________________________________________________________________________
 * |                                                                          |
 * |	                        Ethernet RGMII		                      |
 * |__________________________________________________________________________|
 */

void setup_ksz90xx_iomux_enet( struct ksz90xx_pins ksz90xx ) {
	udelay( 20 );
	gpio_request(ksz90xx.gpio_pwr, "ksz90xx.gpio_pwr");
	gpio_direction_output( ksz90xx.gpio_pwr, 1); /* Power on enet */

	gpio_request(ksz90xx.gpio_rst, "ksz90xx.gpio_rst");
	gpio_direction_output( ksz90xx.gpio_rst, 0 ); /* assert PHY rst */

	gpio_request(ksz90xx.gpio_clken, "ksz90xx.gpio_clken");
	gpio_direction_output( ksz90xx.gpio_clken, 1 );
	gpio_request(ksz90xx.gpio_rd0, "ksz90xx.gpio_rd0");
	gpio_direction_output( ksz90xx.gpio_rd0,   1 ); //RD0
	gpio_request(ksz90xx.gpio_rd1, "ksz90xx.gpio_rd1");
	gpio_direction_output( ksz90xx.gpio_rd1,   1 ); //RD1
	gpio_request(ksz90xx.gpio_rd2, "ksz90xx.gpio_rd2");
	gpio_direction_output( ksz90xx.gpio_rd2,   1 ); //RD2
	gpio_request(ksz90xx.gpio_rd3, "ksz90xx.gpio_rd3");
	gpio_direction_output( ksz90xx.gpio_rd3,   1 ); //RD3

	udelay( 1000 );

	gpio_set_value( ksz90xx.gpio_rst, 1 ); /* deassert PHY rst */

	/* Need delay 100ms to exit from reset. */
	udelay( 1000 * 100 );

	gpio_free( ksz90xx.gpio_clken );
	gpio_free( ksz90xx.gpio_rd0 );
	gpio_free( ksz90xx.gpio_rd1 );
	gpio_free( ksz90xx.gpio_rd2 );
	gpio_free( ksz90xx.gpio_rd3 );
	udelay( 20 );
}


int ksz90xx_rgmii_rework( struct phy_device *phydev ) {
	/* control data pad skew - devaddr = 0x02, register = 0x04 */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_CTRL_SIG_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	/* rx data pad skew - devaddr = 0x02, register = 0x05 */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_RX_DATA_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	/* tx data pad skew - devaddr = 0x02, register = 0x05 */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_TX_DATA_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	/* gtx and rx clock pad skew - devaddr = 0x02, register = 0x08 */

	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_CLOCK_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x03FF);
	return 0;
}
//
//
//
//int board_phy_config (struct phy_device *phydev) {
//	mx6_rgmii_rework(phydev);
//
//	if (phydev->drv->config)
//		phydev->drv->config(phydev);
//
//	return 0;
//}
//
//
//int board_eth_init (bd_t *bis) {
//	uint32_t base = IMX_FEC_BASE;
//	struct mii_dev *bus = NULL;
//	struct phy_device *phydev = NULL;
//	int ret;
//	
//	enable_ethernet_rail ();
//	setup_iomux_enet();
//
//	bus = fec_get_miibus(base, -1);
//	if (!bus)
//		return 0;
//	/* scan phy 4,5,6,7 */
//	phydev = phy_find_by_mask(bus, (0xf << 4), PHY_INTERFACE_MODE_RGMII);
//
//	if (!phydev) {
//		free(bus);
//		return 0;
//	}
//	printf("using phy at %d\n", phydev->addr);
//	ret  = fec_probe(bis, -1, base, bus, phydev);
//	if (ret) {
//		printf("FEC MXC: %s:failed\n", __func__);
//		free(phydev);
//		free(bus);
//	}
//	return 0;
//}
