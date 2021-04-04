#pragma once
#include "../TypeMacros.hpp"
#include "Beatmap.hpp"

#include <vector>

DECLARE_CLASS(BeatSaver, Page, 
    GETTER_VALUE(std::vector<BeatSaver::Beatmap>, Docs);
    GETTER_VALUE(int, TotalDocs);
    GETTER_VALUE(int, LastPage);
    GETTER_VALUE_OPTIONAL(int, PreviousPage);
    GETTER_VALUE_OPTIONAL(int, NextPage);
)
