#pragma once
#include "../TypeMacros.hpp"
#include "ParitySummary.hpp"

DECLARE_JSON_CLASS(BeatSaver, BeatmapDifficulty, 
    GETTER_VALUE(float, NJS);
    GETTER_VALUE(float, Offset);
    GETTER_VALUE(int, Notes);
    GETTER_VALUE(int, Bombs);
    GETTER_VALUE(int, Obstacles);
    GETTER_VALUE(double, NPS);
    GETTER_VALUE(int, Length);
    GETTER_VALUE(std::string, Characteristic); // Enum string [ Standard, OneSaber, NoArrows, _90Degree, _360Degree, Lightshow, Lawless ] for characteristic
    GETTER_VALUE(std::string, Difficulty); // Enum string [ Easy, Normal, Hard, Expert, ExpertPlus ] for diffculty
    GETTER_VALUE(int, Events);
    GETTER_VALUE(bool, Chroma);
    GETTER_VALUE(bool, ME);
    GETTER_VALUE(bool, NE);
    GETTER_VALUE(bool, Cinema);
    GETTER_VALUE(double, Seconds);
    GETTER_VALUE(BeatSaver::BeatmapParitySummary, ParitySummary);
    GETTER_VALUE_OPTIONAL(float, Stars); // No idea what that value really is for, but it sure exists on some maps
)