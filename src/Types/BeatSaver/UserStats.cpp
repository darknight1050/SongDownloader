#include "Types/BeatSaver/UserStats.hpp"

DESERIALIZE_METHOD(BeatSaver, UserStats,
    DESERIALIZE_VALUE(TotalUpvotes, totalUpvotes, Int)
    DESERIALIZE_VALUE(TotalDownvotes, totalDownvotes, Int)
    DESERIALIZE_VALUE(TotalMaps, totalMaps, Int)
    DESERIALIZE_VALUE(RankedMaps, rankedMaps, Int)
    DESERIALIZE_VALUE(AvgBpm, avgBpm, Float)
    DESERIALIZE_VALUE(AvgScore, avgScore, Float)
    DESERIALIZE_VALUE(AvgDuration, avgDuration, Float)
    DESERIALIZE_VALUE_OPTIONAL(FirstUpload, firstUpload, String)
    DESERIALIZE_VALUE_OPTIONAL(LastUpload, lastUpload, String)
    DESERIALIZE_CLASS(DiffStats, diffStats)
)