#pragma once

#include "Types/BeatSaver/Page.hpp"

#include <string>

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key);

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash);

    std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex);

    void DownloadBeatmap(const BeatSaver::Beatmap& beatmap);
    

    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished);

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate);
    
}