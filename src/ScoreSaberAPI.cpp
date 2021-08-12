#include "ScoreSaberAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songloader/shared/API.hpp"

#include "Exceptions.hpp"

#define BASE_URL std::string("https://scoresaber.com/")
#define API_URL BASE_URL + std::string("api.php?function=get-leaderboards")

namespace ScoreSaber::API {

    std::string exception;

    std::optional<ScoreSaber::Page> GetTrending(int pageIndex, int amount, bool ranked) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL + "&cat=0&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)));
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

    std::optional<ScoreSaber::Page> GetLatestRanked(int pageIndex, int amount, bool ranked) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL + "&cat=1&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)));
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

    std::optional<ScoreSaber::Page> GetTopPlayed(int pageIndex, int amount, bool ranked) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL + "&cat=2&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)));
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

    std::optional<ScoreSaber::Page> GetTopRanked(int pageIndex, int amount, bool ranked) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)));
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

    void GetTrendingAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex, int amount, bool ranked) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=0&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
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

    void GetLatestRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex, int amount, bool ranked) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=1&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
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

    void GetTopPlayedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex, int amount, bool ranked ) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=2&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
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

    void GetTopRankedAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex, int amount, bool ranked) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
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

    void SearchPagedAsync(std::string query, std::function<void(std::optional<ScoreSaber::Page>)> finished, int pageIndex, int amount, bool ranked) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(pageIndex++) + "&ranked=" + std::to_string(int(ranked)) + "&search=" + query,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
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


}
