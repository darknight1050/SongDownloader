#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(AutoMapper, std::string, "AutoMapper", "", "Show auto-generated Beatmaps");
    CONFIG_VALUE(SearchType, std::string, "Search Type", "Search", "What to Display!");
    CONFIG_VALUE(SortOrder, std::string, "Sort Order", "Relevance", "How results should be sorted");
    CONFIG_VALUE(NE, std::string, "NE", "", "If NoodleExtensions maps should be included in search results");
    CONFIG_VALUE(ME, std::string, "ME", "", "If MappingExtensions maps should be included in search results");
    CONFIG_VALUE(Chroma, std::string, "Chroma", "", "If Chroma maps should be included in search results");
    CONFIG_VALUE(Ranked, std::string, "Ranked", "", "If Ranked maps should be included in search results");

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(AutoMapper);
        CONFIG_INIT_VALUE(SearchType);
        CONFIG_INIT_VALUE(SortOrder);
        CONFIG_INIT_VALUE(NE);
        CONFIG_INIT_VALUE(ME);
        CONFIG_INIT_VALUE(Chroma);
        CONFIG_INIT_VALUE(Ranked);
    )
)
