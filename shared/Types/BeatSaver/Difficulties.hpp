#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, Difficulties, 
    GETTER_VALUE(bool, Easy);
    GETTER_VALUE(bool, Normal);
    GETTER_VALUE(bool, Hard);
    GETTER_VALUE(bool, Expert);
    GETTER_VALUE(bool, ExpertPlus);
)