#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserDiffStats,
    GETTER_VALUE(int, Total);
    GETTER_VALUE(int, Easy);
    GETTER_VALUE(int, Normal);
    GETTER_VALUE(int, Hard);
    GETTER_VALUE(int, Expert);
    GETTER_VALUE(int, ExpertPlus);
)