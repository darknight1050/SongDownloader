#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(AutoMapper, bool, "AutoMapper", false, "Show auto-generated Beatmaps");

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(AutoMapper);
    )
)
