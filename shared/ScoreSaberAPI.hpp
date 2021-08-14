#pragma once

#include "Types/ScoreSaber/Page.hpp"

#include <string>
#include <optional>

namespace ScoreSaber::API {

    enum class SearchType {
        Trending,
        LatestRanked,
        TopPlayed,
        TopRanked
    };

    /// <summary>
    /// Retrieves songs that are Trending as determined by ScoreSaber.
    /// </summary>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetTrending(bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves the latest ranked songs.
    /// </summary>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetLatestRanked(bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves the songs that have the most plays as determined by ScoreSaber.
    /// </summary>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetTopPlayed(bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves songs ordered from highest ranked value to lowest.
    /// </summary>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetTopRanked(bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves CoverImage as uint8_t bytes
    /// </summary>
    /// <param name="song">Reference to a scoresaber song</param>
    /// <returns>An optional that can contain the bytes for the coverImage or a std::nullopt if an error was encountered or coverImage found</returns>
    std::vector<uint8_t> GetCoverImage(const ScoreSaber::Song& song);

    /// <summary>
    /// Retrieves songs that are Trending as determined by ScoreSaber.
    /// </summary>
    /// <param name="finished">Function to run after getting the Response</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    void GetTrendingAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves the latest ranked songs.
    /// </summary>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    void GetLatestRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves the songs that have the most plays as determined by ScoreSaber.
    /// </summary>
    /// <param name="finished"></param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    void GetTopPlayedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Retrieves the songs that have the most plays as determined by ScoreSaber.
    /// </summary>
    /// <param name="finished"></param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="query">Searh Query</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    void GetTopRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked = false, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Searches TopRanked on ScoreSaber 
    /// </summary>
    /// <param name="query">Searh Query</param>
    /// <param name="finished"></param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    void SearchRankedAsync(std::string query, SearchType list, std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked = false, int pageIndex = 0, int amount = 20);
    
    /// <summary>
    /// Gets CoverImage from ScoreSaber song
    /// </summary>
    /// <param name="song">Reference to a ScoreSaber Song</param>
    /// <param name="finished"></param>
    /// <param name="progressUpdate"></param>
    void GetCoverImageAsync(const ScoreSaber::Song& song, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

    /// <summary>
    /// String containing any exceptions that were encountered during the request, empty otherwise
    /// </summary>
    extern std::string exception;

}