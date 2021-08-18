#pragma once
#include "../TypeMacros.hpp"
#include "User.hpp"
#include "Metadata.hpp"
#include "Stats.hpp"
#include "BeatmapVersion.hpp"

DECLARE_JSON_CLASS(BeatSaver, Beatmap, 
    GETTER_VALUE(std::string, Id);
    GETTER_VALUE(std::string, Name);
    GETTER_VALUE(std::string, Description);
    GETTER_VALUE(BeatSaver::UserDetail, Uploader);
    GETTER_VALUE(BeatSaver::Metadata, Metadata);
    GETTER_VALUE(BeatSaver::Stats, Stats);
    GETTER_VALUE(std::string, Uploaded);
    GETTER_VALUE(bool, Automapper);
    GETTER_VALUE(bool, Ranked);
    GETTER_VALUE(bool, Qualified);
    GETTER_VALUE(std::vector<BeatSaver::BeatmapVersion>, Versions);
    GETTER_VALUE_OPTIONAL(std::string, Curator);
)
