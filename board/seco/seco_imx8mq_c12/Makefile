#
# Copyright 2017 NXP
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += seco_imx8mq_c12.o
obj-$(CONFIG_CMD_SECO_CONFIG_IMX8) += env_conf.o
ifdef CONFIG_SPL_BUILD
obj-y += spl.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing.o lpddr4_timing_b0.o lpddr4_timing_d0.o
endif

