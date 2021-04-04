#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, User, 
    GETTER_VALUE(std::string, Id);
    GETTER_VALUE(std::string, Username);
)