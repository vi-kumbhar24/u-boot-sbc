/*
 * 
 *
 * Reading C61 Board type
 *
 * marco.sandrelli@seco.com
 *
 *
 *
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/imx8mm_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <seco/seco_imx8mm_c61_setup.h>

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE )

/* ____________________________________________________________________________
  |                                                                            |
  |                                  C61 REVISION                              |
  |____________________________________________________________________________|
*/


#define C61_IOMUX_REG(x)        ((x))
#define C61_PADCTRL_REG(x)      ((x))
#define C61_GDIR_REG_IN(x,n)    writel((readl(x + 0x4)) & ~(1<<n), x + 0x4 )
#define C61_PSR_REG(x,n)	(readl(x + 0x8) & (1<<n))

#define shift_0			0
#define shift_1			1
#define shift_2			2
#define shift_3			3

#define GPIO4_PAD_BASE		0x30230000		
/* RAM straps */
#define GPIO_CFG_0		0
#define GPIO_CFG_1              1
/* HDMI straps */
#define GPIO_CFG_2              17
/* Wilink Strap */
#define GPIO_CFG_3		13

#define GPIO1_PAD_BASE          0x30200000
#define GPIO4_PAD_BASE          0x30230000
#define GPIO_CFG_4              15
#define GPIO_CFG_5              22
#define GPIO_CFG_6              21
#define GPIO_CFG_7              0

/* Quad - Quad Lite Strap */
#define GPIO_CFG_8              5
/* SPI - CAN RTC Strap */
#define GPIO_CFG_9              29

struct sizes {
	u32 s0;
	u32 s1;
};

DECLARE_GLOBAL_DATA_PTR;

iomux_v3_cfg_t const board_conf_pads[] = {

        /* RAM CONFIG */
        IMX8MM_PAD_SAI1_RXFS_GPIO4_IO0	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),   /* cfg_0 */
        IMX8MM_PAD_SAI1_RXC_GPIO4_IO1	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),   /* cfg_1 */

};


int c61_get_board_configuration ( void ) {

        ulong value = 0;

	/* 
         *
         * CFG code is composed in this way:
         * cfg_0 -> first bit   X 
         * cfg_1 -> second bit  X
         * cfg_2 -> third bit   X
         *  
         */
	
	imx_iomux_v3_setup_multiple_pads(board_conf_pads, ARRAY_SIZE(board_conf_pads));

	/* Mux as Input */
	C61_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_0);
	C61_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_1);

	/* Read Conf value */
	value = (C61_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_0) >> GPIO_CFG_0) << shift_0 | 
		(C61_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_1) >> GPIO_CFG_1) << shift_1 ; 

        return value;

}

int c61_get_configuration_port1 ( void ) {
	
	struct gpio_desc desc_enet8;
        struct gpio_desc desc_enet9;
	int ret, value_sum;

	ret = dm_gpio_lookup_name("gpio@21_8", &desc_enet8);
        if (ret)
                return -1;

        ret = dm_gpio_lookup_name("gpio@21_9", &desc_enet9);
        if (ret)
                return -1;

	ret = dm_gpio_request(&desc_enet8, "gpio21_cfg0");
        if (ret)
                return -1;

        ret = dm_gpio_request(&desc_enet9, "gpio21_cfg1");
        if (ret)
                return -1;

	dm_gpio_set_dir_flags(&desc_enet8, GPIOD_IS_IN);
	dm_gpio_set_dir_flags(&desc_enet9, GPIOD_IS_IN);

	value_sum = dm_gpio_get_value(&desc_enet8)  | dm_gpio_get_value(&desc_enet9) << 1;

	ret = dm_gpio_free((desc_enet8.dev),&desc_enet8);
        if (ret)
                return -1;

        ret = dm_gpio_free((desc_enet9.dev), &desc_enet9);
        if (ret)
                return -1;


        //printf("PCA953x cfg port1 = 0x%x\n",value_sum);

        return value_sum;

}

int c61_get_configuration_port2 ( void ) {
	
	struct gpio_desc desc_enet10;
        struct gpio_desc desc_enet11;
	int ret, value_sum;

	ret = dm_gpio_lookup_name("gpio@21_10", &desc_enet10);
        if (ret)
                return -1;

        ret = dm_gpio_lookup_name("gpio@21_11", &desc_enet11);
        if (ret)
                return -1;

	ret = dm_gpio_request(&desc_enet10, "gpio21_cfg2");
        if (ret)
                return -1;

        ret = dm_gpio_request(&desc_enet11, "gpio21_cfg3");
        if (ret)
                return -1;

	dm_gpio_set_dir_flags(&desc_enet10, GPIOD_IS_IN);
	dm_gpio_set_dir_flags(&desc_enet11, GPIOD_IS_IN);

	value_sum = dm_gpio_get_value(&desc_enet10)  | dm_gpio_get_value(&desc_enet11) << 1;

	ret = dm_gpio_free((desc_enet10.dev), &desc_enet10);
        if (ret)
                return -1;

        ret = dm_gpio_free((desc_enet11.dev), &desc_enet11);
        if (ret)
                return -1;

	//printf("PCA953x cfg port2 = 0x%x\n",value_sum);

        return value_sum;

}

int c61_get_configuration_video ( void ) {

        struct gpio_desc desc_enet3;
        struct gpio_desc desc_enet4;
        int ret, value_sum;

        ret = dm_gpio_lookup_name("gpio@21_3", &desc_enet3);
        if (ret)
                return -1;

        ret = dm_gpio_lookup_name("gpio@21_4", &desc_enet4);
        if (ret)
                return -1;

        ret = dm_gpio_request(&desc_enet4, "gpio21_cfg13");
        if (ret)
                return -1;

        ret = dm_gpio_request(&desc_enet3, "gpio21_cfg14");
        if (ret)
                return -1;

        dm_gpio_set_dir_flags(&desc_enet3, GPIOD_IS_IN);
        dm_gpio_set_dir_flags(&desc_enet4, GPIOD_IS_IN);

        value_sum = dm_gpio_get_value(&desc_enet4)  | dm_gpio_get_value(&desc_enet3) << 1;

	ret = dm_gpio_free((desc_enet4.dev), &desc_enet4);
        if (ret)
                return -1;

        ret = dm_gpio_free((desc_enet3.dev), &desc_enet3);
        if (ret)
                return -1;

	//printf("PCA953x cfg video = 0x%x\n",value_sum);

        return value_sum;

}

int c61_get_configuration_modem ( void ) {

        struct gpio_desc desc_enet6;
        int ret, value_sum;


        ret = dm_gpio_lookup_name("gpio@21_6", &desc_enet6);
        if (ret)
                return -1;

        ret = dm_gpio_request(&desc_enet6, "gpio21_cfg11");
        if (ret)
                return -1;

        dm_gpio_set_dir_flags(&desc_enet6, GPIOD_IS_IN);

        value_sum = dm_gpio_get_value(&desc_enet6);

        ret = dm_gpio_free((desc_enet6.dev), &desc_enet6);
        if (ret)
                return -1;

	//printf("PCA953x cfg modem = 0x%x\n",value_sum);

        return value_sum;

}
