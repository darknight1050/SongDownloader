#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(AutoMapper, bool, "AutoMapper", false, "Show auto-generated Beatmaps");
    CONFIG_VALUE(BsrSearch, bool, "!bsr Search", false, "Search for a !bsr key");

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(AutoMapper);
        CONFIG_INIT_VALUE(BsrSearch);
    )
)
