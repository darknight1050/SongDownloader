#pragma once
#include "../TypeMacros.hpp"
#include "Leaderboard.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Leaderboards,
    GETTER_VALUE(std::vector<ScoreSaber::Leaderboard>, Leaderboards);
    GETTER_VALUE(int, TotalCount);
)
