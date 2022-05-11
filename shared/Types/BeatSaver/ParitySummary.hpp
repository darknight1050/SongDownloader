#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, BeatmapParitySummary,
    GETTER_VALUE(int, Errors, "errors");
    GETTER_VALUE(int, Warns, "warns");
    GETTER_VALUE(int, Resets, "resets");
    ERROR_CHECK
)
