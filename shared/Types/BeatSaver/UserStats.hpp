#pragma once
#include "../TypeMacros.hpp"
#include "UserDiffStats.hpp"

DECLARE_JSON_CLASS(BeatSaver, UserStats,
    GETTER_VALUE(int, TotalUpvotes, "totalUpvotes");
    GETTER_VALUE(int, TotalDownvotes, "totalDownvotes");
    GETTER_VALUE(int, TotalMaps, "totalMaps");
    GETTER_VALUE(int, RankedMaps, "rankedMaps");
    GETTER_VALUE(float, AvgBpm, "avgBpm");
    GETTER_VALUE(float, AvgScore, "avgScore");
    GETTER_VALUE(float, AvgDuration, "avgDuration");
    GETTER_VALUE_OPTIONAL(std::string, FirstUpload, "firstUpload");
    GETTER_VALUE_OPTIONAL(std::string, LastUpload, "lastUpload");
    GETTER_VALUE(BeatSaver::UserDiffStats, DiffStats, "diffStats");
    ERROR_CHECK
)
