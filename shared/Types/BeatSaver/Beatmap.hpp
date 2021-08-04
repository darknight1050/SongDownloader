#pragma once
#include "../TypeMacros.hpp"
#include "User.hpp"
#include "Metadata.hpp"
#include "Stats.hpp"
#include "BeatmapVersion.hpp"

DECLARE_JSON_CLASS(BeatSaver, Beatmap, 
    GETTER_VALUE(bool, Automapper);
    GETTER_VALUE_OPTIONAL(std::string, Curator);
    GETTER_VALUE(std::string, Description);
    GETTER_VALUE(std::string, Id);
    GETTER_VALUE(BeatSaver::Metadata, Metadata);
    GETTER_VALUE(std::string, Name);
    GETTER_VALUE(bool, Qualified);
    GETTER_VALUE(bool, Ranked);
    //GETTER_VALUE(BeatSaver::Instant, Uploaded);
    //GETTER_VALUE(BeatSaver::User, Uploader);
    GETTER_VALUE(std::vector<BeatSaver::BeatmapVersion>, Versions);
    GETTER_VALUE(BeatSaver::Stats, Stats);
)
