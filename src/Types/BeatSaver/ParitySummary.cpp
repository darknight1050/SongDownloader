#include "Types/BeatSaver/ParitySummary.hpp"

DESERIALIZE_METHOD(BeatSaver, BeatmapParitySummary,
    DESERIALIZE_VALUE(Errors, errors, Int)
    DESERIALIZE_VALUE(Warns, warns, Int)
    DESERIALIZE_VALUE(Resets, resets, Int)
)