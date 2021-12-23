#ifndef __MX6SECO_SBC_A62_CONFIG_H
#define __MX6SECO_SBC_A62_CONFIG_H



#define MACH_SECO_TYPE                           4800
#define CONFIG_MACH_TYPE                         MACH_SECO_TYPE

#define SECO_NUM_BOOT_DEV                        2  // eMMC, uSD



#include "mx6_seco_dtbo.h"

#define SECO_SYS_CONFIGURATION
#define ENV_DEFAULT_DISPLAY                      "panel=LDB-1280P60\0"
#define ENV_DEFAULT_VIDEO_MODE                   "videomode=video=mxcfb0:dev=ldb,LDB-1280P60,datamap=spwg,if=RGB24,bpp=32\0"
#define ENV_DEFAULT_VIDEO_SETING                 "fdt_overlay_video_list=\0"
#define ENV_DEFAULT_PERIPHERAL                   "fdt_overlay_per_list=\0"


#include "mx6seco_common.h"

/* ____________________________________________________________________________
  |                                                                            |
  |                                     UART                                   |
  |____________________________________________________________________________|
*/
#define CONFIG_MXC_UART_BASE                     UART2_BASE
#define ENV_CONSOLE_DEV                          "ttymxc1"



/* ____________________________________________________________________________
  |                                                                            |
  |                                   WATCHDOG                                 |
  |____________________________________________________________________________|
 */
#define APX_WDT_TRIGGER_BASE                     GPIO3_BASE_ADDR
#define APX_WDT_TRIGGER_NUM                      25

#define APX_WDT_ENABLE_BASE                      GPIO4_BASE_ADDR
#define APX_WDT_ENABLE_NUM                       11



/* ____________________________________________________________________________
  |                                                                            |
  |                                       SPI                                  |
  |____________________________________________________________________________|
*/




/* ____________________________________________________________________________
  |                                                                            |
  |                                     USDHC                                  |
  |____________________________________________________________________________|
*/
#define CONFIG_SYS_FSL_USDHC_NUM                 2
#define CONFIG_SYS_MMC_ENV_DEV                   0                 /*USDHC2*/
#define CONFIG_SYS_MMC_ENV_PART                  0	               /* user area */
#define CONFIG_MMCROOT	                         "/dev/mmcblk0p2"  /* USDHC2 */

#define CONFIG_SYS_FSL_ESDHC_ADDR                0 //USDHC2_BASE_ADDR



/* ____________________________________________________________________________
  |                                                                            |
  |                                    ETHERNET                                |
  |____________________________________________________________________________|
*/
#define CONFIG_FEC_ENET_DEV                      0
#define CONFIG_FEC_MXC_PHYADDR                   0x6
#define IMX_FEC_BASE			                       ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE                      RGMII



/* ____________________________________________________________________________
  |                                                                            |
  |                                       USB                                  |
  |____________________________________________________________________________|
*/
#define CONFIG_MXC_USB_PORTSC                    (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS                     0
#define CONFIG_USB_MAX_CONTROLLER_COUNT          7



/* ____________________________________________________________________________
  |                                                                            |
  |                                       I2C                                  |
  |____________________________________________________________________________|
*/
#ifdef CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C2	                 /* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3	                 /* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SLAVE                     0
#endif



/* ____________________________________________________________________________
  |                                                                            |
  |                                    PROMPT                                  |
  |____________________________________________________________________________|
*/
#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT                        "MX6QDLS SYS-A62-10 U-Boot > "



/* ____________________________________________________________________________
  |                                                                            |
  |                                   DISPLAY                                  |
  |____________________________________________________________________________|
*/
#ifdef CONFIG_VIDEO
#define SECO_COMMON_DISPLAY
#define SECO_COMMON_HDMI
#define SECO_COMMON_LVDS
#undef  SECO_COMMON_RGB
#endif



/* ____________________________________________________________________________
  |                                                                            |
  |                                 ENVIRONMENT                                |
  |____________________________________________________________________________|
*/


#define SCFG_DEFAULT_FDT_IMX6Q_FILE            "imx6q-seco_SYS_A62_10.dtb"
#define SCFG_DEFAULT_FDT_IMX6DL_FILE           "imx6dl-seco_SYS_A62_10.dtb"


/*  SCFG = SECO CONFIG  */
#define BOARD_BOOT_ID_EMMC                       0
#define BOARD_BOOT_ID_EXT_SD                     1

#define BOARD_ROOT_ID_EMMC                       0
#define BOARD_ROOT_ID_EXT_SD                     1




#define SCFG_KERNEL_BOOT_DEV_IMX6Q_NUM           6
#define SCFG_KERNEL_BOOT_DEV_IMX6DL_NUM          5

#define SCFG_FDT_BOOT_DEV_IMX6Q_NUM              6
#define SCFG_FDT_BOOT_DEV_IMX6DL_NUM             5

#define SCFG_RAMFS_BOOT_DEV_IMX6Q_NUM            7
#define SCFG_RAMFS_BOOT_DEV_IMX6DL_NUM           6

#define SCFG_ROOTFS_BOOT_DEV_IMX6Q_NUM           5
#define SCFG_ROOTFS_BOOT_DEV_IMX6DL_NUM          4



#define SCFG_BOOT_DEV_ID_EMMC                    __stringify(BOARD_BOOT_ID_EMMC)"\0"
#define SCFG_BOOT_DEV_ID_U_SD                    "0"
#define SCFG_BOOT_DEV_ID_EXT_SD                  __stringify(BOARD_BOOT_ID_EXT_SD)"\0"
#define SCFG_BOOT_DEV_ID_SPI                     "0"
#define SCFG_BOOT_DEV_ID_SATA                    "0"
#define SCFG_BOOT_DEV_ID_USB                     "0"

#define SCFG_ROOT_DEV_ID_EMMC                    __stringify(BOARD_ROOT_ID_EMMC)"\0"
#define SCFG_ROOT_DEV_ID_U_SD                    "0"
#define SCFG_ROOT_DEV_ID_EXT_SD                  __stringify(BOARD_ROOT_ID_EXT_SD)"\0"
#define SCFG_ROOT_DEV_ID_SATA                    "0"
#define SCFG_ROOT_DEV_ID_USB                     "0"


#define LOAD_ADDR_KERNEL_LOCAL_DEV               __stringify(CONFIG_LOADADDR)"\0"
#define LOAD_ADDR_KERNEL_REMOTE_DEV              "0x12000000"

#define LOAD_ADDR_FDT_LOCAL_DEV                  __stringify(CONFIG_FDT_LOADADDR)"\0"
#define LOAD_ADDR_FDT_REMOTE_DEV                 "0x18000000"

#define LOAD_ADDR_RAMFS_LOCAL_DEV                __stringify(CONFIG_RAMFS_LOADADDR)"\0"
#define LOAD_ADDR_RAMFS_REMOTE_DEV               "0x12C00000"

#define SPI_LOAD_ADDRESS                         __stringify(CONFIG_SPI_KERNEL_LOADADDR)"\0"
#define SPI_LOAD_LEN                             __stringify(CONFIG_SPI_KERNEL_LEN)"\0"


#define SCFG_SET_VIDEOMODE                       0
#define SCFG_VIDEO_MODE_NUM                      0

#ifdef CONFIG_OF_LIBFDT_OVERLAY
#define SCFG_FDT_OVERLAY_PERIPHERAL_NUM          1
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */

#define CONFIG_CMD_SECO_CONFIG





#endif    /*  __MX6SECO_SBC_A62_CONFIG_H  */
