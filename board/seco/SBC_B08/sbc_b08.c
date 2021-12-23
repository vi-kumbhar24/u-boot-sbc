// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2012-2016 SECO srl.
 * Copyright 2017-2018 NXP
 *
 * Author:  Vivek Kumbhar <vivek.kumbhar@seco.com>
 */


#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/mach-imx/spi.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/boot_mode.h>
#include <asm/mach-imx/video.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/mxc_hdmi.h>
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <input.h>
#include <micrel.h>
#include <usb.h>
#include <usb/ehci-ci.h>
#include <power/regulator.h>

#ifdef CONFIG_FSL_FASTBOOT
#include <fb_fsl.h>
#ifdef CONFIG_ANDROID_RECOVERY
#include <recovery.h>
#endif
#endif /*CONFIG_FSL_FASTBOOT*/


#include "pin_muxing_SBC_B08.h"
#include "detect.h"
#include "../common/proto_seco.h"
#include "../common/pfuze.h"

DECLARE_GLOBAL_DATA_PTR;



static int detect_revision( void );
static int detect_board( void );


int dram_init( void ) {
	gd->ram_size = imx_ddr_size();
	return 0;
}

void ldo_mode_set(int ldo_bypass) {}


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   UART                                   |
 * |__________________________________________________________________________|
 */
static void setup_iomux_uart( void ) {
	imx_iomux_v3_setup_multiple_pads(uart1_pads, ARRAY_SIZE(uart1_pads)); /*uart1*/
	imx_iomux_v3_setup_multiple_pads(uart2_pads, ARRAY_SIZE(uart2_pads)); /*uart2*/
	imx_iomux_v3_setup_multiple_pads(uart5_pads, ARRAY_SIZE(uart5_pads)); /*debug uart*/
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                            BOARD DETECTION                               |
 * |__________________________________________________________________________|
 */


void board_code_init( void ) {
    imx_iomux_v3_setup_multiple_pads(gpio_board_code_pad, ARRAY_SIZE(gpio_board_code_pad));
	imx_iomux_v3_setup_multiple_pads(board_recognition_pads, ARRAY_SIZE(board_recognition_pads));
}


int detect_revision( ) {
	int board_rev;

	gpio_request( BOARD_CODE_0, "CODE BIT0" );
    gpio_request( BOARD_CODE_1, "CODE BIT1" );
	gpio_direction_input( BOARD_CODE_0 );
	gpio_direction_input( BOARD_CODE_1 );
    board_rev = ( gpio_get_value(BOARD_CODE_0) << 0 |  gpio_get_value(BOARD_CODE_1) << 1 ) & 0x3;
	gpio_free( BOARD_CODE_0 );
    gpio_free( BOARD_CODE_1 );

    return board_rev;
}


int detect_board( ) {
    int status;

    gpio_request( BOARD_CONF, "BOARD CONF" );
    gpio_direction_input( BOARD_CONF );
    status = gpio_get_value(BOARD_CONF);
    gpio_free( BOARD_CONF );

    if (status == 0)
		return SECO_B08_TYPE_BASIC;
    else
		return SECO_B08_TYPE_FULL;
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   USDHC                                  |
 * |__________________________________________________________________________|
 */
#ifdef CONFIG_FSL_ESDHC

/* map the usdhc controller id to the devno given to the board device */
int usdhc_devno[4] = { -1, 1, -1, 0};

boot_mem_dev_t boot_mem_dev_list[SECO_NUM_BOOT_DEV] = {
	{ 0x3, 0x3, -1,  -1,  0, "SPI Nor" },
	{ 0x4, 0x5, -1,   0, -1, "External SD" },
	{ 0x6, 0x7, -1,  -1, -1, "eMMC" },
};

#ifdef CONFIG_SPL_BUILD

#define USDHC2_PWR_GPIO                               IMX_GPIO_NR(6, 1)
#define USDHC2_CD_GPIO	                              IMX_GPIO_NR(6, 2) 


/* USDHC map:
 * 	USDHC4  -->  eMMC  -->  FSL_SDHC: 0
 * 	USDHC2  -->  uSD   -->  FSL_SDHC: 1
 */
struct fsl_esdhc_cfg usdhc_cfg[CONFIG_SYS_FSL_USDHC_NUM] = {
	{USDHC4_BASE_ADDR, 0, 8},
	{USDHC2_BASE_ADDR, 0, 4},
};


struct usdhc_l usdhc_list_spl[CONFIG_SYS_FSL_USDHC_NUM] = {
	{usdhc4_pads, ARRAY_SIZE(usdhc4_pads), -1},
	{usdhc2_pads, ARRAY_SIZE(usdhc2_pads), USDHC2_CD_GPIO, USDHC2_PWR_GPIO},
};

enum mxc_clock usdhc_clk[CONFIG_SYS_FSL_USDHC_NUM] = {
	MXC_ESDHC4_CLK,
	MXC_ESDHC2_CLK,
};




int board_mmc_getcd (struct mmc *mmc) {
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;
	int type = detect_board ( );
	switch (cfg->esdhc_base) {
		case USDHC2_BASE_ADDR:
			ret = !gpio_get_value( USDHC2_CD_GPIO );
			break;
		case USDHC4_BASE_ADDR:
            if ( is_SBC_BASIC( type ) ) {
			    ret = 0; /* eMMC/uSDHC4 is always present only in FULL configuration */
            } else {
                ret = 1;
            }
			break;
	}
	return ret;
}


#if defined(CONFIG_MX6SX)

int board_mmc_init( bd_t *bis ) {
	struct src *psrc = (struct src *)SRC_BASE_ADDR;
	unsigned reg = readl(&psrc->sbmr1) >> 11;
	/*
	 * Upon reading BOOT_CFG register the following map is done:
	 * Bit 11 and 12 of BOOT_CFG register can determine the current
	 * mmc port
	 * 0x2                  SD3
	 * 0x3                  SD4
	 */
	int ret = 0;
	int index = 0;
	
	switch (reg & 0x3) {
		case 1:
			index = 1;
			break;
		case 3:
			index = 0;
			break;
	}


	print_boot_device( );
	
	imx_iomux_v3_setup_multiple_pads( usdhc_list_spl[index].pad, usdhc_list_spl[index].num );	
	usdhc_cfg[index].sdhc_clk = mxc_get_clock( usdhc_clk[index] );
	gd->arch.sdhc_clk = usdhc_cfg[index].sdhc_clk;
	ret = fsl_esdhc_initialize( bis, &usdhc_cfg[index] );

	if ( ret ) 
		printf( "Warning: failed to initialize mmc dev %d\n", index );

	return ret;
}

#endif
#endif	/*  CONFIG_SPL_BUILD  */
#endif  /*  CONFIG_FSL_ESDHC  */
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   ETHERNET                               |
 * |__________________________________________________________________________|
 */
int board_phy_config (struct phy_device *phydev) {
	phy_write (phydev, MDIO_DEVAD_NONE, MII_BMCR, 0x2100);
	phydev->autoneg = 0;

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}


static int setup_fec( void ) {
	struct iomuxc *iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;
	struct anatop_regs *anatop = (struct anatop_regs *)ANATOP_BASE_ADDR;
	int type = detect_board( );
	int reg;

	/* Use 125M anatop loopback REF_CLK1 for ENET1, clear gpr1[13], gpr1[17]*/
	clrsetbits_le32(&iomuxc_regs->gpr[1], IOMUX_GPR1_FEC1_MASK, 0);
	enable_fec_anatop_clock(0, ENET_50MHZ);

	if ( is_SBC_FULL( type ) ) {	
		/* Use 125M anatop loopback REF_CLK1 for ENET2, clear gpr1[14], gpr1[18]*/
		clrsetbits_le32(&iomuxc_regs->gpr[1], IOMUX_GPR1_FEC2_MASK, 0);
		enable_fec_anatop_clock(1, ENET_50MHZ);
	}
	
	mdelay( 250 );

	reg = readl(&anatop->pll_enet);
	reg |= BM_ANADIG_PLL_ENET_REF_25M_ENABLE;
	writel(reg, &anatop->pll_enet);
		
	return 0;
}
 


int board_eth_init(bd_t *bis)
{
	return cpu_eth_init(bis);
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                    USB                                   |
 * |__________________________________________________________________________|
 */
#ifdef CONFIG_USB_EHCI_MX6

#define HSIC_RST                                 IMX_GPIO_NR(5, 2)
#define HSIC_INT                                 IMX_GPIO_NR(2, 11)

#define USB_HOST3_PWR                            IMX_GPIO_NR(4, 8)
#define USB_HOST4_PWR                            IMX_GPIO_NR(4, 9)


#define USB_OTHERREGS_OFFSET	                 0x800
#define UCTRL_PWR_POL                            (1 << 9)


int board_ehci_hcd_init (int port) {
	u32 *usbnc_usb_ctrl;

	switch (port) {
	case 0:
		SETUP_IOMUX_PADS( usb_otg1_pads );
		break;
	case 1:
		SETUP_IOMUX_PADS ( usb_otg2_pads );
		break;
	case 2:
		SETUP_IOMUX_PADS( usb_hsic_pads );
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return -EINVAL;
	}

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +
				 port * 4);

	/* Set Power polarity */
	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}
#endif  /*  CONFIG_USB_EHCI_MX6  */
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


int power_init_board(void)
{
	struct udevice *dev;

	dev = pfuze3000_common_init();
	if (!dev)
		return -ENODEV;

	return 0;
}




/*  __________________________________________________________________________
 * |                                                                          |
 * |                              DISPLAY SETTINGS                            |
 * |__________________________________________________________________________|
 */

#ifdef CONFIG_VIDEO

char *display_name;


#define LVDS_PANEL_ON                            IMX_GPIO_NR(4, 18)
static iomux_v3_cfg_t const lvds_pads[] = {
	MX6_PAD_QSPI1A_DATA2__GPIO4_IO_18 | MUX_PAD_CTRL(NO_PAD_CTRL),	// PANEL_ON
};


#define LVDS_BACKLIGHT_GPIO                      IMX_GPIO_NR(6, 3) 
static iomux_v3_cfg_t const lvds_backlight_pads[] = {
	MX6_PAD_SD1_DATA1__GPIO6_IO_3 | MUX_PAD_CTRL(NO_PAD_CTRL),	// BL_ON
};


void setup_iomux_backlight( int en ) {
	gpio_direction_output ( LVDS_BACKLIGHT_GPIO, en );
}


void enable_lvds( struct display_info_t const *dev ) {
	int ret;

	ret = enable_lcdif_clock( dev->bus, 1 );
	if ( ret ) {
		printf( "Enable LCDIF clock failed, %d\n", ret );
		return;
	}

	setup_display_sx( dev->bus );
	setup_lvds( dev );
	imx_iomux_v3_setup_multiple_pads( lvds_pads, ARRAY_SIZE( lvds_pads ) );
	imx_iomux_v3_setup_multiple_pads( lvds_backlight_pads, ARRAY_SIZE( lvds_backlight_pads ) );

	gpio_request( LVDS_PANEL_ON, "lvds backlight" );
	gpio_direction_output( LVDS_PANEL_ON, 1 );

	gpio_request( LVDS_BACKLIGHT_GPIO, "lvds panel_on" );
	gpio_direction_output ( LVDS_BACKLIGHT_GPIO, 1 );
}


void disable_lvds( struct display_info_t const *dev ) {
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	int reg = readl( &iomux->gpr[2] );

	reg &= ~(IOMUXC_GPR2_LVDS_CH0_MODE_MASK |
		 IOMUXC_GPR2_LVDS_CH1_MODE_MASK);

	writel(reg, &iomux->gpr[2]);
	gpio_direction_output( LVDS_PANEL_ON, 0 );
	setup_iomux_backlight( 0 );
}



#define RGB_PANEL_ON                             IMX_GPIO_NR(4, 21)
static iomux_v3_cfg_t const rgb_pads[] = {
	MX6_PAD_LCD1_CLK__LCDIF1_CLK | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_ENABLE__LCDIF1_ENABLE | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_HSYNC__LCDIF1_HSYNC | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_VSYNC__LCDIF1_VSYNC | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA00__LCDIF1_DATA_0 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA01__LCDIF1_DATA_1 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA02__LCDIF1_DATA_2 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA03__LCDIF1_DATA_3 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA04__LCDIF1_DATA_4 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA05__LCDIF1_DATA_5 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA06__LCDIF1_DATA_6 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA07__LCDIF1_DATA_7 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA08__LCDIF1_DATA_8 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA09__LCDIF1_DATA_9 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA10__LCDIF1_DATA_10 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA11__LCDIF1_DATA_11 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA12__LCDIF1_DATA_12 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA13__LCDIF1_DATA_13 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA14__LCDIF1_DATA_14 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA15__LCDIF1_DATA_15 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA16__LCDIF1_DATA_16 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA17__LCDIF1_DATA_17 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA18__LCDIF1_DATA_18 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA19__LCDIF1_DATA_19 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA20__LCDIF1_DATA_20 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA21__LCDIF1_DATA_21 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA22__LCDIF1_DATA_22 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_DATA23__LCDIF1_DATA_23 | MUX_PAD_CTRL(LCD_PAD_CTRL),
	MX6_PAD_LCD1_RESET__GPIO3_IO_27 | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* CEC Clock */
	MX6_PAD_GPIO1_IO12__CCM_CLKO2 | MUX_PAD_CTRL(NO_PAD_CTRL),

	MX6_PAD_QSPI1A_SCLK__GPIO4_IO_21 | MUX_PAD_CTRL(NO_PAD_CTRL),  // RGB_PANEL_ON
};


#define RGB_BACKLIGHT_CTRL_GPIO                    IMX_GPIO_NR(5, 20) 
#define RBG_BACKLIGHT_ON_GPIO                      IMX_GPIO_NR(5, 23) 
iomux_v3_cfg_t const rgb_backlight_pads[] = {
	MX6_PAD_RGMII2_TD2__GPIO5_IO_20 | MUX_PAD_CTRL(NO_PAD_CTRL),	// RGB_BLT_CTRL
	MX6_PAD_RGMII2_TXC__GPIO5_IO_23 | MUX_PAD_CTRL(NO_PAD_CTRL),	// RGB_BL_ON
};


void enable_rgb (struct display_info_t const *dev) {
	imx_iomux_v3_setup_multiple_pads( rgb_pads, ARRAY_SIZE( rgb_pads ) );

	gpio_request( RGB_PANEL_ON, "rgn panel_on" );
	gpio_direction_output ( RGB_PANEL_ON, 1 );

	gpio_request( RGB_BACKLIGHT_CTRL_GPIO, "rgb backlight_ctrl" );
	gpio_direction_output ( RGB_BACKLIGHT_CTRL_GPIO, 1 );

	gpio_request( RBG_BACKLIGHT_ON_GPIO, "rgb backlight_on" );
	gpio_direction_output ( RBG_BACKLIGHT_ON_GPIO, 1 );
}


void disable_rgb (struct display_info_t const *dev) {
	gpio_direction_output ( RGB_PANEL_ON, 0 );
	gpio_direction_output ( RGB_BACKLIGHT_CTRL_GPIO, 0 );
	gpio_direction_output ( RBG_BACKLIGHT_ON_GPIO, 0 );

}

#endif
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */

#if CONFIG_VERBOSE_BOARD_NAME
#ifdef CONFIG_SECO_BOARD_NAME
char *board_name = CONFIG_SECO_BOARD_NAME;
#else
char *board_name = "";
char *board_name_revB = "RevB";
char *board_name_revC = "RevC";
#endif


void custom_check_board ( ) {
}
#endif



/*
 * Do not overwrite the console
 * Use always serial for U-Boot console
 */
int overwrite_console (void) {
	return 1;
}


int board_early_init_f( void ) {
	
	board_code_init( );
	setup_iomux_uart();
 	
	return 0;
}



int board_init( void ) {
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

#if defined(CONFIG_DM_REGULATOR)
	//regulators_enable_boot_on( false );
#endif

setup_fec( );
	return 0;
}


#ifdef CONFIG_CMD_BMODE
static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"sd3",	 MAKE_CFGVAL(0x40, 0x30, 0x00, 0x00)},
	/* 8 bit bus width */
	{"emmc", MAKE_CFGVAL(0x60, 0x58, 0x00, 0x00)},
	{NULL,	 0},
};
#endif

int board_late_init( void ) {
	
	char memory_buff[50];
    char fdt_buff[100];
	int type = detect_board( );
	int rev = detect_revision( );

#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes( board_boot_modes );
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	
	if ( is_SBC_REVC( rev ) )
		env_set( "board_rev", "RevC" );
	else
		env_set( "board_rev", "RevB" );
		
#endif

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init( );
#endif

	/*  custom fdt_set and memory_set  */
    if ( is_SBC_BASIC( type ) ) {
        sprintf( memory_buff, "mem=512M" );
        sprintf( fdt_buff, SCFG_DEFAULT_FDT_B08_BASIC );
    } else {
        sprintf( memory_buff, "mem=1024M" );
        sprintf( fdt_buff, SCFG_DEFAULT_FDT_B08_FULL );   	
    }

    env_set( "memory", memory_buff );
    printf( "kernel MEM  autodetect: %s\n", memory_buff );
    
    env_set ( "fdt_file", fdt_buff );
    printf( "FDT autodetect: %s\n", fdt_buff );

	return 0;
}



void  board_custom_boot ( void ) {
}


#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_ANDROID_RECOVERY

#define GPIO_VOL_DN_KEY IMX_GPIO_NR(1, 5)
iomux_v3_cfg_t const recovery_key_pads[] = {
	IOMUX_PADS(PAD_GPIO_5__GPIO1_IO05 | MUX_PAD_CTRL(NO_PAD_CTRL)),
};

int is_recovery_key_pressing(void)
{
	int button_pressed = 0;

	/* Check Recovery Combo Button press or not. */
	SETUP_IOMUX_PADS(recovery_key_pads);

	gpio_request(GPIO_VOL_DN_KEY, "volume_dn_key");
	gpio_direction_input(GPIO_VOL_DN_KEY);

	if (gpio_get_value(GPIO_VOL_DN_KEY) == 0) { /* VOL_DN key is low assert */
		button_pressed = 1;
		printf("Recovery key pressed\n");
	}

	return  button_pressed;
}

#endif /*CONFIG_ANDROID_RECOVERY*/

#endif /*CONFIG_FSL_FASTBOOT*/

#ifdef CONFIG_SPL_BUILD


#include <spl.h>
#include <linux/libfdt.h>

#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
	// gpio_request(KEY_VOL_UP, "KEY Volume UP");
	// gpio_direction_input(KEY_VOL_UP);

	// /* Only enter in Falcon mode if KEY_VOL_UP is pressed */
	// return gpio_get_value(KEY_VOL_UP);
	return 0;
}
#endif


#include "ddr_config.h"

static void spl_dram_init( int type ) {
	if ( is_SBC_BASIC( type ) )
	 	ddr_init( mx6sx_2x256_dcd_table, ARRAY_SIZE( mx6sx_2x256_dcd_table ) );
	else
	 	ddr_init( mx6sx_2x512_dcd_table, ARRAY_SIZE( mx6sx_2x512_dcd_table ) );	
}






void board_init_f( ulong dummy ) {
	int type;
	board_code_init( );
 	type = detect_board ( );

	/* DDR initialization */
	spl_dram_init( type );

	/* setup AIPS and disable watchdog */
	arch_cpu_init();

	ccgr_init();
	
 	/* iomux and setup of i2c */
	board_early_init_f();

	/* setup GP timer */
	timer_init();

	/* UART clocks enabled and gd valid - init serial console */
	preloader_console_init();

	setup_fec();

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	/* load/boot image from boot device */
	board_init_r(NULL, 0);
}

void reset_cpu (ulong addr){
}

#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	int type = detect_board ( );
	if ( is_SBC_BASIC( type ) ) {
		if (!strcmp(name, "imx6sx-seco_SBC_B08_basic"))
			return 0;
	} else {
		if (!strcmp(name, "imx6sx-seco_SBC_B08_full"))
			return 0;
	}

	return -1;
}
#endif
