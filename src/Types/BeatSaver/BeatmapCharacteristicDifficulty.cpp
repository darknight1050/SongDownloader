#include "Types/BeatSaver/BeatmapCharacteristicDifficulty.hpp"

DESERIALIZE_METHOD(BeatSaver, BeatmapCharacteristicDifficulty, 
    DESERIALIZE_VALUE(Duration, duration, Float)
    DESERIALIZE_VALUE(Length, length, Int)
    DESERIALIZE_VALUE(Bombs, bombs, Int)
    DESERIALIZE_VALUE(Notes, notes, Int)
    DESERIALIZE_VALUE(Obstacles, obstacles, Int)
    DESERIALIZE_VALUE(NoteJumpSpeed, njs, Float)
    DESERIALIZE_VALUE(NoteJumpSpeedOffset, njsOffset, Float)
)