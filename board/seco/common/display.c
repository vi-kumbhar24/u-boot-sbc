/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/crm_regs.h>
#include <asm/gpio.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/mach-imx/boot_mode.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mxc_hdmi.h>
#include <asm/mach-imx/video.h>
#include <linux/fb.h>
#include <ipu_pixfmt.h>
#include <micrel.h>
#include <i2c.h>

#include "proto_seco.h"
#include "muxing.h"




#if defined(CONFIG_VIDEO_IPUV3) || defined(CONFIG_VIDEO_MXS)



#ifdef SECO_COMMON_HDMI
static void enable_hdmi (struct display_info_t const *dev) {
	disable_lvds(dev);
	imx_enable_hdmi_phy ();
}
#endif


struct display_info_t const displays[] = {
#ifdef SECO_COMMON_RGB
	{
#if defined(CONFIG_VIDEO_MXS)
		.bus	= MX6SX_LCDIF1_BASE_ADDR,
		.pixfmt	= 24,
#else
		.bus	= -1,
		.pixfmt	= IPU_PIX_FMT_RGB24,
#endif
		.addr	= -1,
		.detect	= NULL,
		.enable	= enable_rgb,
		.mode	= {
			.name           = "CLAA-WVGA",
			.refresh        = 57,
			.xres           = 800,
			.yres           = 480,
			.pixclock       = 37037,
			.left_margin    = 35,
			.right_margin   = 35,
			.upper_margin   = 30,
			.lower_margin   = 30,
			.hsync_len      = 10,
			.vsync_len      = 10,
			.sync           = 0,
			.vmode          = FB_VMODE_NONINTERLACED
		}
	},
#endif
#ifdef SECO_COMMON_LVDS
   	{
#if defined(CONFIG_VIDEO_MXS)
		.bus	= LCDIF2_BASE_ADDR,
		.pixfmt	= 18,
#else
		.bus	= -1,
		.pixfmt = IPU_PIX_FMT_LVDS666,
#endif
		.addr	= -1,
		.detect = NULL,
		.enable = enable_lvds,
		.mode   = {
			// Rif. 800x480 Panel UMSH-8596MD-20T
			.name           = "LDB-WVGA",
			.refresh        = 60,
			.xres           = 800,
			.yres           = 480,
			.pixclock       = 30066,
			.left_margin    = 155,
			.right_margin   = 100,
			.upper_margin   = 22,
			.lower_margin   = 22,
			.hsync_len      = 1,
			.vsync_len      = 1,
			.sync           = 0,
			.vmode          = FB_VMODE_NONINTERLACED
		} 
	}, {

		.bus    = -1,
		.addr   = -1,
		.pixfmt = IPU_PIX_FMT_RGB666,
		.detect = NULL,
		.enable = enable_lvds,
		.mode   = {
			// Rif. Panel 1024x768 Panel UMSH-8596MD-15T
			.name           = "LDB-XGA",
			.refresh        = 60,
			.xres           = 1024,
			.yres           = 768,
			.pixclock       = 15385,
			.left_margin    = 220,
			.right_margin   = 40,
			.upper_margin   = 21,
			.lower_margin   = 7,
			.hsync_len      = 60,
			.vsync_len      = 10,
			.sync           = 0,
			.vmode          = FB_VMODE_NONINTERLACED
		}
	}, {
		.bus    = -1,
		.addr   = -1,
		.pixfmt = IPU_PIX_FMT_RGB666,
		.detect = NULL,
		.enable = enable_lvds,
		.mode   = {
			// Rif. 1366x768 Panel CHIMEI M156B3-LA1
			.name           = "LDB-WXGA",
			.refresh        = 59,
			.xres           = 1368,
			.yres           = 768,
			.pixclock       = 13890,
			.left_margin    = 93,
			.right_margin   = 33,
			.upper_margin   = 22,
			.lower_margin   = 7,
			.hsync_len      = 40,
			.vsync_len      = 4,
			.sync           = 0,
			.vmode          = FB_VMODE_NONINTERLACED
		}
	}, {

		.bus    = -1,
		.addr   = -1,
		.pixfmt = IPU_PIX_FMT_RGB24,
		.detect = NULL,
		.enable = enable_lvds,
		.mode   = {
			// Rif. Panel 1024x768 Panel UMSH-8596MD-15T
			.name           = "LDB-1280P60",
			.refresh        = 60,
			.xres           = 1280,
			.yres           = 800,
			.pixclock       = 15128,
			.left_margin    = 220,
			.right_margin   = 40,
			.upper_margin   = 21,
			.lower_margin   = 7,
			.hsync_len      = 60,
			.vsync_len      = 10,
			.sync           = 0,
			.vmode          = FB_VMODE_NONINTERLACED
		}
	},
#endif
#ifdef SECO_COMMON_HDMI
	{
		.bus    = -1,
		.addr   = 0,
		.pixfmt = IPU_PIX_FMT_RGB24,
		.detect = detect_hdmi,
		.enable = enable_hdmi,
		.mode   = {
        	.name           = "HDMI",
        	.refresh        = 60,
        	.xres           = 1024,
        	.yres           = 768,
        	.pixclock       = 15385,
        	.left_margin    = 220,
        	.right_margin   = 40,
        	.upper_margin   = 21,
        	.lower_margin   = 7,
        	.hsync_len      = 60,
        	.vsync_len      = 10,
        	.sync           = FB_SYNC_EXT,
        	.vmode          = FB_VMODE_NONINTERLACED
		}
	},
#endif
};
size_t display_count = ARRAY_SIZE(displays);


#define LVDS_DIVIDER        7
#define OSC_FREQ            24000000 //Hz
#define VIDEO_MAX_DIV       54000
#define VIDEO_MIN_DIV       27000


static const int post_div[] = { 4, 2, 1 };

void setup_lvds ( struct display_info_t const *dev ) {
	int i, p_div, p_div_sel;
	unsigned long lvds_freq = PICOS2KHZ(dev->mode.pixclock) * 1000UL;
	unsigned long lvds_input_freq = lvds_freq * LVDS_DIVIDER;
	unsigned long target_freq = lvds_input_freq / (OSC_FREQ / 1000);  // in mHz
	unsigned long pll_freq, pll_div_sel, fact, num, denom;
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	int reg;

	/* find POST_DIV divider */
	for ( i = 0 ; i < ARRAY_SIZE(post_div) ; i++ ) {
		p_div = post_div[i];
		pll_freq = target_freq * p_div;
		if ( (pll_freq >= VIDEO_MIN_DIV) && (pll_freq <= (VIDEO_MAX_DIV + 1)) ) {
			break;
		}
	}
	if ( i == ARRAY_SIZE(post_div) ) {
		// error: frequency not found
		printf( "LVDS frequency not setted!!!\n" );
		return;
	}
	p_div_sel = i;
	p_div = post_div[p_div_sel];

	pll_div_sel = pll_freq / 1000;
	fact = pll_freq - pll_div_sel * 1000;

	num = fact * 1000;
	denom = 1000 * 1000;

/*  set PLL5 clock */
	reg = readl(&mxc_ccm->analog_pll_video);
	reg |= BM_ANADIG_PLL_VIDEO_POWERDOWN;
	writel(reg, &mxc_ccm->analog_pll_video);

	/*  set PLL5 to 232720000Hz */
	reg &= ~BM_ANADIG_PLL_VIDEO_DIV_SELECT;
	reg |= BF_ANADIG_PLL_VIDEO_DIV_SELECT(pll_div_sel);
	reg &= ~BM_ANADIG_PLL_VIDEO_POST_DIV_SELECT;
	reg |= BF_ANADIG_PLL_VIDEO_POST_DIV_SELECT(p_div_sel);
	writel(reg, &mxc_ccm->analog_pll_video);

	writel(BF_ANADIG_PLL_VIDEO_NUM_A(num),
			&mxc_ccm->analog_pll_video_num);
	writel(BF_ANADIG_PLL_VIDEO_DENOM_B(denom),
			&mxc_ccm->analog_pll_video_denom);

	reg &= ~BM_ANADIG_PLL_VIDEO_POWERDOWN;
	writel(reg, &mxc_ccm->analog_pll_video);
}


#if defined(CONFIG_VIDEO_MXS)
void setup_display_sx (u32 lcd_base_addr) {
	struct mxc_ccm_reg *imx_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	u32 reg = 0;
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	if (is_cpu_type(MXC_CPU_MX6SX)) {
		if ((lcd_base_addr != LCDIF1_BASE_ADDR) &&
		    (lcd_base_addr != LCDIF2_BASE_ADDR)) {
			puts("Wrong LCD interface!\n");
			return;
		}
	} else {
		debug("This chip not support lvds bridge!\n");
		return;
	}

	/* Turn on LDB DI0 clocks */
	reg = readl(&imx_ccm->CCGR3);
	reg |=  MXC_CCM_CCGR3_LDB_DI0_MASK;
	writel(reg, &imx_ccm->CCGR3);

	/*  set PLL5 clock */
	reg = readl(&imx_ccm->analog_pll_video);
	reg |= BM_ANADIG_PLL_VIDEO_POWERDOWN;
	writel(reg, &imx_ccm->analog_pll_video);

	/*  set PLL5 to 232720000Hz */
	reg &= ~BM_ANADIG_PLL_VIDEO_DIV_SELECT;
	reg |= BF_ANADIG_PLL_VIDEO_DIV_SELECT(0x26);
	reg &= ~BM_ANADIG_PLL_VIDEO_POST_DIV_SELECT;
	reg |= BF_ANADIG_PLL_VIDEO_POST_DIV_SELECT(0);
	writel(reg, &imx_ccm->analog_pll_video);

	writel(BF_ANADIG_PLL_VIDEO_NUM_A(0xC0238),
			&imx_ccm->analog_pll_video_num);
	writel(BF_ANADIG_PLL_VIDEO_DENOM_B(0xF4240),
			&imx_ccm->analog_pll_video_denom);

	reg &= ~BM_ANADIG_PLL_VIDEO_POWERDOWN;
	writel(reg, &imx_ccm->analog_pll_video);

	s32 timeout = 100000;
	while (timeout--)
		if (readl(&imx_ccm->analog_pll_video) & BM_ANADIG_PLL_VIDEO_LOCK)
			break;

	if (timeout < 0)
		printf("Warning: video pll lock timeout!\n");

	reg = readl(&imx_ccm->analog_pll_video);
	reg |= BM_ANADIG_PLL_VIDEO_ENABLE;
	reg &= ~BM_ANADIG_PLL_VIDEO_BYPASS;
	writel(reg, &imx_ccm->analog_pll_video);


		/*  set LDB0, LDB1 clk select to 000/000 (PLL5 clock) */
	reg = readl(&imx_ccm->cs2cdr);
	reg &= ~(MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_MASK
		 | MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_MASK);
	reg |= (0 << MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_OFFSET)
	      | (0 << MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_OFFSET);
	writel(reg, &imx_ccm->cs2cdr);

	reg = readl(&imx_ccm->cscmr2);
	reg |= MXC_CCM_CSCMR2_LDB_DI0_IPU_DIV | MXC_CCM_CSCMR2_LDB_DI1_IPU_DIV;
	writel(reg, &imx_ccm->cscmr2);

	/* set LDB DI0 clock for LCDIF PIX clock */
	reg = readl(&imx_ccm->cscdr2);
	if (lcd_base_addr == LCDIF1_BASE_ADDR) {
		reg &= ~MXC_CCM_CSCDR2_LCDIF1_CLK_SEL_MASK;
		reg |= (0x3 << MXC_CCM_CSCDR2_LCDIF1_CLK_SEL_OFFSET);
	} else {
		reg &= ~MXC_CCM_CSCDR2_LCDIF2_CLK_SEL_MASK;
		reg |= (0x3 << MXC_CCM_CSCDR2_LCDIF2_CLK_SEL_OFFSET);
	}
	writel(reg, &imx_ccm->cscdr2);

	reg = IOMUXC_GPR2_DI0_VS_POLARITY_ACTIVE_LOW
		| IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG
		| IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT
		| IOMUXC_GPR2_LVDS_CH0_MODE_ENABLED_DI0;
	writel(reg, &iomux->gpr[6]);

	reg = readl(&iomux->gpr[5]);
	if (lcd_base_addr == LCDIF1_BASE_ADDR)
		reg &= ~0x8;  /* MUX LVDS to LCDIF1 */
	else
		reg |= 0x8; /* MUX LVDS to LCDIF2 */
	writel(reg, &iomux->gpr[5]);


	reg = IOMUXC_GPR2_BGREF_RRMODE_EXTERNAL_RES
	     | IOMUXC_GPR2_DI1_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_DI0_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_BIT_MAPPING_CH1_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT
	     | IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT
	     | IOMUXC_GPR2_LVDS_CH0_MODE_ENABLED_DI0
	     | IOMUXC_GPR2_LVDS_CH1_MODE_ENABLED_DI0;
	writel(reg, &iomux->gpr[2]);

	reg = readl(&iomux->gpr[3]);
	reg = (reg & ~(IOMUXC_GPR3_LVDS1_MUX_CTL_MASK
			| IOMUXC_GPR3_HDMI_MUX_CTL_MASK))
	    | (IOMUXC_GPR3_MUX_SRC_IPU1_DI0
	       << IOMUXC_GPR3_LVDS1_MUX_CTL_OFFSET);
	writel(reg, &iomux->gpr[3]);
}
#endif /* CONFIG_VIDEO_MXS */


void setup_display (void) {

 	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
 	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;
 	int reg;

 	enable_ipu_clock();
 #ifdef SECO_COMMON_HDMI
 	imx_setup_hdmi();
#endif

 	/* Turn on LDB0, LDB1, IPU,IPU DI0 clocks */
 	reg = readl(&mxc_ccm->CCGR3);
 	reg |=  MXC_CCM_CCGR3_LDB_DI0_MASK | MXC_CCM_CCGR3_LDB_DI1_MASK;
 	writel(reg, &mxc_ccm->CCGR3);

 	/*  set PLL5 clock */
 	reg = readl(&mxc_ccm->analog_pll_video);
 	reg |= BM_ANADIG_PLL_VIDEO_POWERDOWN;
 	writel(reg, &mxc_ccm->analog_pll_video);

 	/*  set PLL5 to 232720000Hz */
 	reg &= ~BM_ANADIG_PLL_VIDEO_DIV_SELECT;
 	reg |= BF_ANADIG_PLL_VIDEO_DIV_SELECT(0x26);
 	reg &= ~BM_ANADIG_PLL_VIDEO_POST_DIV_SELECT;
 	reg |= BF_ANADIG_PLL_VIDEO_POST_DIV_SELECT(0);
 	writel(reg, &mxc_ccm->analog_pll_video);

 	writel(BF_ANADIG_PLL_VIDEO_NUM_A(0xC0238),
 			&mxc_ccm->analog_pll_video_num);
	writel(BF_ANADIG_PLL_VIDEO_DENOM_B(0xF4240),
 			&mxc_ccm->analog_pll_video_denom);

 	reg &= ~BM_ANADIG_PLL_VIDEO_POWERDOWN;
 	writel(reg, &mxc_ccm->analog_pll_video);

 	s32 timeout = 100000;
 	while (timeout--)
 		if (readl(&mxc_ccm->analog_pll_video) & BM_ANADIG_PLL_VIDEO_LOCK)
 			break;

 	if (timeout < 0)
 		printf("Warning: video pll lock timeout!\n");

 	reg = readl(&mxc_ccm->analog_pll_video);
 	reg |= BM_ANADIG_PLL_VIDEO_ENABLE;
 	reg &= ~BM_ANADIG_PLL_VIDEO_BYPASS;
 	writel(reg, &mxc_ccm->analog_pll_video);


 	/*  set LDB0, LDB1 clk select to 000/000 (PLL5 clock) */
 	reg = readl(&mxc_ccm->cs2cdr);
 	reg &= ~(MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_MASK
		 | MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_MASK);
 	reg |= (0 << MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_OFFSET)
 	      | (0 << MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_OFFSET);
 	writel(reg, &mxc_ccm->cs2cdr);

 	reg = readl(&mxc_ccm->cscmr2);
	reg |= MXC_CCM_CSCMR2_LDB_DI0_IPU_DIV | MXC_CCM_CSCMR2_LDB_DI1_IPU_DIV;
 	writel(reg, &mxc_ccm->cscmr2);

 	reg = readl(&mxc_ccm->chsccdr);
 	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
 		<< MXC_CCM_CHSCCDR_IPU1_DI0_CLK_SEL_OFFSET);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
 		<< MXC_CCM_CHSCCDR_IPU1_DI1_CLK_SEL_OFFSET);
 	writel(reg, &mxc_ccm->chsccdr);

 	reg = IOMUXC_GPR2_BGREF_RRMODE_EXTERNAL_RES
 	     | IOMUXC_GPR2_DI1_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_DI0_VS_POLARITY_ACTIVE_LOW
 	     | IOMUXC_GPR2_BIT_MAPPING_CH1_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT
 	     | IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG
 	     | IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT
 	     | IOMUXC_GPR2_LVDS_CH0_MODE_ENABLED_DI0
 	     | IOMUXC_GPR2_LVDS_CH1_MODE_ENABLED_DI0;
 	writel(reg, &iomux->gpr[2]);

 	reg = readl(&iomux->gpr[3]);
 	reg = (reg & ~(IOMUXC_GPR3_LVDS1_MUX_CTL_MASK
 			| IOMUXC_GPR3_HDMI_MUX_CTL_MASK))
	    | (IOMUXC_GPR3_MUX_SRC_IPU1_DI0
 	       << IOMUXC_GPR3_LVDS1_MUX_CTL_OFFSET);
 	writel(reg, &iomux->gpr[3]);
 }



#endif /* CONFIG_VIDEO_IPUV3 || CONFIG_VIDEO_MXS */


