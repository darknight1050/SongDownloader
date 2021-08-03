#pragma once
#include "../TypeMacros.hpp"
#include "User.hpp"
#include "Metadata.hpp"
#include "Stats.hpp"
#include "BeatmapVersion.hpp"

DECLARE_JSON_CLASS(BeatmapsIO, Beatmap, 
    GETTER_VALUE(bool, Automapper);
	GETTER_VALUE_OPTIONAL(std::string, Curator);
    GETTER_VALUE_OPTIONAL(std::string, Description);
    GETTER_VALUE(int, Id);
    GETTER_VALUE(BeatmapsIO::Metadata, Metadata);
    GETTER_VALUE(std::string, Name);
	GETTER_VALUE(bool, Qualified);
	GETTER_VALUE(bool, Ranked);
	//GETTER_VALUE(BeatmapsIO::Instant, Uploaded);
    //GETTER_VALUE(BeatmapsIO::User, Uploader);
	GETTER_VALUE(std::vector<BeatmapsIO::BeatmapVersion>, Versions);
    GETTER_VALUE(BeatmapsIO::Stats, Stats);
)
