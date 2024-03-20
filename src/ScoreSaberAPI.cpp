#include "ScoreSaberAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songcore/shared/SongCore.hpp"

#include "Exceptions.hpp"

#define BASE_URL "https://scoresaber.com"
#define API_URL_DEPRECATED BASE_URL "/api.php?function=get-leaderboards"
#define API_URL BASE_URL "/api"
#define API_LEADERBOARD API_URL "/leaderboards"
#define FILE_DOWNLOAD_TIMEOUT 64

namespace ScoreSaber::API {

    // TODO: Add search option

    std::string exception;

    std::optional<ScoreSaber::Page> GetTrending(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL_DEPRECATED "&cat=0&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Page> GetLatestRanked(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL_DEPRECATED "&cat=1&limit={}&page={}&ranke={}", amount, ++pageIndex, int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Page> GetTopPlayed(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL_DEPRECATED "&cat=2&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Page> GetTopRanked(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL_DEPRECATED "&cat=3&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Leaderboards> GetList(ListCategory list, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        exception.clear();
        std::stringstream requeststream;
        requeststream << API_LEADERBOARD;
        requeststream << fmt::format("?category={}", static_cast<int>(list));
        requeststream << fmt::format("&page={}", ++pageIndex);
        if (ranked.has_value()) requeststream << fmt::format("&ranked={}", ranked.value());
        if (qualified.has_value()) requeststream << fmt::format("&qualified={}", qualified.value());
        if (unique.has_value()) requeststream << fmt::format("&unique={}", unique.value());
        auto requestURL = requeststream.str();
        LOG_DEBUG("Request: {}", requestURL);
        auto json = WebUtils::GetJSON(requestURL);
        if (!json.has_value())
            return std::nullopt;
        if (json.value().GetObject().HasMember("errorMessage") && json.value().GetObject()["errorMessage"].IsString()) {
            if (strcmp(json.value().GetObject()["errorMessage"].GetString(), "Not Found") != 0) exception = json.value().GetObject()["errorMessage"].GetString();
            return std::nullopt;
        }
        try {
            ScoreSaber::Leaderboards page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }


    std::vector<uint8_t> GetCoverImage(const ScoreSaber::Song& song) {
        std::string data;
        std::string URL = song.GetImage();
        URL.erase(remove(URL.begin(), URL.end(), '\\'), URL.end());
        WebUtils::Get(BASE_URL + URL, FILE_DOWNLOAD_TIMEOUT, data);
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    std::vector<uint8_t> GetCoverImage(const ScoreSaber::Leaderboard& ldb) {
        std::string data;
        WebUtils::Get(ldb.GetCoverImage(), FILE_DOWNLOAD_TIMEOUT, data);
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    void GetTrendingAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL_DEPRECATED "&cat=0&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Page page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByHashAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void GetLatestRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL_DEPRECATED "&cat=1&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Page page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByHashAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void GetTopPlayedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL_DEPRECATED "&cat=2&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Page page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByHashAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void GetTopRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL_DEPRECATED "&cat=3&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Page page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByHashAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void GetListAsync(ListCategory list, std::function<void(std::optional<ScoreSaber::Leaderboards>)> finished, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        exception.clear();
        std::stringstream requeststream;
        requeststream << API_LEADERBOARD;
        requeststream << fmt::format("?category={}", static_cast<int>(list));
        requeststream << fmt::format("&page={}", ++pageIndex);
        if (ranked.has_value()) requeststream << fmt::format("&ranked={}", ranked.value());
        if (qualified.has_value()) requeststream << fmt::format("&qualified={}", qualified.value());
        if (unique.has_value()) requeststream << fmt::format("&unique={}", unique.value());
        auto requestURL = requeststream.str();
        LOG_DEBUG("Request: {}", requestURL);
        WebUtils::GetJSONAsync(requestURL,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else if (document.IsObject() && document.GetObject().HasMember("errorMessage") && document.GetObject()["errorMessage"].IsString()) {
                    if (strcmp(document.GetObject()["errorMessage"].GetString(), "Not Found") != 0) exception = document.GetObject()["errorMessage"].GetString();
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Leaderboards page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByHashAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void SearchSSAsync(std::string query, SearchType list, std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL_DEPRECATED "&cat={}&limit={}&page={}&ranked={}&search={}", static_cast<int>(list), amount, ++pageIndex, int(ranked), query),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Page page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByHashAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void SearchAsync(std::string query, ListCategory list, std::function<void(std::optional<ScoreSaber::Leaderboards>)> finished, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        exception.clear();
        std::stringstream requeststream;
        requeststream << API_LEADERBOARD;
        requeststream << fmt::format("?category={}", static_cast<int>(list));
        requeststream << fmt::format("&page={}", ++pageIndex);
        if (!query.empty()) requeststream << fmt::format("&search={}", query);
        if (ranked.has_value()) requeststream << fmt::format("&ranked={}", ranked.value());
        if (qualified.has_value()) requeststream << fmt::format("&qualified={}", qualified.value());
        if (unique.has_value()) requeststream << fmt::format("&unique={}", unique.value());
        auto requestURL = requeststream.str();
        LOG_DEBUG("URL is: {}", requestURL);
        WebUtils::GetJSONAsync(requestURL,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else if (document.IsObject() && document.GetObject().HasMember("errorMessage") && document.GetObject()["errorMessage"].IsString()) {
                    if (strcmp(document.GetObject()["errorMessage"].GetString(), "Not Found") != 0) exception = document.GetObject()["errorMessage"].GetString();
                    finished(std::nullopt);
                }
                else {
                    try {
                        ScoreSaber::Leaderboards page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        // Convert the document into a string and log/write to file for debug purposes
                        rapidjson::StringBuffer buffer;
                        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        document.Accept(writer);
                        writefile("/sdcard/ModData/SearchSSAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void GetCoverImageAsync(const ScoreSaber::Song& song, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        std::string URL = song.GetImage();
        URL.erase(remove(URL.begin(), URL.end(), '\\'), URL.end());
        WebUtils::GetAsync(BASE_URL + URL, FILE_DOWNLOAD_TIMEOUT,
            [finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

    void GetCoverImageAsync(const ScoreSaber::Leaderboard& ldb, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
    WebUtils::GetAsync(ldb.GetCoverImage(), FILE_DOWNLOAD_TIMEOUT,
            [finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

}
