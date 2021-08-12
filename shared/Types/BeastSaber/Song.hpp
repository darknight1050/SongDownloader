#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeastSaber, Song, 
    GETTER_VALUE(std::string, Title);
    GETTER_VALUE(std::string, Song_key);
    GETTER_VALUE(std::string, Hash);
    GETTER_VALUE(std::string, Level_author_name);
    GETTER_VALUE_OPTIONAL(std::string, Curated_by);
)
