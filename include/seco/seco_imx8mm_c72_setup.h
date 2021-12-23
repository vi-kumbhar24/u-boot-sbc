#ifndef _C72_REVISION_H_
#define _C72_REVISION_H_

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
        NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
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

int c72_get_board_configuration (void);

typedef enum {
        RAM_x32_1GB   = 0,
        RAM_x32_2GB   = 0x1,
        RAM_x32_4GB   = 0x2,
        RAM_x16_512MB = 0x4,
        RAM_x16_1GB   = 0x5,
        RAM_x16_2GB   = 0x6,
} RAM_STRAPS;

#define GET_C72_STRAPS  (c72_get_board_configuration() & 0x3)

#define C72_IS_1GB      (GET_C72_STRAPS == RAM_x32_1GB)     
#define C72_IS_2GB      (GET_C72_STRAPS == RAM_x32_2GB)
#define C72_IS_4GB      (GET_C72_STRAPS == RAM_x32_4GB)

#endif
