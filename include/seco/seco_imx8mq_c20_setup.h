#ifndef _C20_REVISION_H_
#define _C20_REVISION_H_

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
	eDP,
        NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
        NUM_ADDONS = 0,
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

void c20_fdt_ram_setup(void *blob, bd_t *bdev);
void c20_fdt_vpu_setup(void *blob, bd_t *bdev);
void c20_fdt_rev_setup(void *blob, bd_t *bdev);
int c20_get_board_configuration (void);
int c20_get_board_rev (void);

typedef enum {
        RAM_1GB = 0,
        RAM_2GB = 0x1,
        RAM_4GB = 0x2,
        RAM_8GB = 0x3,
} RAM_STRAPS;

typedef enum {
        REV_B = 0,
        REV_C = 0x1,
} BOARD_REV;

#define GET_C20_STRAPS  (c20_get_board_configuration() & 0x3)

#define C20_IS_1GB	(GET_C20_STRAPS == RAM_1GB)
#define C20_IS_2GB	(GET_C20_STRAPS == RAM_2GB)
#define C20_IS_4GB      (GET_C20_STRAPS == RAM_4GB)

#define GET_C20_REV	(c20_get_board_rev() & 0x1)

#define C20_IS_REVB	(GET_C20_REV == REV_B)
#define C20_IS_REVC	(GET_C20_REV == REV_C)

#endif
