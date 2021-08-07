#include "Types/BeatSaver/Metadata.hpp"

DESERIALIZE_METHOD(BeatSaver, Metadata, 
    DESERIALIZE_VALUE(BPM, bpm, Float)
    DESERIALIZE_VALUE(Duration, duration, Int)
    DESERIALIZE_VALUE(SongName, songName, String)
    DESERIALIZE_VALUE(SongSubName, songSubName, String)
    DESERIALIZE_VALUE(SongAuthorName, songAuthorName, String)
    DESERIALIZE_VALUE(LevelAuthorName, levelAuthorName, String)
)