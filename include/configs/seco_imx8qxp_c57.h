/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018-2019 NXP
 */

#ifndef __SECO_IMX8QXP_C57_H
#define __SECO_IMX8QXP_C57_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>
#include "imx_env.h"
#include "seco_imx8qxp_common.h"

#ifdef CONFIG_SPL_BUILD
#define CONFIG_PARSE_CONTAINER
#define CONFIG_SPL_TEXT_BASE				0x100000
#define CONFIG_SPL_MAX_SIZE				(192 * 1024)
#define CONFIG_SYS_MONITOR_LEN				(1024 * 1024)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR		0x1040 /* (32K + 2Mb)/sector_size */
#define CONFIG_SYS_SPI_U_BOOT_OFFS 0x200000

/*
 * 0x08081000 - 0x08180FFF is for m4_0 xip image,
  * So 3rd container image may start from 0x8181000
 */
#define CONFIG_SYS_UBOOT_BASE 0x08181000
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION		0

#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
/*
 * The memory layout on stack:  DATA section save + gd + early malloc
 * the idea is re-use the early malloc (CONFIG_SYS_MALLOC_F_LEN) with
 * CONFIG_SYS_SPL_MALLOC_START
 */
#define CONFIG_SPL_STACK		0x013fff0
#define CONFIG_SPL_BSS_START_ADDR      0x00130000
#define CONFIG_SPL_BSS_MAX_SIZE		0x1000	/* 4 KB */
#define CONFIG_SYS_SPL_MALLOC_START	0x82200000
#define CONFIG_SYS_SPL_MALLOC_SIZE     0x80000	/* 512 KB */
#define CONFIG_SERIAL_LPUART_BASE	0x5a080000
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_MALLOC_F_ADDR		0x00138000

#define CONFIG_SPL_RAW_IMAGE_ARM_TRUSTED_FIRMWARE

#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#define CONFIG_OF_EMBED
#endif

#define CONFIG_REMAKE_ELF

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_ARCH_MISC_INIT

#define CONFIG_CMD_READ

/* Flat Device Tree Definitions */
#define CONFIG_OF_BOARD_SETUP

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV
#undef CONFIG_CMD_IMLS

#undef CONFIG_CMD_CRC32
#undef CONFIG_BOOTM_NETBSD

#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR       0
#define USDHC1_BASE_ADDR                0x5B010000
#define USDHC2_BASE_ADDR                0x5B020000
#define CONFIG_SUPPORT_EMMC_BOOT	/* eMMC specific */

#define CONFIG_ENV_OVERWRITE


#define CONFIG_PCIE_IMX
#define CONFIG_CMD_PCI
#define CONFIG_PCI_SCAN_SHOW

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

/* eMMC Configuration */
#define BOOT_ID_EMMC 0
#define ROOT_ID_EMMC 0
#define SECO_NUM_BOOT_DEV                        1



/* Link Definitions */
#define CONFIG_LOADADDR			0x80280000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_SYS_INIT_SP_ADDR         0x80200000

/* Default environment is in SD */
#define CONFIG_ENV_SIZE			0x2000
#ifdef CONFIG_QSPI_BOOT
#define CONFIG_ENV_OFFSET       (4 * 1024 * 1024)
#define CONFIG_ENV_SECT_SIZE	(128 * 1024)
#define CONFIG_ENV_SPI_BUS	CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS	CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE	CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ	CONFIG_SF_DEFAULT_SPEED
#else
#define CONFIG_ENV_OFFSET       (64 * SZ_64K)
#define CONFIG_SYS_MMC_ENV_PART		0	/* user area */
#endif

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

/* On LPDDR4 board, USDHC1 is for eMMC, USDHC2 is for SD on CPU board */
#define CONFIG_SYS_MMC_ENV_DEV		0   /* emmc0 */
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"  /* emmc0p2 */
#define CONFIG_SYS_FSL_USDHC_NUM	1

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		((CONFIG_ENV_SIZE + (32 * 1024)) * 1024)

#define CONFIG_SYS_SDRAM_BASE		0x80000000

#define PHYS_SDRAM_1			0x80000000
#define PHYS_SDRAM_2			0x880000000

#define PHYS_SDRAM_1_SIZE		0x40000000	/* 1 GB */
#define PHYS_SDRAM_2_SIZE		0x0

#ifdef	CONFIG_SECO_IMX8QXP_C57_2GB
	#define PHYS_SDRAM_2_SIZE		0x0		/* 2 GB */
#elif	CONFIG_SECO_IMX8QXP_C57_3GB
	#define PHYS_SDRAM_2_SIZE		0x40000000	/* 3 GB */
#endif

#define PHYS_DRAM_IS_1GB                0x40000000
#define PHYS_DRAM_IS_2GB                0x80000000
#define PHYS_DRAM_IS_3GB                0xc0000000
#define PHYS_DRAM_IS_4GB                0x100000000



#ifdef CONFIG_SECO_IMX8QXP_C57_2GB
#define PHYS_DRAM_IS_2GB                0x80000000
#elif  CONFIG_SECO_IMX8QXP_C57_3GB
#define PHYS_DRAM_IS_3GB                0xc0000000
#endif

#define CONFIG_SYS_MEMTEST_START    0xA0000000
#define CONFIG_SYS_MEMTEST_END      (CONFIG_SYS_MEMTEST_START + (PHYS_SDRAM_1_SIZE >> 2))

/* Serial */
#define CONSOLE_DEV                     "ttyLP2"
#define DEBUG_LPUART_BASE_ADDR   0x5a080000
#define CONFIG_BAUDRATE			115200

/*Device tree env*/
#define DEFAULT_FDT_FILE                "seco-imx8qxp-c57.dtb"
#define DEFAULT_OVERLAY_FILE            "seco-imx8qxp-c57-sn65dsi86-edp.dtbo"
#define DEFAULT_HDPTX_FILE                "hdmitxfw.bin"
#define DEFAULT_HDPRX_FILE                "hdmirxfw.bin"


/* Monitor Command Prompt */
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_SYS_CBSIZE              2048
#define CONFIG_SYS_MAXARGS             64
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		8000000	/* 8MHz */

#ifndef CONFIG_DM_PCA953X
#define CONFIG_PCA953X
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO
#endif

/* MT35XU512ABA1G12 has only one Die, so QSPI0 B won't work */
#ifdef CONFIG_FSL_FSPI
#define FSL_FSPI_FLASH_SIZE		SZ_64M
#define FSL_FSPI_FLASH_NUM		1
#define FSPI0_BASE_ADDR			0x5d120000
#define FSPI0_AMBA_BASE			0
#define CONFIG_SYS_FSL_FSPI_AHB
#endif

#define CONFIG_SERIAL_TAG

/* USB Config */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#define CONFIG_USBD_HS

#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE

#endif

#define CONFIG_USB_MAX_CONTROLLER_COUNT 2

/* USB OTG controller configs */
#ifdef CONFIG_USB_EHCI_HCD
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#endif

/* Networking */
#define CONFIG_FEC_ENET_DEV 0

#if (CONFIG_FEC_ENET_DEV == 0)
#define IMX_FEC_BASE			0x5B040000
#define CONFIG_FEC_MXC_PHYADDR          0x9
#define CONFIG_ETHPRIME                 "eth0"
#elif (CONFIG_FEC_ENET_DEV == 1)
#define IMX_FEC_BASE			0x5B050000
#define CONFIG_FEC_MXC_PHYADDR          0x8
#define CONFIG_ETHPRIME                 "eth1"
#endif

#define CONFIG_FEC_XCV_TYPE		RGMII
#define FEC_QUIRK_ENET_MAC

#ifndef CONFIG_LIB_RAND
#define CONFIG_LIB_RAND
#endif
#define CONFIG_NET_RANDOM_ETHADDR

/* Framebuffer */
#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_IMXDPUV1
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#endif

#define CONFIG_OF_SYSTEM_SETUP

#if defined(CONFIG_ANDROID_SUPPORT)
#include "imx8qxp_mek_android.h"
#elif defined (CONFIG_ANDROID_AUTO_SUPPORT)
#include "imx8qxp_mek_android_auto.h"
#endif

#endif /* __SECO_IMX8QXP_C57_H */
