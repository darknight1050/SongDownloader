#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, BeatmapParitySummary,
    GETTER_VALUE(int, Errors);
    GETTER_VALUE(int, Warns);
    GETTER_VALUE(int, Resets);
)