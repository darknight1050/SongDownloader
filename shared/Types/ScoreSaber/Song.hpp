#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(ScoreSaber, Song, 
    GETTER_VALUE(int, Uid);
    GETTER_VALUE(std::string, Id);
    GETTER_VALUE(std::string, Name);
    GETTER_VALUE(std::string, SongSubName);
    GETTER_VALUE(std::string, SongAuthorName);
    GETTER_VALUE(std::string, LevelAuthorName);
    GETTER_VALUE(int, Bpm);
    GETTER_VALUE(std::string, Diff);
    GETTER_VALUE(std::string, Scores);
    GETTER_VALUE(int, Scores_day);
    GETTER_VALUE(int, Ranked);
    GETTER_VALUE(int, Stars);
    GETTER_VALUE(std::string, Image);
)
