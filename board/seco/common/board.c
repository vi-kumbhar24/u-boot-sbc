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
#if defined CONFIG_ARCH_IMX8
#include <asm/arch/imx8-pins.h>
#endif
#include <asm/arch/crm_regs.h>
#include <asm/gpio.h>
#if defined CONFIG_ARCH_MX6 
#include <asm/arch/mx6-pins.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/mxc_hdmi.h>
#endif
#include <asm/mach-imx/boot_mode.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/sys_proto.h>
#include <linux/fb.h>
#include <micrel.h>
#include <i2c.h>
#include <dt-bindings/seco/ectrl.h>
#include <dt-bindings/seco/dm_ectrl.h>

#include "proto_seco.h"


DECLARE_GLOBAL_DATA_PTR; 



/*  __________________________________________________________________________
 * |                                                                          |
 * |                               BOOT VALIDATE                              |
 * |__________________________________________________________________________|
 */

#if defined CONFIG_ARCH_IMX8
void boot_validate (int gpio, const iomux_cfg_t gpio_pad) {

	SETUP_IOMUX_IMX8_PADS (gpio_pad);
	gpio_request(gpio,"boot_validate");
	gpio_direction_output (gpio, 1);
	mdelay(20);

        /* Set Low */
	gpio_set_value (gpio, 0);
    	mdelay(20);

        /* Set High */
	gpio_set_value (gpio, 1);
	gpio_free(gpio);
	
}
#endif

#if defined CONFIG_ARCH_MX6
void boot_validate (int gpio, iomux_v3_cfg_t const *gpio_pad, unsigned count) {

        imx_iomux_v3_setup_multiple_pads( gpio_pad, count );
		gpio_request(gpio,"boot_validate");
        gpio_direction_output (gpio, 0);

        /* Set Low */
        gpio_set_value (gpio, 0);
        udelay(1000);

        /* Set High */
        gpio_set_value (gpio, 1);
		gpio_free(gpio);
}



/*  __________________________________________________________________________
 * |                                                                          |
 * |                               BOARD REVISION                             |
 * |__________________________________________________________________________|
 */
int scode_value (int seco_code) {
	gpio_direction_input(seco_code);
	return gpio_get_value(seco_code);
}

struct udevice *get_ectrl_device( ) {
	int             ret;
	struct udevice  *dev;

	ret = uclass_get_device_by_name( UCLASS_I2C_GENERIC, "ectrl", &dev );
	if ( ret == -ENODEV ) {
	 	printf( "Ectrl device not found\n" );
		return NULL;
	}

	return dev;
}


int get_ectrl_board_revision( struct udevice *dev, u8 *id, char *major_v, u8 *minor_v ) {
	const struct dm_ectrl_ops  *ops;

	if ( !dev )
		return -1;

	ops = dev_get_driver_ops( dev );
	if ( !ops ) 
		return -1;

 	return ops->get_board_id( dev, id, major_v, minor_v );
}


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                 DISPLAY INFO                             |
 * |__________________________________________________________________________|
 */
#if CONFIG_VERBOSE_BOARD_NAME
int checkboard (void) {

	custom_check_board( );
	printf("Board: %s\n", board_name);

	return 0;
}
#endif



int print_boot_device (void) {

	uint soc_sbmr       = readl(SRC_BASE_ADDR + 0x4);
	uint bt_mem_ctl     = (soc_sbmr & 0x000000F0) >> 4 ;
	uint bt_mem_type    = (soc_sbmr & 0x00000008) >> 3;

	uint bt_usdhc_num   = (soc_sbmr & 0x00001800) >> 11;
	uint bt_port_select = (soc_sbmr & 0x05000000) >> 24;

	uint id = -1;
	uint port_sel = -1;
	uint mtype = -1;

	int index;

	for ( index = 0 ; index < SECO_NUM_BOOT_DEV ; index++ ) {

		if ( bt_mem_ctl >= boot_mem_dev_list[index].mem_ctl_min &&
				bt_mem_ctl <= boot_mem_dev_list[index].mem_ctl_max ) {

			if ( boot_mem_dev_list[index].mem_type != -1 )
				if ( boot_mem_dev_list[index].mem_type == bt_mem_type )
					mtype = bt_mem_type;

			if ( boot_mem_dev_list[index].bt_usdhc_num != -1 )
				id = bt_usdhc_num + 1;

			if ( boot_mem_dev_list[index].bt_port_select != -1 )
				if ( boot_mem_dev_list[index].bt_port_select == bt_port_select )
					port_sel = bt_port_select;

			break;

		}
	}

	if ( index < SECO_NUM_BOOT_DEV ) {
		printf ("Boot: %s\n", boot_mem_dev_list[index].label);
		if ( id != -1 )
			printf (" (bus id: %d)\n", id);
		if ( port_sel != -1 )
			printf ("%d\n", port_sel);
		if ( mtype != -1 )
			printf ("%d\n", mtype);
	} else
		printf ("Boot: Unknow\n"); 

	return index;
}




/*  __________________________________________________________________________
 * |                                                                          |
 * |                                 FILE INFO                                |
 * |__________________________________________________________________________|
 */
int fdt_autodetect( void ) {
	char *autodetect_str = env_get ("fdt_autodetect");

	if ( (autodetect_str != NULL) && (strcmp(autodetect_str, "yes") == 0) ) {
		return 1;
	}

	return 0;
}


void fdt_set( void ) {
	if ( fdt_autodetect( ) ) {
		printf( "FDT autodetect: " );
		if ( is_mx6dl() || is_mx6solo() || is_mx6sx() ) {
			env_set( "fdt_file", SCFG_DEFAULT_FDT_IMX6DL_FILE );
			printf ("%s\n", SCFG_DEFAULT_FDT_IMX6DL_FILE );
		} else if ( is_mx6dq() ) {
			env_set( "fdt_file", SCFG_DEFAULT_FDT_IMX6Q_FILE );
			printf ("%s\n", SCFG_DEFAULT_FDT_IMX6Q_FILE );
		} else {
			printf ("UNKNOW\n" );
		}
	}
}


int mem_autodetect( void ) {
	char *autodetect_str = env_get ("mem_autodetect");

	if ( (autodetect_str != NULL) && (strcmp(autodetect_str, "yes") == 0) ) {
		return 1;
	}

	return 0;
}


void memory_set( void ) {
	char memory_buff[50];
	ulong size = gd->ram_size / (1 << 20); // get size in MB

	if ( mem_autodetect( ) ) {

		printf( "kernel MEM  autodetect: " );

		if ( size < DEFAULT_CMA_VALUE )
			sprintf( memory_buff, "mem=%luM cma=128M", size );
		else
			sprintf( memory_buff, "mem=%luM", size );

		printf( "%s\n", memory_buff );
		env_set( "memory", memory_buff );

	}
}

#endif
