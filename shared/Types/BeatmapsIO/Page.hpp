#pragma once
#include "../TypeMacros.hpp"
#include "Beatmap.hpp"

#include <vector>

DECLARE_JSON_CLASS(BeatmapsIO, Page,
    GETTER_VALUE(std::vector<BeatmapsIO::Beatmap>, Docs);
    //GETTER_VALUE(int, TotalDocs);
    //GETTER_VALUE(int, LastPage);
    //GETTER_VALUE_OPTIONAL(int, PreviousPage);
    //GETTER_VALUE_OPTIONAL(int, NextPage);
)
