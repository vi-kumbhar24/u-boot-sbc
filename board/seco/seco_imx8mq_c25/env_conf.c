/*
 * 
 * Copyright SECO S.p.A. 
 * Mantained by marco.sandrelli@seco.com
 *
 */

#include <common.h>
#include <seco_imx8_env.h>

/* Video Mode List */
/* Aligne this list with the file include/seco/seco_imx8mq_c25_setup.h */
overlay_struct_mode_t overlay_video_mode_list [] = {

	{ "no video", "no dtbo" },
        { "HDMI", "seco-imx8mq-c25-hdmi.dtbo" },
        { "LVDS DCSS-1920x1080", "seco-imx8mq-c25-dcss-sn65dsi84.dtbo"},
        { "LVDS DCSS-800x480", "seco-imx8mq-c25-dcss-sn65dsi84-800x480.dtbo"},
        { "eDP  DCSS-1920x1080", "seco-imx8mq-c25-dcss-sn65dsi86.dtbo"},
        { "Display Port", "seco-imx8mq-c25-dp.dtbo" },
};

size_t overlay_video_size = sizeof( overlay_video_mode_list ) / sizeof( overlay_video_mode_list[0] );


/* Addons List */
overlay_struct_mode_t overlay_addons_list [] = {
	{ "No addons", "" },
        { "UART4 Support", "seco-imx8mq-c25-uart4.dtbo"},
        { "CAN RTC Support", "seco-imx8mq-c25-can-rtc.dtbo"},
        { "Q7 SPI Support", "seco-imx8mq-c25-q7-spi.dtbo"},
	{ "PCIe", "seco-imx8mq-c25-pcie.dtbo"},
};

size_t overlay_addons_size = sizeof( overlay_addons_list ) / sizeof( overlay_addons_list[0] );

/*
 *  Device Boot List  
 */

data_boot_dev_t kern_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_KERNEL_SRC_MMC,    BOOT_DEV_ID_EMMC,    LOAD_ADDR_KERNEL_LOCAL_DEV,   "Image" },
        { DEV_USB,	"uSD onboard",    ENV_KERNEL_SRC_USB,	 BOOT_DEV_ID_USB,     LOAD_ADDR_KERNEL_LOCAL_DEV,   "Image" },
        { DEV_TFTP,     "TFTP",           ENV_KERNEL_SRC_TFTP,   "",                  LOAD_ADDR_KERNEL_REMOTE_DEV,  "Image" },
        { DEV_USB,      "USB",            ENV_KERNEL_SRC_USB,    BOOT_DEV_ID_USB,     LOAD_ADDR_KERNEL_LOCAL_DEV,   "Image" },
};

size_t kern_dev_size = sizeof( kern_dev_list ) / sizeof( kern_dev_list[0] );

data_boot_dev_t fdt_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_FDT_SRC_MMC,     BOOT_DEV_ID_EMMC,    LOAD_ADDR_FDT_LOCAL_DEV,   CONFIG_DEFAULT_FDT_FILE },
        { DEV_USB,	"uSD onboard",    ENV_FDT_SRC_USB,     BOOT_DEV_ID_USB,     LOAD_ADDR_FDT_LOCAL_DEV,   CONFIG_DEFAULT_FDT_FILE },
        { DEV_TFTP,     "TFTP",           ENV_FDT_SRC_TFTP,    "",                  LOAD_ADDR_FDT_REMOTE_DEV,  CONFIG_DEFAULT_FDT_FILE },
        { DEV_USB,      "USB",            ENV_FDT_SRC_USB,     BOOT_DEV_ID_USB,     LOAD_ADDR_FDT_LOCAL_DEV,   CONFIG_DEFAULT_FDT_FILE },
};

size_t fdt_dev_size = sizeof( fdt_dev_list ) / sizeof( fdt_dev_list[0] );

data_boot_dev_t overlay_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_OVERLAY_SRC_MMC,     BOOT_DEV_ID_EMMC,    LOAD_ADDR_OVERLAY_LOCAL_DEV,   DEFAULT_OVERLAY_FILE },
        { DEV_USB,	"uSD onboard",    ENV_OVERLAY_SRC_USB,	   BOOT_DEV_ID_USB,	LOAD_ADDR_OVERLAY_LOCAL_DEV,   DEFAULT_OVERLAY_FILE },
        { DEV_TFTP,     "TFTP",           ENV_OVERLAY_SRC_TFTP,    "",                  LOAD_ADDR_OVERLAY_REMOTE_DEV,  DEFAULT_OVERLAY_FILE },
        { DEV_USB,      "USB",            ENV_OVERLAY_SRC_USB,     BOOT_DEV_ID_USB,     LOAD_ADDR_OVERLAY_LOCAL_DEV,   DEFAULT_OVERLAY_FILE },
};

size_t overlay_dev_size = sizeof( overlay_dev_list ) / sizeof( overlay_dev_list[0] );

data_boot_dev_t filesystem_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_FS_SRC_MMC,	ROOT_DEV_ID_EMMC,    "" },
        { DEV_USB,	"uSD onboard",    ENV_FS_SRC_USB,     "",  		   "" },
        { DEV_NFS,      "NFS",            ENV_FS_SRC_NFS,     "",                  "" },
        { DEV_USB,      "USB",            ENV_FS_SRC_USB,     "",                  "" },
};

size_t filesystem_dev_size = sizeof( filesystem_dev_list ) / sizeof( filesystem_dev_list[0] );
