#pragma once
#include "../TypeMacros.hpp"
#include "User.hpp"
#include "Metadata.hpp"
#include "Stats.hpp"
#include "BeatmapVersion.hpp"

namespace BeatSaver {
    class Beatmap;
    namespace API {
        void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate);
        void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate);
    }
}

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
    inline void DownloadLatestBeatmapAsync(std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        BeatSaver::API::DownloadBeatmapAsync(*this, finished, progressUpdate);
    }
    inline void GetLatestCoverImageAsync(std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        BeatSaver::API::GetCoverImageAsync(*this, finished, progressUpdate);
    }
)
