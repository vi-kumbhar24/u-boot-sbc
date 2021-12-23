#ifndef _C12_REVISION_H_
#define _C12_REVISION_H_

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
	HDMI = 2,
	LVDS_DCSS,
	LVDS_800x480,
	LVDS_LCDIF,
	HDMI_LVDS,
	NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
	WILINK = 0,
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

void c12_fdt_ram_setup(void *blob, bd_t *bdev);
void c12_fdt_vpu_setup(void *blob, bd_t *bdev);
void c12_fdt_pcie_setup(void *blob, bd_t *bdev);
int c12_get_board_configuration (void);
int c12_get_hw_revision ( void );
int c12_get_q_ql ( void );
int c12_get_spi_canrtc ( void );
int c12_get_pcie ( void );

typedef enum {
        RAM_1GB = 0,
        RAM_2GB = 0x1,
        RAM_4GB = 0x2,
        RAM_8GB = 0x3,
} RAM_STRAPS;

#define GET_C12_STRAPS  (c12_get_board_configuration() & 0x3)

#define C12_IS_1GB      (GET_C12_STRAPS == RAM_1GB)     
#define C12_IS_2GB      (GET_C12_STRAPS == RAM_2GB)
#define C12_IS_4GB      (GET_C12_STRAPS == RAM_4GB)

#define C12_HAS_HDMI    (~(c12_get_board_configuration()) & 0x4)
#define C12_IS_QLITE    (~(c12_get_q_ql()) & 0x1)
#define C12_HAS_WILINK  ((c12_get_board_configuration()) & 0x8)
#define C12_HAS_CANRTC  (~(c12_get_spi_canrtc()) & 0x1)
#define C12_FW_03       ((gd->fwrev & 0xf) >= 0x3) ? 1 : 0
#define C12_HAS_PCIE    (~(c12_get_pcie()) & 0x1)

#endif
