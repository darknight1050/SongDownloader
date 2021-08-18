#pragma once
#include "../TypeMacros.hpp"
#include "Song.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Page,
    GETTER_VALUE(std::vector<ScoreSaber::Song>, Songs);
)
