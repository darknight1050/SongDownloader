#pragma once

#include "Types/BeatSaver/Page.hpp"

#include <string>

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key);

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash);

    /// <summary>
    /// Search on BeatSaver
    /// </summary>
    /// <param name="query"> What you want to search on BeatSaver</param>
    /// <param name="pageIndex"> Index of the pages search results</param>
    /// <param name="sortOrder"> Order in which the search results should be sorted default "Relevance"</param>
    /// <returns>An std::optional that either contains BeatSaver::Page or std::nullopt</returns>
    std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex, std::string sortOrder = "Relevance");

    bool DownloadBeatmap(const BeatSaver::Beatmap& beatmap);

    std::vector<uint8_t> GetCoverImage(const BeatSaver::Beatmap& beatmap);

    std::vector<uint8_t> GetPreview(const BeatSaver::Beatmap& beatmap);

    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    /// <summary>
    /// Get a Beatmap based on Hash
    /// </summary>
    /// <param name="hash"> Hash of the beatmap, levelID of a custom level contains it</param>
    /// <param name="finished"> Function to run after it finishes loading</param>
    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    /// <summary>
    /// Search on BeatSaver
    /// </summary>
    /// <param name="query"> What you want to search on BeatSaver</param>
    /// <param name="pageIndex"> Index of the pages search results</param>
    /// <param name="finished"> Function to run after it finishes loading</param>
    /// <param name="sortOrder"> Order in which the search results should be sorted default "Relevance"</param>
    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished, std::string sortOrder = "Relevance");

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);
    
    // Returns mp3 with previewAudio
    void GetPreviewAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

}