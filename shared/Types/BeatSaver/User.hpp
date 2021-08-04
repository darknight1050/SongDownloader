#pragma once
#include "../TypeMacros.hpp"
//#include "UserStats.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserDetail, 
    GETTER_VALUE(int, Id);
    GETTER_VALUE(std::string, Username);
    GETTER_VALUE(std::string, AvatarURL);
    GETTER_VALUE_OPTIONAL(std::string, Hash);
    //GETTER_VALUE(BeatSaver::UserStats, stats); // Listed in documentation as well as testplay not seen in any recent requests though
    GETTER_VALUE_OPTIONAL(bool, Testplay);
)