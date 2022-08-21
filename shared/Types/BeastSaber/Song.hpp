#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeastSaber, Song, 
    ERROR_CHECK
    GETTER_VALUE(std::string, Title, "title");
    GETTER_VALUE(std::string, Song_key, "song_key");
    GETTER_VALUE(std::string, Hash, "hash");
    GETTER_VALUE(std::string, Level_author_name, "level_author_name");
    GETTER_VALUE_OPTIONAL(std::string, Curated_by, "curated_by");
)
