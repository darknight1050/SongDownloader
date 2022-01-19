#include "Types/ScoreSaber/Difficulty.hpp"

DESERIALIZE_METHOD(ScoreSaber, Difficulty_t,
    DESERIALIZE_VALUE(LeaderboardId, leaderboardId, Int)
    DESERIALIZE_VALUE(Difficulty, difficulty, Int)
    DESERIALIZE_VALUE(GameMode, gameMode, String)
    DESERIALIZE_VALUE(DifficultyRaw, difficultyRaw, String)
)