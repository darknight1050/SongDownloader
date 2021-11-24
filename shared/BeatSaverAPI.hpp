#pragma once

#include "Types/BeastSaber/Song.hpp"

#include "Types/BeatSaver/Page.hpp"

#include "Types/ScoreSaber/Page.hpp"
#include "Types/ScoreSaber/Leaderboards.hpp"

#include <string>

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key);

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash);

    std::optional<BeatSaver::UserDetail> GetUserById(int id);

    /// <summary>
    /// Search on BeatSaver
    /// </summary>
    /// <param name="query"> What you want to search on BeatSaver</param>
    /// <param name="pageIndex"> Index of the pages search results</param>
    /// <param name="sortOrder"> Order in which the search results should be sorted default "Relevance"</param>
    /// <returns>An std::optional that either contains BeatSaver::Page or std::nullopt</returns>
    /// <param name="ME"> MappingExtensions maps should be included in search default "" for unset can be "true" or "false" otherwise</param>
    /// <param name="NE"> MappingExtensions maps should be included in search default "" for unset can be "true" or "false" otherwise</param>
    /// <param name="Chroma"> MappingExtensions maps should be included in search default "" for unset can be "true" or "false" otherwise</param>
    std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex, std::string sortOrder = "Relevance", std::string ME = "", std::string NE = "", std::string Chroma = "");

    /// <summary>
    /// Get maps ordered by upload date
    /// </summary>
    /// <param name="automapper">If automapper maps should be included, default false</param>
    /// <param name="after">Specify to only show maps after specific data format 
    /// "<date>T<time>" with date in "YYYY-DD-MM" and time in "hh:mm:ss.sss" plus timezone designation or "Z" for the UTC timezone 
    /// for example "2020-03-08T01:46:20.649Z" or "2020-01-08T00:00:00.000+02:00" for UTC +02:00 read ISO 8601 UTC Date time
    /// defaults to an empty string</param>
    /// <returns></returns>
    std::optional<BeatSaver::Page> LatestPaged(bool automapper = false, std::string after = "");

    std::optional<BeatSaver::Page> PlaysPaged(int pageIndex);

    bool DownloadBeatmap(const BeatSaver::Beatmap& beatmap);

    std::vector<uint8_t> GetCoverImage(const BeatSaver::Beatmap& beatmap);

    // You need to check yourself if the bytes would make a valid coverImage
    std::optional<std::vector<uint8_t>> GetCoverImage(std::string hash);

    std::vector<uint8_t> GetPreview(const BeatSaver::Beatmap& beatmap);

    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    /// <summary>
    /// Get a Beatmap based on Hash
    /// </summary>
    /// <param name="hash"> Hash of the beatmap, levelID of a custom level contains it</param>
    /// <param name="finished"> Function to run after it finishes loading</param>
    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    void GetBeatmapByUserIdAsync(int userID, int page, std::function<void(std::optional<BeatSaver::Page>)> finished);

    void GetUserByIdAsync(int id, std::function<void(std::optional<BeatSaver::UserDetail>)> finished);

    /// <summary>
    /// Search on BeatSaver
    /// </summary>
    /// <param name="query"> What you want to search on BeatSaver</param>
    /// <param name="pageIndex"> Index of the pages search results</param>
    /// <param name="finished"> Function to run after it finishes loading</param>
    /// <param name="sortOrder"> Order in which the search results should be sorted default "Relevance"</param>
    /// <param name="ME"> MappingExtensions maps should be included in search default "" for unset can be "true" or "false" otherwise</param>
    /// <param name="NE"> MappingExtensions maps should be included in search default "" for unset can be "true" or "false" otherwise</param>
    /// <param name="Chroma"> MappingExtensions maps should be included in search default "" for unset can be "true" or "false" otherwise</param>
    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished, std::string sortOrder = "Relevance", std::string automapper = "", std::string ranked = "", std::string ME = "", std::string NE = "", std::string Chroma = "");

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, const BeatSaver::BeatmapVersion& beatmapVer, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    /// <summary>
    /// For downloading maps gotten from the BeastSaberAPI
    /// </summary>
    /// <param name="song">BeastSaber Song reference</param>
    /// <param name="finished">Function to run after finishing to download</param>
    /// <param name="progressUpdate">Function to run on updating download progress</param>
    void DownloadBeatmapAsync(const BeastSaber::Song& song, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    /// <summary>
    /// For downloading maps gotten from the ScoreSaberAPI
    /// </summary>
    /// <param name="song">ScoreSaber Song reference</param>
    /// <param name="finished">Function to run after finishing to download</param>
    /// <param name="progressUpdate">Function to run on updating download progress</param>
    [[deprecated("Downloading with ScoreSaber::Song will not be supported in the future use ScoreSaber::Leaderboard!")]]
    void DownloadBeatmapAsync(const ScoreSaber::Song& song, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    /// <summary>
    /// For downloading maps gotten from the ScoreSaberAPI
    /// </summary>
    /// <param name="ldb">ScoreSaber Leaderboard reference</param>
    /// <param name="finished">Function to run after finishing to download</param>
    /// <param name="progressUpdate">Function to run on updating download progress</param>
    void DownloadBeatmapAsync(const ScoreSaber::Leaderboard& ldb, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

    void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

    void GetCoverImageAsync(const BeatSaver::BeatmapVersion& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

    void GetCoverImageByHashAsync(std::string hash, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

    // Returns mp3 with previewAudio
    void GetPreviewAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

    extern std::string exception;

}