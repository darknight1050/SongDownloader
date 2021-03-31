#pragma once
#include "../TypeMacros.hpp"

DECLARE_CLASS(BeatSaver, BeatmapCharacteristicDifficulty, 
    GETTER_VALUE(float, Duration);
    GETTER_VALUE(int, Length);
    GETTER_VALUE(int, Bombs);
    GETTER_VALUE(int, Notes);
    GETTER_VALUE(int, Obstacles);
    GETTER_VALUE(float, NoteJumpSpeed);
    GETTER_VALUE(float, NoteJumpSpeedOffset);
)