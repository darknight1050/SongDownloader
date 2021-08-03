#pragma once
#include "../TypeMacros.hpp"
#include <vector>

DECLARE_JSON_CLASS(BeatSaver, Metadata, 
    GETTER_VALUE(float, BPM);
    GETTER_VALUE(int, Duration);
    GETTER_VALUE(std::string, SongName);
    GETTER_VALUE(std::string, SongSubName);
    GETTER_VALUE(std::string, SongAuthorName);
    GETTER_VALUE(std::string, LevelAuthorName);
)
