#pragma once
#include "../TypeMacros.hpp"
//#include "UserStats.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserDetail, 
    GETTER_VALUE(std::string, AvatarURL);
    GETTER_VALUE(std::string, Hash);
    GETTER_VALUE(int, Id);
    GETTER_VALUE(std::string, Username);
    //GETTER_VALUE(BeatSaver::UserStats, stats);
    GETTER_VALUE_OPTIONAL(bool, Testplay);
)