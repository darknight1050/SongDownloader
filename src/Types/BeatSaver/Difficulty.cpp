#include "Types/BeatSaver/Difficulty.hpp"

DESERIALIZE_METHOD(BeatSaver, BeatmapDifficulty,
    DESERIALIZE_VALUE(NJS, njs, Float)
    DESERIALIZE_VALUE(Offset, offset, Float)
    DESERIALIZE_VALUE(Notes, notes, Int)
    DESERIALIZE_VALUE(Bombs, bombs, Int)
    DESERIALIZE_VALUE(Obstacles, obstacles, Int)
    DESERIALIZE_VALUE(NPS, nps, Double)
    DESERIALIZE_VALUE(Length, length, Int)
    DESERIALIZE_VALUE(Characteristic, characteristic, String)
    DESERIALIZE_VALUE(Difficulty, difficulty, String)
    DESERIALIZE_VALUE(Events, events, Int)
    DESERIALIZE_VALUE(Chroma, chroma, Bool)
    DESERIALIZE_VALUE(ME, me, Bool)
    DESERIALIZE_VALUE(NE, ne, Bool)
    DESERIALIZE_VALUE(Cinema, cinema, Bool)
    DESERIALIZE_VALUE(Seconds, seconds, Int)
    DESERIALIZE_CLASS(ParitySummary, paritySummary)
    DESERIALIZE_VALUE_OPTIONAL(Stars, stars, Float)

)