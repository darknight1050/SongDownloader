#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatmapsIO, BeatmapDifficulty, 
    GETTER_VALUE(float, NJS);
    GETTER_VALUE(float, Offset);
    GETTER_VALUE(int, Notes);
    GETTER_VALUE(int, Bombs);
    GETTER_VALUE(int, Obstacles);
    GETTER_VALUE(double, NPS);
    GETTER_VALUE(int, Length);
    GETTER_VALUE(std::string, Characteristic); // Enum [ Standard, OneSaber, NoArrows, _90Degree, _360Degree, Lightshow, Lawless ] String for characteristic
    GETTER_VALUE(std::string, Difficulty); // Enum [ Easy, Normal, Hard, Expert, ExpertPlus ] String for diffculty
    GETTER_VALUE(int, Events);
    GETTER_VALUE(bool, Chroma);
    GETTER_VALUE(bool, ME);
    GETTER_VALUE(bool, NE);
    GETTER_VALUE(bool, Cinema);
    GETTER_VALUE(double, Seconds);
    //GETTER_VALUE(ParitySummary, ParitySummary);
    GETTER_VALUE_OPTIONAL(float, Stars); // Listed in the documentation not seen on any response
)