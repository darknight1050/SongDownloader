#pragma once
#include "../TypeMacros.hpp"
#include "Leaderboard.hpp"
#include "Metadata.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Leaderboards,
    ERROR_CHECK
    GETTER_VALUE(std::vector<ScoreSaber::Leaderboard>, Leaderboards, "leaderboards");
    GETTER_VALUE_OPTIONAL(Metadata_t, Metadata, "metadata");
)
