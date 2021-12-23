/*
 *
 *
 * Reading C57 Board type
 *
 * tommaso.merciai@seco.com
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
#include <seco/seco_imx8qxp_c57_setup.h>

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE )

/* ____________________________________________________________________________
  |                                                                            |
  |                                  C57 REVISION                              |
  |____________________________________________________________________________|
*/

#define GPIO_PAD_CTRL   ((SC_PAD_CONFIG_OD_IN << PADRING_CONFIG_SHIFT) | (SC_PAD_ISO_OFF << PADRING_LPCONFIG_SHIFT))

#define SECO_CODE_4 IMX_GPIO_NR(4,19)
#define SECO_CODE_5 IMX_GPIO_NR(3,2)



iomux_cfg_t const board_conf_pads[] = {

	SC_P_USDHC1_RESET_B | MUX_MODE_ALT(4) | MUX_PAD_CTRL(GPIO_PAD_CTRL), /* SECO_CODE_4 */
	SC_P_CSI_EN | MUX_MODE_ALT(4) | MUX_PAD_CTRL(GPIO_PAD_CTRL),         /* SECO_CODE_5 */
};


int c57_get_board_configuration ( void ) {

    int value = 0;
    int ret = 0;
    int gpio;
    struct gpio_desc seco_code4;
    struct gpio_desc seco_code5;

    ret = sc_pm_set_resource_power_mode(-1, SC_R_GPIO_3, SC_PM_PW_MODE_ON);
    if (ret)
    return -1;
    ret = sc_pm_set_resource_power_mode(-1, SC_R_GPIO_4, SC_PM_PW_MODE_ON);
    if (ret)
    return -1;

    ret = dm_gpio_lookup_name("gpio4_19", &seco_code4);
    if (ret)
    return -1;

    ret = dm_gpio_lookup_name("gpio3_2", &seco_code5);
    if (ret)
    return -1;

	ret = dm_gpio_request(&seco_code4, "seco_code4");
    if (ret)
    return -1;


    ret = dm_gpio_request(&seco_code5, "seco_code5");
    if (ret)
    return -1;


	dm_gpio_set_dir_flags(&seco_code4, GPIOD_IS_IN);
	dm_gpio_set_dir_flags(&seco_code5, GPIOD_IS_IN);


    value = dm_gpio_get_value(&seco_code4)  | dm_gpio_get_value(&seco_code5) << 1;

	ret = dm_gpio_free((seco_code4.dev),&seco_code4);
    if (ret)
            return -1;

    ret = dm_gpio_free((seco_code5.dev), &seco_code5);
    if (ret)
            return -1;

    return value;

}
