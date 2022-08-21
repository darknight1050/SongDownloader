#pragma once
#include "../TypeMacros.hpp"
#include "UserStats.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserDetail, 
    ERROR_CHECK
    GETTER_VALUE(int, Id, "id");
    GETTER_VALUE(std::string, Username, "name");
    GETTER_VALUE_OPTIONAL(std::string, Hash, "hash");
    GETTER_VALUE(std::string, AvatarURL, "avatar");
    GETTER_VALUE_OPTIONAL(bool, Testplay, "testplay");
    GETTER_CLASS_OPTIONAL(BeatSaver::UserStats, Stats, "stats");
)
