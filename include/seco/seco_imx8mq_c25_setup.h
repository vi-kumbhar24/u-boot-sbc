#ifndef _C25_REVISION_H_
#define _C25_REVISION_H_

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
        EDP_DCSS,
        DP_DCSS,
        NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
	CAN_RTC = 0,
	Q7_SPI,
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


void c25_fdt_ram_setup(void *blob, bd_t *bdev);
void c25_fdt_vpu_setup(void *blob, bd_t *bdev);
int c25_get_board_configuration (void);

typedef enum {
        RAM_1GB = 0,
        RAM_2GB = 0x1,
        RAM_4GB = 0x2,
        RAM_8GB = 0x3,
} RAM_STRAPS;

#define GET_C25_STRAPS  (c25_get_board_configuration() & 0x3)

#define C25_IS_1GB	(GET_C25_STRAPS == RAM_1GB)
#define C25_IS_2GB	(GET_C25_STRAPS == RAM_2GB)
#define C25_IS_4GB      (GET_C25_STRAPS == RAM_4GB)
#define C25_IS_8GB      (GET_C25_STRAPS == RAM_8GB)

#define C25_HAS_HDMI	!(c25_get_board_configuration() & 0x4)
#define C25_IS_QLITE	C25_IS_1GB
#define C25_HAS_WILINK	!(c25_get_board_configuration() & 0x10)
#define C25_HAS_CANRTC  !(!(c25_get_board_configuration() & 0x40)>>1 &  (c25_get_board_configuration() & 0x20)) /* true for conf = 0x0, conf = 0x1, conf = 0x3*/

#define C25_HAS_UART4	!((c25_get_board_configuration() & 0x40)>>6) & !((c25_get_board_configuration() & 0x20)>>5) /* true for conf = 0x0 */
#define C25_HAS_Q7SPI   (c25_get_board_configuration() & 0x20) /* true for conf = 0x1, conf = 0x3 */  

#endif
