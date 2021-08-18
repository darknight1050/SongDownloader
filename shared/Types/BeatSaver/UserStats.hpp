#pragma once
#include "../TypeMacros.hpp"
#include "UserDiffStats.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserStats,
    GETTER_VALUE(int, TotalUpvotes);
    GETTER_VALUE(int, TotalDownvotes);
    GETTER_VALUE(int, TotalMaps);
    GETTER_VALUE(int, RankedMaps);
    GETTER_VALUE(float, AvgBpm);
    GETTER_VALUE(float, AvgScore);
    GETTER_VALUE(int, AvgDuration);
    GETTER_VALUE(std::string, FirstUpload);
    GETTER_VALUE(std::string, LastUpload);
    GETTER_VALUE(BeatSaver::UserDiffStats, DiffStats);
)