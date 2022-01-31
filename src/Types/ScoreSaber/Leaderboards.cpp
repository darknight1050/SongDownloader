#include "Types/ScoreSaber/Leaderboards.hpp"

DESERIALIZE_METHOD(ScoreSaber, Leaderboards,
    DESERIALIZE_VECTOR(Leaderboards, leaderboards, ScoreSaber::Leaderboard)
    //DESERIALIZE_VALUE(Next_page, next_page, Int)
    DESERIALIZE_CLASS_OPTIONAL(Metadata, metadata)
)
