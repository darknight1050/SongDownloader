#pragma once
#include "../TypeMacros.hpp"
#include "Beatmap.hpp"

#include <vector>

DECLARE_JSON_CLASS(BeatmapsIO, Page,
    GETTER_VALUE(std::vector<BeatmapsIO::Beatmap>, Docs);
)
