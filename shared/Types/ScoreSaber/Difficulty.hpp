#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Difficulty_t,
    GETTER_VALUE(int, LeaderboardId);
    GETTER_VALUE(int, Difficulty);
    GETTER_VALUE(std::string, GameMode);
    GETTER_VALUE(std::string, DifficultyRaw);
)
