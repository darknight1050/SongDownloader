#include "ScoreSaberAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/ScoreSaberRequest.hpp"
#include "Utils/ScoreSaberResponse.hpp"

#define BASE_URL "https://scoresaber.com"
#define API_URL_DEPRECATED BASE_URL "/api.php?function=get-leaderboards"
#define FILE_DOWNLOAD_TIMEOUT 64

namespace ScoreSaber::API {

    std::string exception;

    std::optional<ScoreSaber::Page> GetTrending(bool ranked, int pageIndex, int amount) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL_DEPRECATED "&cat=0&limit={}&page={}&ranked={}", amount, ++pageIndex, int(ranked)));
        if (!json.has_value())
            return std::nullopt;
        try {
            ScoreSaber::Page page;
            Page::Deserialize(&page, json.value());
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
            Page::Deserialize(&page, json.value());
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
            Page::Deserialize(&page, json.value());
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
            Page::Deserialize(&page, json.value());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<ScoreSaber::Maps> GetMaps(ListCategory list, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        exception.clear();
        try {
            auto url = Detail::MapsURL(list, {}, ranked, qualified, pageIndex);
            LOG_DEBUG("Request: {}", url);
            std::string data;
            const auto httpCode = WebUtils::Get(url, data);
            rapidjson::Document document;
            document.Parse(data);
            return Detail::ParseMapsResponse(httpCode, document.HasParseError(), document, exception);
        } catch (const std::exception& e) {
            exception = e.what();
            return std::nullopt;
        }
    }

    std::vector<uint8_t> GetCoverImage(const ScoreSaber::Map& map) {
        std::string data;
        const auto httpCode = WebUtils::Get(map.GetCoverUrl(), FILE_DOWNLOAD_TIMEOUT, data);
        if (httpCode < 200 || httpCode >= 300) return {};
        return {data.begin(), data.end()};
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
                        Page::Deserialize(&page, document);
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
                        Page::Deserialize(&page, document);
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
                        Page::Deserialize(&page, document);
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
                        Page::Deserialize(&page, document);
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

    void GetListAsync(ListCategory list, std::function<void(std::optional<ScoreSaber::Maps>)> finished, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        SearchAsync({}, list, finished, ranked, qualified, unique, pageIndex);
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
                        Page::Deserialize(&page, document);
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

    void SearchAsync(std::string query, ListCategory list, std::function<void(std::optional<ScoreSaber::Maps>)> finished, std::optional<bool> ranked, std::optional<bool> qualified, std::optional<bool> unique, int pageIndex) {
        exception.clear();
        std::string url;
        try {
            url = Detail::MapsURL(list, query, ranked, qualified, pageIndex);
        } catch (const std::exception& e) {
            exception = e.what();
            finished(std::nullopt);
            return;
        }
        LOG_DEBUG("Request: {}", url);
        WebUtils::GetJSONAsync(url,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                auto maps = Detail::ParseMapsResponse(httpCode, error, document, exception);
                if (!maps) LOG_ERROR("{}", exception);
                finished(maps);
            }
        );
    }

    void GetCoverImageAsync(const ScoreSaber::Map& map, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(map.GetCoverUrl(), FILE_DOWNLOAD_TIMEOUT,
            [finished](long httpCode, std::string data) {
                if (httpCode < 200 || httpCode >= 300) {
                    finished({});
                    return;
                }
                finished(std::vector<uint8_t>(data.begin(), data.end()));
            }, progressUpdate
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
