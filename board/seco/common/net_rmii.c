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

void setup_ksz80xx_iomux_enet ( struct ksz80xx_pins ksx80xx ) {
	udelay( 20 );

	gpio_direction_output( ksx80xx.gpio_rst, 0 ); /* assert PHY rst */
	udelay( 10000 );
	gpio_set_value( ksx80xx.gpio_rst, 1 ); /* deassert PHY rst */

	/* Need delay 100ms to exit from reset. */
	udelay( 1000 * 100 );
}


int ksz80xx_rmii_rework (struct phy_device *phydev) {
	phy_write (phydev, MDIO_DEVAD_NONE, MII_BMCR, 0x2100);
	phydev->autoneg = 0;
	return 0;
}




//int board_phy_config (struct phy_device *phydev) {
//	mx6_rmii_rework(phydev);
//
//	if (phydev->drv->config)
//		phydev->drv->config(phydev);
//
//	return 0;
//}


//int board_eth_init (bd_t *bis) {
//	uint32_t base = IMX_FEC_BASE;
//	struct mii_dev *bus = NULL;
//	struct phy_device *phydev = NULL;
//	int ret;
//	
//	enable_ethernet_rail ();
//	setup_iomux_enet();
//
//#ifdef CONFIG_FEC_MXC
//	bus = fec_get_miibus(base, -1);
//	if (!bus)
//		return 0;
//	/* scan phy 4,5,6,7 */
//	//phydev = phy_find_by_mask(bus, (0xf << 4), PHY_INTERFACE_MODE_RGMII);
//	phydev = phy_find_by_mask(bus, (0xFF), PHY_INTERFACE_MODE_RMII);
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
//#endif
//	return 0;
//}
