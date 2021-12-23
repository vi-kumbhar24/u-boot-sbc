/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


#ifndef __MX8MQSECO_COMMON_CONFIG_H
#define __MX8MQSECO_COMMON_CONFIG_H


/* ____________________________________________________________________________
  |                                                                            |
  |                                   ENVIRONMENT                              |
  |____________________________________________________________________________|
*/

#define CONFIG_SYS_MMC_IMG_LOAD_PART   1

#define ROOT_DEV_ID		1
#define ROOT_PARTITION		2
#define FDT_LOADADDR	 	0x43000000
#define OVERLAY_LOADADDR_BASE	0x44000000
#define OVERLAY_LOADADDR_OFFSET 0x01000000
#define KERNEL_LOADADDR		0x40280000

#define FDT_RESIZE		0x60000

#define BOOTARGS_BASE  "setenv bootargs ${console_interface} ${memory} ${cpu_freq} ${videomode} ${options} ${root_dev}"

#define ENV_COMMON                                        \
	"bootdelay="__stringify(CONFIG_BOOTDELAY)"\0"            \
	"stdin=serial\0"                                         \
	"stdout=serial\0"                                        \
	"stderr=serial\0"			

#define ENV_BOOTARG_BASE                                                                               \
	"console_interface='console="CONSOLE_DEV "," __stringify(CONFIG_BAUDRATE)",earlycon=ec_imx6q,0x30890000,115200'\0"                \
	"bootargs_base="BOOTARGS_BASE"\0"	\
	"options=drm_kms_helper.edid_firmware=edid/1920x1080.bin\0"

#define ENV_KERNEL                                                                                                                   \
	"kernel_device_id="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0"                                                                          \
        "kernel_partition="__stringify(CONFIG_SYS_MMC_IMG_LOAD_PART)"\0"                                                                    \
        "kernel_loadaddr="__stringify(KERNEL_LOADADDR)"\0"                                                                                  \
	"kernel_file=Image\0"                                                                                                              \
	"kernel_load=mmc dev ${kernel_device_id}; fatload mmc ${kernel_device_id}:${kernel_partition} ${kernel_loadaddr} ${kernel_file}\0"


#define ENV_FDT                                                                                                  \
	"fdt_device_id="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0"                                                             \
        "fdt_partition="__stringify(CONFIG_SYS_MMC_IMG_LOAD_PART)"\0"                                                   \
	"fdt_loadaddr="__stringify(FDT_LOADADDR)"\0"                                                                 \
	"fdt_file=" DEFAULT_FDT_FILE "\0"                                                                            \
	"fdt_high=0xffffffff\0"	                                                                                            \
	"fdt_load=mmc dev ${fdt_device_id}; fatload mmc ${fdt_device_id}:${fdt_partition} ${fdt_loadaddr} ${fdt_file}\0"

#define ENV_OVERLAY       											\
	"overlay_device_id="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0"                                                             \
        "overlay_partition="__stringify(CONFIG_SYS_MMC_IMG_LOAD_PART)"\0"                                                   \
        "overlay_load=mmc dev ${overlay_device_id}; fatload mmc ${overlay_device_id}:${overlay_partition} ${overlay_loadaddr} ${overlay_file}\0"

#define ENV_ROOT                                                                               \
	"root_device_id="__stringify(ROOT_DEV_ID)"\0"                                              \
	"root_partition="__stringify(ROOT_PARTITION)"\0"                                           \
	"root_set_dev=setenv root_dev root=/dev/mmcblk${root_device_id}p${root_partition} rootwait rw\0"  \
	"root_add_env=setenv bootargs ${bootargs} ${root_dev}\0"


#define ENV_VIDEO                                          \
	"video_lvds1=video=mxcfb1:dev=ldb,LDB-WVGA,if=RGB666\0"       \
	"video_lvds2=video=mxcfb2:dev=ldb,LDB-WVGA,if=RGB666\0"       \
	"video_hdmi=video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24\0"   \
	"videomode=video=mxcfb0:dev=hdmi,1920x1080M@60\0"


#define CMD_SET_BOOT                                         \
	"run bootargs_base; "                                           \
	"if test \"${run_from_tftp}\" = \"1\"; then "                    \
		"run ipsetup;"  \
	"fi;"                                                           \
	"if test \"${run_from_nfs}\" = \"0\"; then "                    \
		"echo Running in Normal Mode... ; "                         \
	"else "                                                         \
		"echo Running in Remote... ; "                              \
		"run set_ip; "                                              \
	"fi;"                                                           \
	"run root_set_dev; "                                            \
	"run root_add_env; "                                            \
	"run kernel_load; "                                             \
	"run fdt_load; "                                                \
	"if test \"${run_overlay}\" = \"0\"; then "                    \
                "echo Running in Normal Mode... ; "                         \
        "else "                                                         \
                "echo Apply overlay... ; "                              \
                "run overlay_setup;"   \
        "fi;"				\
	"booti ${kernel_loadaddr} - ${fdt_loadaddr}"


#define CONFIG_EXTRA_ENV_SETTINGS           \
	ENV_COMMON                       \
	ENV_BOOTARG_BASE                 \
	ENV_KERNEL                       \
	ENV_FDT                          \
	ENV_OVERLAY 			\
	ENV_ROOT                        	\
	"bootcmd=" CMD_SET_BOOT "\0"

#endif /* SECOCONFIG COMMON */
