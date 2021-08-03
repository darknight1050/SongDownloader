#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatmapsIO, User, 
    GETTER_VALUE(std::string, AvatarURL);
    GETTER_VALUE(std::string, Hash);
    GETTER_VALUE(int, Id);
    GETTER_VALUE(std::string, Username);
)