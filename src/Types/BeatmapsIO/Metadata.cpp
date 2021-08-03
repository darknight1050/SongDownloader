#include "Types/BeatmapsIO/Metadata.hpp"

DESERIALIZE_METHOD(BeatmapsIO, Metadata, 
    //DESERIALIZE_VALUE(BPM, bpm, Float)
    DESERIALIZE_VALUE(Duration, duration, Int)
    DESERIALIZE_VALUE(LevelAuthorName, levelAuthorName, String)
    DESERIALIZE_VALUE(SongAuthorName, songAuthorName, String)
    DESERIALIZE_VALUE(SongName, songName, String)
    DESERIALIZE_VALUE(SongSubName, songSubName, String)
    //DESERIALIZE_VALUE_OPTIONAL(Automapper, automapper, String)
    //DESERIALIZE_CLASS(Difficulties, difficulties)
    //DESERIALIZE_VECTOR(Characteristics, characteristics, BeatmapCharacteristic)
)