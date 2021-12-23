#ifndef __MX6SECO_ENVIRONMENT_H
#define __MX6SECO_ENVIRONMENT_H


/*
 *
 * Variable              Scope                 Description
 * --------------------------------------------------------------------------------------------------------
 *
 * kernel_device_id      eMMC/SD/              numeric id of device
 * kernel_partition      eMMC/SD/USB/SATA      numeric id of device's partition
 * kernel_file           eMMC/SD/USB/SATA      absolute path (folder+name) of file to load
 * kernel_load2ram       all                   u-boot command to exec in order to load file to memory
 *
 * fdt_autodetect        all                   automatic detect the corret dtb file name (cpu type based)
 * fdt_device_id         eMMC/SD/              numeric id of device
 * fdt_partition         eMMC/SD/USB/SATA      numeric id of device's partition
 * fdt_file              eMMC/SD/USB/SATA      absolute path (folder+name) of file to load
 * fdt_load              all                   u-boot command to exec in order to load file to memory
 *
 * ramfs_use             all                   0 = do not use ramfs ; 1 = use ramfs
 * ramfs_device_id       eMMC/SD/              numeric id of device
 * ramfs_partition       eMMC/SD/USB/SATA      numeric id of device's partition
 * ramfs_file            eMMC/SD/USB/SATA      absolute path (folder+name) of file to load
 * ramfs_load2ram        all                   u-boot command to exec in order to load file to memory
 *
 * root_device_id        eMMC/SD/              numeric id of device
 * root_partition        eMMC/SD/USB/SATA      numeric id of device's partition
 * root_mount            all                   u-boot command to generate bootargs about FS mount point
 *
 * ipaddr                TFTP                  target ip
 * netmask               TFTP                  target netmask
 * nfs_ip_client         NFS                   static IP of client
 * nfs_netmask           NFS                   netmask for client
 * nfs_ip_server         NFS                   static IP of server
 * nfs_path              NFS                   remote path of FS folder
 * eth_if                NFS                   id of the eth inteface to use for network Filesystem
 *
 */



#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)



/* ____________________________________________________________________________
  |                                                                            |
  |                                 ENV MACROS                                 |
  |____________________________________________________________________________|
*/

/* -------------------- BOOTSCRIPT -------------------- */
#define MACRO_ENV_BOOTSCRIPT_SRC_USDHCI  \
	load mmc ${bootscript_device_id}:${bootscript_partition} ${bootscript_loadaddr} ${script}

#define MACRO_ENV_BOOTENV_SRC_USDHCI \
	load mmc ${bootscript_device_id}:${bootenv_partition} ${bootenv_loadaddr} ${envtxt}


/* -------------------- KERNEL -------------------- */

#define MACRO_ENV_KERNEL_SRC_USDHCI     \
	load mmc ${kernel_device_id}:${kernel_partition}  ${kernel_loadaddr} ${kernel_file}

#define MACRO_ENV_KERNEL_SRC_USB     \
	usb start; load usb ${kernel_device_id}:${kernel_partition} ${kernel_loadaddr} ${kernel_file}

#define MACRO_ENV_KERNEL_SRC_SATA    \
	sata init; load sata 0:${kernel_partition} ${kernel_loadaddr} ${kernel_file}

#define MACRO_ENV_KERNEL_SRC_SPI     \
	sf probe 0; sf ${kernel_loadaddr} ${kernel_spi_addr} ${kernel_spi_len}

#define MACRO_ENV_KERNEL_SRC_TFTP   \
	tftpboot ${kernel_loadaddr} ${kernel_file}



/* -------------------- FDT -------------------- */
#define MACRO_ENV_FDT_SRC_USDHCI     \
	load mmc ${fdt_device_id}:${fdt_partition}  ${fdt_loadaddr} ${fdt_file}

#define MACRO_ENV_FDT_SRC_USB     \
	usb start; load usb ${fdt_device_id}:${fdt_partition} ${fdt_loadaddr} ${fdt_file}

#define MACRO_ENV_FDT_SRC_SATA    \
	sata init; load sata 0:${fdt_partition} ${fdt_loadaddr} ${fdt_file}

#define MACRO_ENV_FDT_SRC_SPI     \
	sf probe 0; sf ${fdt_loadaddr} ${fdt_spi_addr} ${fdt_spi_len}

#define MACRO_ENV_FDT_SRC_TFTP   \
	tftpboot ${fdt_loadaddr} ${fdt_file}

#ifdef CONFIG_OF_LIBFDT_OVERLAY
#define MACRO_ENV_FDT_LOAD_OVERLAY_BASE \
	fdt addr ${fdt_loadaddr}; fdt resize ${fdt_resize}; \
	seco_config fdtoverlay ; \
	if test -n ${fdt_overlay_per_cmd}; then run fdt_overlay_per_cmd; fi; \
	if test -n ${fdt_overlay_video_cmd}; then run fdt_overlay_video_cmd; fi;


#define MACRO_ENV_FDT_OVERLAY_SRC_USDHCI     \
	load mmc ${fdt_device_id}:${fdt_partition}

#define MACRO_ENV_FDT_OVERLAY_SRC_USB     \
	usb start; load usb ${fdt_device_id}:${fdt_partition}

#define MACRO_ENV_FDT_OVERLAY_SRC_SATA    \
	sata init; load sata 0:${fdt_partition}

#define MACRO_ENV_FDT_OVERLAY_SRC_SPI     \
	sf probe 0; sf ${fdt_loadaddr} ${fdt_spi_addr} ${fdt_spi_len}

#define MACRO_ENV_FDT_OVERLAY_SRC_TFTP   \
	tftpboot

#define MACRO_ENV_FDT_OVERLAY_APPLY   \
	fdt apply
#else
#define MACRO_ENV_FDT_LOAD_OVERLAY_BASE "\0"
#endif    /* CONFIG_OF_LIBFDT_OVERLAY */


/* -------------------- RAMFS -------------------- */

#define MACRO_ENV_RAMFS_SRC_USDHCI     \
	load mmc ${ramfs_device_id}:${ramfs_partition}  ${ramfs_loadaddr} ${ramfs_file}

#define MACRO_ENV_RAMFS_SRC_USB     \
	usb start; load usb ${ramfs_device_id}:${ramfs_partition} ${ramfs_loadaddr} ${ramfs_file}

#define MACRO_ENV_RAMFS_SRC_SATA    \
	sata init; load sata 0:${ramfs_partition} ${ramfs_loadaddr} ${ramfs_file}

#define MACRO_ENV_RAMFS_SRC_SPI     \
	sf probe 0; sf ${ramfs_loadaddr} ${ramfs_spi_addr} ${ramfs_spi_len}

#define MACRO_ENV_RAMFS_SRC_TFTP   \
	tftpboot ${ramfs_loadaddr} ${ramfs_file}



/* -------------------- FILESYSTEM -------------------- */

#define MACRO_ENV_FS_COMMON  rootwait rw

#define MACRO_ENV_FS_SRC_USDHCI   \
	setenv root_dev root=/dev/mmcblk${root_device_id}p${root_partition} rootwait rw

#define MACRO_ENV_FS_SRC_USB      \
	setenv root_dev setenv root_dev root=/dev/sda${root_partition} rootwait rw

#define MACRO_ENV_FS_SRC_SATA     \
	setenv root_dev setenv root=/dev/sda${root_partition} rootwait rw

#define MACRO_ENV_FS_SRC_NFS      \
	setenv root_dev root=/dev/nfs rw rootwait nfsroot=${nfs_ip_server}:/${nfs_path}/ ip=${nfs_ip_client}:${nfs_ip_server}::${nfs_netmask}::eth${eth_if}:off rootwait rw




/* ____________________________________________________________________________
  |                                                                            |
  |                                 ENVIRONMENT                                |
  |____________________________________________________________________________|
*/


#define ENV_MMCAUTODETECT                        "yes"
#define ENV_FDTAUTODETECT                        "yes"
#define ENV_MEMAUTODETECT                        "yes"


#define ENV_BOOTARGS_BASE  setenv bootargs ${console_interface} ${memory} ${cpu_freq} ${videomode} vt.global_cursor_default=0 ${root_dev}



#define ENV_COMMON                                                    \
	"bootdelay="__stringify(CONFIG_BOOTDELAY)"\0"                     \
	"stdin=serial\0"                                                  \
	"stdout=serial\0"                                                 \
	"stderr=serial\0"                                                 \
	"mmcautodetect="ENV_MMCAUTODETECT"\0"                             \
	"fdt_autodetect="ENV_FDTAUTODETECT"\0"                            \
	"mem_autodetect="ENV_MEMAUTODETECT"\0"                            \


#define ENV_ARGS_BASE                                                                          \
	"console_interface='console=" ENV_CONSOLE_DEV "," __stringify(CONFIG_BAUDRATE)"'\0"    \
	"bootargs_base="STR(ENV_BOOTARGS_BASE)"\0"



#define ENV_SYS_MMC_ENV_DEV                      0
#define ENV_SYS_MMC_IMG_LOAD_PART                1
#define ENV_LOADADDR                             CONFIG_LOADADDR

#define ENV_KERNEL                                                    \
	"kernel_device_id="__stringify(ENV_SYS_MMC_ENV_DEV)"\0"           \
	"kernel_partition="__stringify(ENV_SYS_MMC_IMG_LOAD_PART)"\0"     \
	"kernel_loadaddr="__stringify(ENV_LOADADDR)"\0"                   \
	"kernel_file=zImage\0"                                            \
	"kernel_load_usdhci2ram="STR(MACRO_ENV_KERNEL_SRC_USDHCI)"\0"     \
	"kernel_load_usb2ram="STR(MACRO_ENV_KERNEL_SRC_USB)"\0"           \
	"kernel_load_eth2ram="STR(MACRO_ENV_KERNEL_SRC_TFTP)"\0"          \
	"kernel_load2ram="STR(MACRO_ENV_KERNEL_SRC_USDHCI)"\0"



#define ENV_FDT_LOADADDR                         CONFIG_FDT_LOADADDR
#define ENV_DEFAULT_FDT_FILE                     CONFIG_FDT_FILE_NAME

#ifdef CONFIG_OF_LIBFDT_OVERLAY

#if (defined(CONFIG_MX6SX))
#define ENV_FDT_OVERLAY_BASEADDR                 0x8a000000
#endif

#if (defined(CONFIG_MX6Q)) || (defined(CONFIG_MX6DL)) || (defined(CONFIG_MX6QDL))
#define ENV_FDT_OVERLAY_BASEADDR                 0x1a000000
#endif


#define ENV_FDT_OVERLAY_BASEADDR_OFFSET          0x01000000

#endif  /* CONFIG_OF_LIBFDT_OVERLAY */

#define ENV_FDT_RESIZE                           0x4000

#ifndef CONFIG_OF_LIBFDT_OVERLAY

#define ENV_FDT                                                       \
	"fdt_device_id="__stringify(ENV_SYS_MMC_ENV_DEV)"\0"              \
	"fdt_partition="__stringify(ENV_SYS_MMC_IMG_LOAD_PART)"\0"        \
	"fdt_loadaddr="__stringify(ENV_FDT_LOADADDR)"\0"                  \
	"fdt_file="__stringify(ENV_DEFAULT_FDT_FILE)"\0"                  \
	"fdt_high=0xffffffff\0"	                                          \
	"fdt_load_usdhci2ram="STR(MACRO_ENV_FDT_SRC_USDHCI)"\0"           \
	"fdt_load_usb2ram="STR(MACRO_ENV_FDT_SRC_USB)"\0"                 \
	"fdt_load_eth2ram="STR(MACRO_ENV_FDT_SRC_TFTP)"\0"                \
	"fdt_load2ram="STR(MACRO_ENV_FDT_SRC_USDHCI)"\0"

#else

#define ENV_FDT                                                       \
	"fdt_device_id="__stringify(ENV_SYS_MMC_ENV_DEV)"\0"              \
	"fdt_partition="__stringify(ENV_SYS_MMC_IMG_LOAD_PART)"\0"        \
	"fdt_loadaddr="__stringify(ENV_FDT_LOADADDR)"\0"                  \
	"fdt_file="__stringify(ENV_DEFAULT_FDT_FILE)"\0"                  \
	"fdt_high=0xffffffff\0"	                                          \
	"fdt_resize="__stringify(ENV_FDT_RESIZE)"\0"                      \
	"fdt_load_usdhci2ram="STR(MACRO_ENV_FDT_SRC_USDHCI)"\0"           \
	"fdt_load_usb2ram="STR(MACRO_ENV_FDT_SRC_USB)"\0"                 \
	"fdt_load_eth2ram="STR(MACRO_ENV_FDT_SRC_TFTP)"\0"                \
	"fdt_load2ram="STR(MACRO_ENV_FDT_SRC_USDHCI)"\0"                  \
	"fdt_overlay_load2ram="STR(MACRO_ENV_FDT_LOAD_OVERLAY_BASE)"\0"

#endif  /* CONFIG_OF_LIBFDT_OVERLAY */



#if defined(CONFIG_MX6SL) || defined(CONFIG_MX6SLL) || \
	defined(CONFIG_MX6SX) || \
	defined(CONFIG_MX6UL) || defined(CONFIG_MX6ULL)
#define ENV_RAMFS_LOADADDR                       0x86800000
#else
#define ENV_RAMFS_LOADADDR                       0x12C00000
#endif



#define ENV_BOOTSCRIPT                                                        \
	"script=seco_boot.scr\0"                                                  \
	"envtxt=/loader/uEnv.txt\0"                                                       \
	"bootscript_device_id="__stringify(ENV_SYS_MMC_ENV_DEV)"\0"               \
	"bootscript_partition="__stringify(ENV_SYS_MMC_IMG_LOAD_PART)"\0"         \
	"bootenv_partition="__stringify(ENV_SYS_MMC_IMG_LOAD_PART)"\0"            \
	"bootscript_loadaddr="__stringify(ENV_LOADADDR)"\0"                       \
	"bootenv_loadaddr="__stringify(ENV_RAMFS_LOADADDR)"\0"                    \
	"bootscript_load_usdhci2ram="STR(MACRO_ENV_BOOTSCRIPT_SRC_USDHCI)"\0"     \
	"bootenv_load_usdhci2ram="STR(MACRO_ENV_BOOTENV_SRC_USDHCI)"\0"


#define ENV_RAMFS                                                     \
	"ramfs_use=0\0"                                                   \
	"ramfs_device_id="__stringify(ENV_SYS_MMC_ENV_DEV)"\0"            \
	"ramfs_partition="__stringify(ENV_SYS_MMC_IMG_LOAD_PART)"\0"      \
	"ramfs_loadaddr="__stringify(ENV_RAMFS_LOADADDR)"\0"              \
	"ramfs_file=ramfs.img\0"                                          \
	"ramfs_load_usdhci2ram="STR(MACRO_ENV_RAMFS_SRC_USDHCI)"\0"       \
	"ramfs_load_usb2ram="STR(MACRO_ENV_RAMFS_SRC_USB)"\0"             \
	"ramfs_load_eth2ram="STR(MACRO_ENV_RAMFS_SRC_TFTP)"\0"            \
	"ramfs_load2ram="STR(MACRO_ENV_RAMFS_SRC_USDHCI)"\0"



#define ENV_ROOT_DEV_ID                          0
#define ENV_ROOT_PARTITION                       2

#define ENV_ROOT                                                      \
	"root_device_id="__stringify(ENV_ROOT_DEV_ID)"\0"                 \
	"root_partition="__stringify(ENV_ROOT_PARTITION)"\0"              \
	"root_load_usdhci="STR(MACRO_ENV_FS_SRC_USDHCI)"\0"               \
	"root_load_usb="STR(MACRO_ENV_FS_SRC_USB)"\0"                     \
	"root_load_eth="STR(MACRO_ENV_FS_SRC_NFS)"\0"                     \
	"root_mount="STR(MACRO_ENV_FS_SRC_USDHCI)"\0"



#define CONFIG_SERVERIP                          13.0.0.1
#define CONFIG_IPADDR                            13.0.0.10
#define CONFIG_NETMASK                           255.255.255.0
#define DEF_NFS_PATH                             "targetfs/"

#define ENV_NETWORK                                                   \
	"ipaddr="__stringify(CONFIG_IPADDR)"\0"                           \
	"netmask=255.255.255.0\0"                                         \
	"eth_if=0\0"                                                      \
	"nfs_ip_client="__stringify(CONFIG_IPADDR)"\0"                    \
	"nfs_ip_server="__stringify(CONFIG_SERVERIP)"\0"                  \
	"nfs_netmask=255.255.255.0\0"                                     \
	"nfs_path="__stringify(DEF_NFS_PATH)"\0"




#define ENV_VIDEO                                                    \
	"videomode=video=mxcfb0:dev=ldb\0"                               \
	"splashpos=m,m\0"


#ifndef SECO_SYS_CONFIGURATION
#define ENV_DEFAULT_DISPLAY                      "panel=\0"
#define ENV_DEFAULT_VIDEO_MODE                   "videomode=video=mxcfb0:dev=ldb\0"
#define ENV_DEFAULT_VIDEO_SETING                 "fdt_overlay_video_list=\0"
#define ENV_DEFAULT_PERIPHERAL                   "fdt_overlay_per_list=\0"

#include "mx6_seco_def_setting.h"
#endif   /*  SECO_SYS_CONFIGURATION  */

#ifndef CONFIG_OF_LIBFDT_OVERLAY

#define ENV_CMD_SET_BOOT                                              \
	"mmc dev ${bootscript_device_id}; "                               \
	"if mmc rescan; then "                                            \
		"if run bootscript_load_usdhci2ram; then "                    \
			"echo ==> Running bootscript mode... ; "			      \
			" source ; "                                              \
		"fi; "                                                        \
	"fi; "                                                            \
	"echo ==> Running in Normal Mode... ; "							  \
	"run bootargs_base; "                                          	  \
	"run kernel_load2ram; "                                           \
	"run fdt_load2ram; "                                              \
	"run root_mount; "                                                \
	"run bootargs_base; "                                             \
	"if test ${ramfs_use} = 1; then "                                 \
		" run ramfs_load2ram; "                                       \
		"bootz ${kernel_loadaddr} ${ramfs_loadaddr} ${fdt_loadaddr}; "\
	"else "                                                           \
		" bootz ${kernel_loadaddr} - ${fdt_loadaddr}; "               \
	"fi\0"

#else

#define ENV_CMD_SET_BOOT                                              \
	"mmc dev ${bootscript_device_id}; "                               \
	"if mmc rescan; then "                                            \
		"if run bootscript_load_usdhci2ram; then "                    \
			"echo ==> Running bootscript mode... ; "			      \
			" source ; "                                              \
		"fi; "                                                        \
	"fi; "                                                            \
	"echo ==> Running in Normal Mode... ; "							  \
	"run bootargs_base; "                                          	  \
	"run kernel_load2ram; "                                           \
	"run fdt_load2ram; "                                              \
	"run fdt_overlay_load2ram; "                                      \
	"run root_mount; "                                                \
	"run bootargs_base; "                                             \
	"if test ${ramfs_use} = 1; then "                                 \
		" run ramfs_load2ram; "                                       \
		" bootz ${kernel_loadaddr} ${ramfs_loadaddr} ${fdt_loadaddr}; "\
	"else "                                                           \
	 " bootz ${kernel_loadaddr} - ${fdt_loadaddr}; "                  \
	"fi\0"

#endif  /* CONFIG_OF_LIBFDT_OVERLAY */

#define CONFIG_PREBOOT


#define ENV_PREBOOT "preboot=echo Start boot...\0"


#define CONFIG_EXTRA_ENV_SETTINGS                                     \
	ENV_COMMON                                                        \
	ENV_ARGS_BASE                                                     \
	ENV_NETWORK                                                       \
	ENV_BOOTSCRIPT                                                    \
	ENV_KERNEL                                                        \
	ENV_FDT                                                           \
	ENV_RAMFS                                                         \
	ENV_ROOT                                                          \
	ENV_VIDEO                                                         \
	ENV_PREBOOT                                                       \
	ENV_DEFAULT_DISPLAY                                               \
	ENV_DEFAULT_VIDEO_MODE                                            \
	ENV_DEFAULT_VIDEO_SETING                                          \
	ENV_DEFAULT_PERIPHERAL                                            \
	"bootcmd=" ENV_CMD_SET_BOOT "\0"



#endif   /*  __MX6SECO_ENVIRONMENT_H  */
