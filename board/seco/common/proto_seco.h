/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


/*
 * proto_seco.h - definitions an prototypes for seco platform device
 */

#ifndef _PROTO_SECO_H_
#define _PROTO_SECO_H_

#if defined CONFIG_ARCH_MX6
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/mach-imx/video.h>
#endif
#include <fsl_esdhc.h>
#include <ipu_pixfmt.h>
#if defined CONFIG_ARCH_IMX8
#include <asm/arch/iomux.h>
#include <asm/arch/imx8-pins.h>
#endif

/*  __________________________________________________________________________
 * |                                                                          |
 * |                               BOOT VALIDATE                              |
 * |__________________________________________________________________________|
 */
#if defined CONFIG_ARCH_IMX8

#define SETUP_IOMUX_IMX8_PADS(x) imx8_iomux_setup_pad(x); 

void boot_validate (int gpio, const iomux_cfg_t gpio_pad);
#endif
#if defined CONFIG_ARCH_MX6


typedef struct boot_mem_dev {
	uint mem_ctl_min;
	uint mem_ctl_max;
	uint mem_type;
	uint bt_usdhc_num;
	uint bt_port_select;
	char *label; 
} boot_mem_dev_t;

extern boot_mem_dev_t boot_mem_dev_list[SECO_NUM_BOOT_DEV];



/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   USDHC                                  |
 * |__________________________________________________________________________|
 */

struct usdhc_l {
	iomux_v3_cfg_t const *pad;
	int num;
	int gpio_cd;
	int gpio_pwr;
};

extern struct fsl_esdhc_cfg usdhc_cfg[CONFIG_SYS_FSL_USDHC_NUM];
#ifdef CONFIG_SPL_BUILD
extern struct usdhc_l usdhc_list_spl[CONFIG_SYS_FSL_USDHC_NUM];
#else
extern struct usdhc_l usdhc_list[CONFIG_SYS_FSL_USDHC_NUM];
#endif
extern enum mxc_clock usdhc_clk[CONFIG_SYS_FSL_USDHC_NUM];

extern void board_late_mmc_env_init (void);

extern int check_mmc_autodetect (void);

extern int usdhc_devno[4];


/*  __________________________________________________________________________
 * |                                                                          |
 * |                               BOOT VALIDATE                              |
 * |__________________________________________________________________________|
 */
extern void boot_validate (int gpio, iomux_v3_cfg_t const *gpio_pad, unsigned count);



/*  __________________________________________________________________________
 * |                                                                          |
 * |                               BOARD REVISION                             |
 * |__________________________________________________________________________|
 */
extern struct udevice *get_ectrl_device( void );
extern int get_seco_board_revision (struct i2c_pads_info *i2c_pad, unsigned char **boardrev);
extern int get_ectrl_board_revision( struct udevice *dev, u8 *id, char *major_v, u8 *minor_v );

__weak void custom_check_board( void );


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                 DISPLAY INFO                             |
 * |__________________________________________________________________________|
 */
extern char *board_name;
extern int print_boot_device (void);

extern void fdt_set( void );
extern void memory_set( void );

/*  __________________________________________________________________________
 * |                                                                          |
 * |                                  ETHERNET                                |
 * |__________________________________________________________________________|
 */
#include <miiphy.h>
#include <netdev.h>

struct ksz90xx_pins {
	int gpio_rst;
	int gpio_pwr;
	int gpio_clken;
	int gpio_rd0;
	int gpio_rd1;
	int gpio_rd2;
	int gpio_rd3;
};

extern struct ksz90xx_pins ksz9031_pad;

extern void setup_ksz90xx_iomux_enet( struct ksz90xx_pins ksz90xx);

//extern void setup_ksx90xx_iomux_enet( struct ksz90xx_pins ksz90xx );
extern int ksz90xx_rgmii_rework( struct phy_device *phydev );


struct ksz80xx_pins {
	int gpio_rst;
	int gpio_pwr;
	int gpio_clken;
};

extern struct ksz80xx_pins ksz8091_pad;

extern void setup_ksz80xx_iomux_enet( struct ksz80xx_pins ksz80xx);

//extern void setup_ksz80xx_iomux_enet( struct ksz80xx_pins ksz80xx );
extern int ksz80xx_rmii_rework( struct phy_device *phydev );


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                 DISPLAY                                  |
 * |__________________________________________________________________________|
 */
#ifdef SECO_COMMON_DISPLAY

extern char *display_name;

extern void setup_display (void);
extern void setup_display_sx (u32 lcd_base_addr);

#ifdef SECO_COMMON_LVDS
extern void enable_lvds (struct display_info_t const *dev);
extern void disable_lvds (struct display_info_t const *dev);
extern void setup_lvds ( struct display_info_t const *dev );
#endif  /* SECO_COMMON_LVDS */

#ifdef SECO_COMMON_RGB
extern void enable_rgb (struct display_info_t const *dev);
extern void disable_rgb (struct display_info_t const *dev);
#endif  /* SECO_COMMON_RGB */

extern void setup_iomux_backlight (int en );

#endif   /* SECO_COMMON_DISPLAY */


#endif /* CONFIG_ARCH_MX6 */
#endif   /*  _PROTO_SECO_H_  */
