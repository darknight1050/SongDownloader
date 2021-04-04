#pragma once
#include "../TypeMacros.hpp"
#include "User.hpp"
#include "Metadata.hpp"
#include "Stats.hpp"

DECLARE_JSON_CLASS(BeatSaver, Beatmap, 
    GETTER_VALUE(std::string, Id);
    GETTER_VALUE(std::string, Key);
    GETTER_VALUE(std::string, Name);
    GETTER_VALUE_OPTIONAL(std::string, Description);
    GETTER_VALUE(BeatSaver::User, Uploader);
    GETTER_VALUE(std::string, Uploaded);
    GETTER_VALUE(BeatSaver::Metadata, Metadata);
    GETTER_VALUE(BeatSaver::Stats, Stats);
    GETTER_VALUE(std::string, DirectDownload);
    GETTER_VALUE(std::string, DownloadURL);
    GETTER_VALUE(std::string, CoverURL);
    GETTER_VALUE(std::string, Hash);
)
