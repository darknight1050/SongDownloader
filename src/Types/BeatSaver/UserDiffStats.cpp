#include "Types/BeatSaver/UserDiffStats.hpp"

DESERIALIZE_METHOD(BeatSaver, UserDiffStats,
    DESERIALIZE_VALUE(Total, total, Int)
    DESERIALIZE_VALUE(Easy, easy, Int)
    DESERIALIZE_VALUE(Normal, normal, Int)
    DESERIALIZE_VALUE(Hard, hard, Int)
    DESERIALIZE_VALUE(Expert, expert, Int)
    DESERIALIZE_VALUE(ExpertPlus, expertPlus, Int)
)