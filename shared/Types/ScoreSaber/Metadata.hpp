#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Metadata_t,
    GETTER_VALUE(unsigned int, Total);
    GETTER_VALUE(int, Page);
    GETTER_VALUE(int, ItemsPerPage);
)
