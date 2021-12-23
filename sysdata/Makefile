# SPDX-License-Identifier: GPL-2.0+
#
# (C) Copyright 2004-2006
# Davide Cardillo, SECO SpA, seco.com



ifndef CONFIG_SPL_BUILD

ifeq ($(CONFIG_SECO_SYSTEM_DATA),y)
obj-y += sysdata.o
obj-$(CONFIG_SECO_SYSDATA_IN_MMC) += mmc.o
endif

endif
