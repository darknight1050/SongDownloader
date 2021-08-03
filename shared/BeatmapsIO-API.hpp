#pragma once

#include "Types/BeatSaver/Page.hpp"

#include <string>

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapById(int id);

    // Deprecated will not include new Beatmaps! Please use 'GetBeatmapByHash' or 'GetBeatmapById' instead
    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key);

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash);

    std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex);

    bool DownloadBeatmap(const BeatSaver::Beatmap& beatmap);

    std::vector<uint8_t> GetCoverImage(const BeatSaver::Beatmap& beatmap);

    std::vector<uint8_t> GetPreview(const BeatSaver::Beatmap& beatmap);

    void GetBeatmapByIdAsync(int id, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    // Deprecated will not include new Beatmaps! Please use 'GetBeatmapByHashAsync' or 'GetBeatmapByIdAsync' instead
    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished);

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);
    
    // Returns mp3 with previewAudio
    void GetPreviewAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

}