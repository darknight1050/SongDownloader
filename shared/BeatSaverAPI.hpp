#pragma once

#include "Types/BeatSaver/Page.hpp"

#include <string>

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key);

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash);

    std::optional<BeatSaver::Page> SearchPage(std::string query, int pageIndex = 0);

    void DownloadBeatmap(const BeatSaver::Beatmap& beatmap);

}