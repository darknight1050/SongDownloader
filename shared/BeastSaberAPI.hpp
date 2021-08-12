#pragma once

#include "Types/BeastSaber/Page.hpp"

#include <string>
#include <optional>

namespace BeastSaber::API {

    /// <summary>
    /// Get maps ordered by upload date.
    /// Note: BeastSaber is slow as hell, so please use the Async methods.
    /// </summary>
    /// <param name="pageIndex">Index of the page you want to request. Default: 0</param>
    /// <param name="amount">Amount of songs to request.</param>
    /// <returns>Returns an std::optional that can either contain the results or std::nullopt if no results were found or an error encountered</returns>
    std::optional<BeastSaber::Page> CuratorRecommended(int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Get songs bookmarked by provided user.
    /// Note: BeastSaber is slow as hell, so please use the Async methods.
    /// </summary>
    /// <param name="username">BeastSaber Username to get bookmarks from.</param>
    /// <param name="pageIndex">Index of the page you want to request. Default: 0</param>
    /// <param name="amount">Amount of songs to request.</param>
    /// <returns>Returns an std::optional that can either contain the results or std::nullopt if no results were found or an error encountered</returns>
    std::optional<BeastSaber::Page> Bookmarked(std::string username, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Get CuratorRecommended Maps
    /// </summary>
    /// <param name="amount">Amount of songs to request</param>
    /// <param name="pageIndex"> Index of the pages search results</param>
    void CuratorRecommendedAsync(std::function<void(std::optional<BeastSaber::Page>)> finished, int pageIndex = 0, int amount = 20);

    /// <summary>
    /// Get songs bookmarked by provided user
    /// Note: BeastSaber is slow as hell, so please use the Async methods
    /// </summary>
    /// <param name="username">BeastSaber Username to get bookmarks from.</param>
    /// <param name="pageIndex">Index of the page you want to request. Default: 0</param>
    /// <param name="amount">Amount of songs to request</param>
    void BookmarkedAsync(std::string username, std::function<void(std::optional<BeastSaber::Page>)> finished, int pageIndex = 0, int amount = 20);


    extern std::string exception;

}