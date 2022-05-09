#pragma once
#include "../TypeMacros.hpp"
#include <vector>

DECLARE_JSON_CLASS(BeatSaver, Metadata, 
    GETTER_VALUE(float, BPM, "bpm");
    GETTER_VALUE(int, Duration, "duration");
    GETTER_VALUE(std::string, SongName, "songName");
    GETTER_VALUE(std::string, SongSubName, "songSubName");
    GETTER_VALUE(std::string, SongAuthorName, "songAuthorName");
    GETTER_VALUE(std::string, LevelAuthorName, "levelAuthorName");
    ERROR_CHECK
)
