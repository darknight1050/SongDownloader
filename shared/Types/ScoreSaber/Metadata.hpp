#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Metadata_t,
    GETTER_VALUE(unsigned int, Total, "total");
    GETTER_VALUE(int, Page, "page");
    GETTER_VALUE(int, ItemsPerPage, "itemsPerPage");
    ERROR_CHECK
)
