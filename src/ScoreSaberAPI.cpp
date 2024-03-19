#include "ScoreSaberAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songcore/shared/SongCore.hpp"

#include "Exceptions.hpp"

#define BASE_URL std::string("https://scoresaber.com")
#define API_URL_DEPRECATED BASE_URL + std::string("/api.php?function=get-leaderboards")
#define API_URL BASE_URL + std::string("/api")
#define API_LEADERBOARD API_URL + std::string("/leaderboards")
#define FILE_DOWNLOAD_TIMEOUT 64

namespace ScoreSaber::API {

    // TODO: Add search option

    std::string exception;

    std::optional<ScoreSaber::Page> GetTrending(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL_DEPRECATED + "&cat=0&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("%s", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Page> GetLatestRanked(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL_DEPRECATED + "&cat=1&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("%s", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Page> GetTopPlayed(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL_DEPRECATED + "&cat=2&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("%s", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Page> GetTopRanked(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL_DEPRECATED + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("%s", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Leaderboards> GetList(ListCategory list, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        exception.clear();
        std::string request = "?category=" + std::to_string(static_cast<int>(list)) + "&page=" + std::to_string(++pageIndex);
        if (ranked.has_value()) request += "&ranked=" + std::string(ranked.value() ? "true" : "false");
        if (qualified.has_value()) request += "&qualified=" + std::string(qualified.value() ? "true" : "false");
        if (unique.has_value()) request += "&unique=" + std::string(unique.value() ? "true" : "false");
        LOG_DEBUG("Request: %s", (API_LEADERBOARD + request).c_str());
        auto json = WebUtils::GetJSON(API_LEADERBOARD + request);
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
            LOG_ERROR("%s", e.what());
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
        WebUtils::GetJSONAsync(API_URL_DEPRECATED + "&cat=0&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
                        LOG_ERROR("%s", e.what());
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
        WebUtils::GetJSONAsync(API_URL_DEPRECATED + "&cat=1&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
                        LOG_ERROR("%s", e.what());
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
        WebUtils::GetJSONAsync(API_URL_DEPRECATED + "&cat=2&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
                        LOG_ERROR("%s", e.what());
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
        WebUtils::GetJSONAsync(API_URL_DEPRECATED + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
                        LOG_ERROR("%s", e.what());
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
        std::string request = "?category=" + std::to_string(static_cast<int>(list)) + "&page=" + std::to_string(++pageIndex);
        if (ranked.has_value()) request += "&ranked=" + std::string(ranked.value() ? "true" : "false");
        if (qualified.has_value()) request += "&qualified=" + std::string(qualified.value() ? "true" : "false");
        if (unique.has_value()) request += "&unique=" + std::string(unique.value() ? "true" : "false");
        WebUtils::GetJSONAsync(API_LEADERBOARD + request,
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
                        LOG_ERROR("%s", e.what());
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
        WebUtils::GetJSONAsync(API_URL_DEPRECATED + "&cat=" + std::to_string(static_cast<int>(list)) + "&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)) + "&search=" + query,
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
                        LOG_ERROR("%s", e.what());
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
        std::string request = "?category=" + std::to_string(static_cast<int>(list)) + "&page=" + std::to_string(++pageIndex);
        if (!query.empty()) request += "&search=" + query;
        if (ranked.has_value()) request += "&ranked=" + std::string(ranked.value() ? "true" : "false");
        if (qualified.has_value()) request += "&qualified=" + std::string(qualified.value() ? "true" : "false");
        if (unique.has_value()) request += "&unique=" + std::string(unique.value() ? "true" : "false");
        LOG_DEBUG("URL is: %s", (API_LEADERBOARD + request).c_str());
        WebUtils::GetJSONAsync(API_LEADERBOARD + request,
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
                        LOG_ERROR("%s", e.what());
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
