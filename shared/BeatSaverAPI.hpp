#pragma once

#include "Types/BeatSaver/Beatmap.hpp"

#include <string>

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key);

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash);

    void DownloadBeatmap(const BeatSaver::Beatmap& beatmap);

}