#pragma once
#include "../TypeMacros.hpp"
#include "Difficulty.hpp"

DECLARE_JSON_CLASS(BeatmapsIO, BeatmapVersion,
    GETTER_VALUE(std::string, CreatedAt);
    GETTER_VALUE(int, SageScore);
    GETTER_VALUE(std::vector<BeatmapsIO::BeatmapDifficulty>, Diffs);
    GETTER_VALUE_OPTIONAL(std::string, Feedback);
    GETTER_VALUE(std::string, Hash);
    GETTER_VALUE(std::string, Key);
    GETTER_VALUE(std::string, State); // Enum with values Uploaded, Testplay, Published, Feedback
    //GETTER_VALUE(BeatmapsIO::Instant, TestplayAt);
    //GETTER_VALUE(BeatmapsIO::Testplays, Uploaded);
    GETTER_VALUE(std::string, DownloadURL);
    GETTER_VALUE(std::string, CoverURL);
    GETTER_VALUE(std::string, PreviewURL);
)
