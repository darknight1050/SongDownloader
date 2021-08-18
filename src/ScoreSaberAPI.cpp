#include "ScoreSaberAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songloader/shared/API.hpp"

#include "Exceptions.hpp"

#define BASE_URL std::string("https://scoresaber.com")
#define API_URL BASE_URL + std::string("/api.php?function=get-leaderboards")
#define FILE_DOWNLOAD_TIMEOUT 64

namespace ScoreSaber::API {

    // TODO: Add search option

    std::string exception;

    std::optional<ScoreSaber::Page> GetTrending(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(API_URL + "&cat=0&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
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
        auto json = WebUtils::GetJSON(API_URL + "&cat=1&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
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
        auto json = WebUtils::GetJSON(API_URL + "&cat=2&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
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
        auto json = WebUtils::GetJSON(API_URL + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)));
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

    std::vector<uint8_t> GetCoverImage(const ScoreSaber::Song& song) {
        std::string data;
        std::string URL = song.GetImage();
        URL.erase(remove(URL.begin(), URL.end(), '\\'), URL.end());
        WebUtils::Get(BASE_URL + URL, FILE_DOWNLOAD_TIMEOUT, data);
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    void GetTrendingAsync(std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=0&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
        WebUtils::GetJSONAsync(API_URL + "&cat=1&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
        WebUtils::GetJSONAsync(API_URL + "&cat=2&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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
        WebUtils::GetJSONAsync(API_URL + "&cat=3&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)),
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

    void SearchSSAsync(std::string query, SearchType list, std::function<void(std::optional<ScoreSaber::Page>)> finished, bool ranked, int pageIndex, int amount) {
        exception.clear();
        WebUtils::GetJSONAsync(API_URL + "&cat=" + std::to_string((int)list) + "&limit=" + std::to_string(amount) + "&page=" + std::to_string(++pageIndex) + "&ranked=" + std::to_string(int(ranked)) + "&search=" + query,
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

    void GetCoverImageAsync(const ScoreSaber::Song& song, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        std::string URL = song.GetImage();
        URL.erase(remove(URL.begin(), URL.end(), '\\'), URL.end());
        WebUtils::GetAsync(BASE_URL + URL, FILE_DOWNLOAD_TIMEOUT,
            [song, finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

}
