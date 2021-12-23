/*
 * 
 * Copyright SECO S.p.A. 
 * Mantained by marco.sandrelli@seco.com
 *
 */

#include <common.h>
#include <seco_imx8_env.h>

/* Video Mode List */
/* Aligne this list with the file include/seco/seco_imx8mm_c72_setup.h */
overlay_struct_mode_t overlay_video_mode_list [] = {
	{ "No video", "no dtbo" },
        { "eDP LCDIF-1920x1080", "seco-imx8mm-c72-lcdif-sn65dsi86.dtbo"},
        { "LVDS LCDIF-1920x1080", "seco-imx8mm-c72-lcdif-sn65dsi84.dtbo"},
};

size_t overlay_video_size = sizeof( overlay_video_mode_list ) / sizeof( overlay_video_mode_list[0] );


/* Addons List */
overlay_struct_mode_t overlay_addons_list [] = {
        { "No addons", "" },
};

size_t overlay_addons_size = sizeof( overlay_addons_list ) / sizeof( overlay_addons_list[0] );

/*
 *  Device Boot List  
 */

data_boot_dev_t kern_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_KERNEL_SRC_MMC,    BOOT_DEV_ID_EMMC,    LOAD_ADDR_KERNEL_LOCAL_DEV,   "Image" },
        { DEV_U_SD,     "uSD onboard",    ENV_KERNEL_SRC_USD,    BOOT_DEV_ID_U_SD,    LOAD_ADDR_KERNEL_LOCAL_DEV,   "Image" },
        { DEV_TFTP,     "TFTP",           ENV_KERNEL_SRC_TFTP,   "",                  LOAD_ADDR_KERNEL_REMOTE_DEV,  "Image" },
        { DEV_USB,      "USB",            ENV_KERNEL_SRC_USB,    BOOT_DEV_ID_USB,     LOAD_ADDR_KERNEL_LOCAL_DEV,   "Image" },
};

size_t kern_dev_size = sizeof( kern_dev_list ) / sizeof( kern_dev_list[0] );

data_boot_dev_t hdprx_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_HDPRX_SRC_MMC,    BOOT_DEV_ID_EMMC,    LOAD_ADDR_HDPRX_LOCAL_DEV,   "hdmirxfw.bin" },
        { DEV_U_SD,     "uSD onboard",    ENV_HDPRX_SRC_USD,    BOOT_DEV_ID_U_SD,    LOAD_ADDR_HDPRX_LOCAL_DEV,   "hdmirxfw.bin" },
        { DEV_TFTP,     "TFTP",           ENV_HDPRX_SRC_TFTP,   "",                  LOAD_ADDR_HDPRX_REMOTE_DEV,  "hdmirxfw.bin" },
        { DEV_USB,      "USB",            ENV_HDPRX_SRC_USB,    BOOT_DEV_ID_USB,     LOAD_ADDR_HDPRX_LOCAL_DEV,   "hdmirxfw.bin" },
};

size_t hdprx_dev_size = sizeof( hdprx_dev_list ) / sizeof( hdprx_dev_list[0] );

data_boot_dev_t hdptx_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_HDPTX_SRC_MMC,    BOOT_DEV_ID_EMMC,    LOAD_ADDR_HDPTX_LOCAL_DEV,   "hdmitxfw.bin" },
        { DEV_U_SD,     "uSD onboard",    ENV_HDPTX_SRC_USD,    BOOT_DEV_ID_U_SD,    LOAD_ADDR_HDPTX_LOCAL_DEV,   "hdmitxfw.bin" },
        { DEV_TFTP,     "TFTP",           ENV_HDPTX_SRC_TFTP,   "",                  LOAD_ADDR_HDPTX_REMOTE_DEV,  "hdmitxfw.bin" },
        { DEV_USB,      "USB",            ENV_HDPTX_SRC_USB,    BOOT_DEV_ID_USB,     LOAD_ADDR_HDPTX_LOCAL_DEV,   "hdmitxfw.bin" },
};

size_t hdptx_dev_size = sizeof( hdptx_dev_list ) / sizeof( hdptx_dev_list[0] );

data_boot_dev_t fdt_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_FDT_SRC_MMC,     BOOT_DEV_ID_EMMC,    LOAD_ADDR_FDT_LOCAL_DEV,   CONFIG_DEFAULT_FDT_FILE },
        { DEV_U_SD,     "uSD onboard",    ENV_FDT_SRC_USD,     BOOT_DEV_ID_U_SD,    LOAD_ADDR_FDT_LOCAL_DEV,   CONFIG_DEFAULT_FDT_FILE },
        { DEV_TFTP,     "TFTP",           ENV_FDT_SRC_TFTP,    "",                  LOAD_ADDR_FDT_REMOTE_DEV,  CONFIG_DEFAULT_FDT_FILE },
        { DEV_USB,      "USB",            ENV_FDT_SRC_USB,     BOOT_DEV_ID_USB,     LOAD_ADDR_FDT_LOCAL_DEV,   CONFIG_DEFAULT_FDT_FILE },
};

size_t fdt_dev_size = sizeof( fdt_dev_list ) / sizeof( fdt_dev_list[0] );

data_boot_dev_t overlay_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_OVERLAY_SRC_MMC,     BOOT_DEV_ID_EMMC,    LOAD_ADDR_OVERLAY_LOCAL_DEV,   DEFAULT_OVERLAY_FILE },
        { DEV_U_SD,     "uSD onboard",    ENV_OVERLAY_SRC_USD,     BOOT_DEV_ID_U_SD,    LOAD_ADDR_OVERLAY_LOCAL_DEV,   DEFAULT_OVERLAY_FILE },
        { DEV_TFTP,     "TFTP",           ENV_OVERLAY_SRC_TFTP,    "",                  LOAD_ADDR_OVERLAY_REMOTE_DEV,  DEFAULT_OVERLAY_FILE },
        { DEV_USB,      "USB",            ENV_OVERLAY_SRC_USB,     BOOT_DEV_ID_USB,     LOAD_ADDR_OVERLAY_LOCAL_DEV,   DEFAULT_OVERLAY_FILE },
};

size_t overlay_dev_size = sizeof( overlay_dev_list ) / sizeof( overlay_dev_list[0] );

data_boot_dev_t filesystem_dev_list [] = {
        { DEV_EMMC,     "eMMC onboard",   ENV_FS_SRC_MMC,     ROOT_DEV_ID_EMMC,    "" },
        { DEV_U_SD,     "uSD onboard",    ENV_FS_SRC_USD,     ROOT_DEV_ID_U_SD,    "" },
        { DEV_NFS,      "NFS",            ENV_FS_SRC_NFS,     "",                  "" },
        { DEV_USB,      "USB",            ENV_FS_SRC_USB,     "",                  "" },
};

size_t filesystem_dev_size = sizeof( filesystem_dev_list ) / sizeof( filesystem_dev_list[0] );
