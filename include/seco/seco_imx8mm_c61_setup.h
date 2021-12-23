#ifndef _C61_REVISION_H_
#define _C61_REVISION_H_

#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/gpio.h>
#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>

#include <linux/libfdt.h>
/*
 *      This typedef must be matched with:
 *              overlay_video_mode_list[]
 *              overlay_addons_list[]
 *      in env_conf.c files 
 *
*/

typedef enum {
	NO_VIDEO = 0,
	EDP_VIDEO,
	LVDS_VIDEO,
	LVDS_LEDRIVER_VIDEO,
	NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
	GPIO_OPT = 0,
	RS232_OPT,
	RS485_OPT,
	CAN_OPT,
	NUM_ADDONS,
} ADDONS;

#define BOOT_DEV_ID_EMMC      __stringify(BOOT_ID_EMMC)"\0"
#define BOOT_DEV_ID_U_SD      __stringify(BOOT_ID_USD)"\0"
#define BOOT_DEV_ID_EXT_SD    __stringify(BOOT_ID_EXT_SD)"\0"
#define BOOT_DEV_ID_SPI       "0"
#define BOOT_DEV_ID_SATA      "0"
#define BOOT_DEV_ID_USB       "0"

#define ROOT_DEV_ID_EMMC      __stringify(ROOT_ID_EMMC)"\0"
#define ROOT_DEV_ID_U_SD      __stringify(ROOT_ID_USD)"\0"
#define ROOT_DEV_ID_EXT_SD    __stringify(ROOT_ID_EXT_SD)"\0"

void c61_fdt_ram_setup(void *blob, bd_t *bdev);
void c61_fdt_vpu_setup(void *blob, bd_t *bdev);
void c61_fdt_pcie_setup(void *blob, bd_t *bdev);
int c61_get_board_configuration (void);
int c61_get_configuration_port1 (void);
int c61_get_configuration_port2 (void);
int c61_get_configuration_video (void);
int c61_get_configuration_modem (void);

int c61_get_hw_revision ( void );
int c61_get_q_ql ( void );
int c61_get_spi_canrtc ( void );
int c61_get_pcie ( void );


#define C61_IS_1GB	((!(c61_get_board_configuration() & 0x1)) & (!((c61_get_board_configuration() & 0x2)>>1)))
#define C61_IS_2GB	(((c61_get_board_configuration() & 0x2)>>1) & ~(c61_get_board_configuration() & 0x1))
#define C61_IS_4GB      ((c61_get_board_configuration() & 0x1) & (c61_get_board_configuration() & 0x2)>>1)

#define C61_GET_CONF_PORT1	c61_get_configuration_port1()
#define C61_GET_CONF_PORT2	c61_get_configuration_port2()
#define C61_GET_CONF_VIDEO	c61_get_configuration_video()
#define C61_MODEM_IS_PRESENT	(c61_get_configuration_modem() & 0x1)
#endif
