/*
 * 
 *
 * Reading C43 Board type
 *
 * marco.sandrelli@seco.com
 *
 *
 *
 */

#include <common.h>
#include <errno.h>

#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/clock.h>
#include <asm/arch/sci/sci.h>
#include <asm/arch/iomux.h>
#include <asm/arch/imx8-pins.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/sys_proto.h>
#include <seco/seco_imx8qm_c43_setup.h>

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE )

/* ____________________________________________________________________________
  |                                                                            |
  |                                  C43 REVISION                              |
  |____________________________________________________________________________|
*/

#define GPIO_PAD_CTRL   ((SC_PAD_CONFIG_NORMAL << PADRING_CONFIG_SHIFT) | (SC_PAD_ISO_OFF << PADRING_LPCONFIG_SHIFT))


#define SECO_CODE_0	IMX_GPIO_NR(1,11)
#define SECO_CODE_1     IMX_GPIO_NR(1,12)	

iomux_cfg_t const board_conf_pads[] = {

        /* RAM CONFIG */
	SC_P_LVDS1_GPIO01 | MUX_MODE_ALT(3) | MUX_PAD_CTRL(GPIO_PAD_CTRL), /* SECO_CODE_0 */
	SC_P_LVDS1_I2C0_SCL | MUX_MODE_ALT(3) | MUX_PAD_CTRL(GPIO_PAD_CTRL), /* SECO_CODE_1 */

};


int c43_get_board_configuration ( void ) {

        ulong value = 0;

	/* 
         *
         * CFG code is composed in this way:
         * SECO_CODE_0 -> first bit   X 
         * SECO_CODE_1 -> second bit  X
         *  
         */
	
	imx8_iomux_setup_multiple_pads (board_conf_pads, ARRAY_SIZE(board_conf_pads));

	/* Mux as Input */
	gpio_request(SECO_CODE_0, "seco_code_0");
	gpio_request(SECO_CODE_1, "seco_code_1");
        gpio_direction_input(SECO_CODE_0);
        gpio_direction_input(SECO_CODE_1);


	/* Read Conf value */
	value = ((gpio_get_value(SECO_CODE_1))<<1) | gpio_get_value(SECO_CODE_0) ; 

        return value;

}
