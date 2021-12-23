#
# Copyright 2018 NXP
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += seco_imx8mn_c72.o
obj-$(CONFIG_CMD_SECO_CONFIG_IMX8) += env_conf.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
obj-$(CONFIG_IMX8M_DDR4) += ddr4_timing.o
endif
