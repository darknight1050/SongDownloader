#include "Types/ScoreSaber/Leaderboards.hpp"

DESERIALIZE_METHOD(ScoreSaber, Leaderboards,
    DESERIALIZE_VECTOR(Leaderboards, leaderboards, ScoreSaber::Leaderboard)
    //DESERIALIZE_VALUE(Next_page, next_page, Int)
    DESERIALIZE_VALUE(TotalCount, totalCount, Int)
)
