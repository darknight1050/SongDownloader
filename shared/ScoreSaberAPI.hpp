#pragma once

#include "Types/ScoreSaber/Page.hpp"

#include <string>
#include <optional>

namespace ScoreSaber::API {

    /// <summary>
    /// Retrieves songs that are Trending as determined by ScoreSaber.
    /// </summary>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetTrending(int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves the latest ranked songs.
    /// </summary>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetLatestRanked(int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves the songs that have the most plays as determined by ScoreSaber.
    /// </summary>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetTopPlayed(int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves songs ordered from highest ranked value to lowest.
    /// </summary>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    /// <returns>An optional that can contain a ScoreSaber::Page or a std::nullopt if an error was encountered or no map found</returns>
    std::optional<ScoreSaber::Page> GetTopRanked(int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves songs that are Trending as determined by ScoreSaber.
    /// </summary>
    /// <param name="finished">Function to run after getting the Response</param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    void GetTrendingAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves the latest ranked songs.
    /// </summary>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    void GetLatestRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves the songs that have the most plays as determined by ScoreSaber.
    /// </summary>
    /// <param name="finished"></param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    void GetTopPlayedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex = 0, int amount = 20, bool ranked = false);

    /// <summary>
    /// Retrieves the songs that have the most plays as determined by ScoreSaber.
    /// </summary>
    /// <param name="finished"></param>
    /// <param name="pageIndex">Index of the pages search results default: 0</param>
    /// <param name="amount">Amount of Songs to get default: 20</param>
    /// <param name="ranked">If return value should contain only ranked or be mixed default: false</param>
    void GetTopRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex = 0, int amount = 20, bool ranked = false);


    /// <summary>
    /// String containing any exceptions that were encountered during the request, empty otherwise
    /// </summary>
    extern std::string exception;

}