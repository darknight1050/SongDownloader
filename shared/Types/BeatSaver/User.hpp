#pragma once
#include "../TypeMacros.hpp"
#include "UserStats.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserDetail, 
    GETTER_VALUE(int, Id);
    GETTER_VALUE(std::string, Username);
    GETTER_VALUE_OPTIONAL(std::string, Hash);
    GETTER_VALUE(std::string, AvatarURL);
    GETTER_VALUE_OPTIONAL(bool, Testplay);
    GETTER_CLASS_OPTIONAL(BeatSaver::UserStats, Stats);
)