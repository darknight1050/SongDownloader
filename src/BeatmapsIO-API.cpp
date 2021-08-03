#include "BeatmapsIO-API.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songloader/shared/API.hpp"

#define BASE_URL std::string("beatmaps.io")
#define API_URL std::string("https://api.") + BASE_URL
#define CDN_URL std::string("https://cdn.") + BASE_URL + "/"

#define FILE_DOWNLOAD_TIMEOUT 64

namespace BeatmapsIO::API {

    std::optional<BeatmapsIO::Beatmap> GetBeatmapByKey(std::string key) {
        auto json = WebUtils::GetJSON(API_URL + "/maps/id/" + key);
        if (!json.has_value())
            return std::nullopt;
        BeatmapsIO::Beatmap beatmap;
        beatmap.Deserialize(json.value().GetObject());
        return beatmap;
    }

    std::optional<BeatmapsIO::Beatmap> GetBeatmapByBeatSaverKey(std::string key) {
        auto json = WebUtils::GetJSON(API_URL + "/maps/beatsaver/" + key);
        if (!json.has_value())
            return std::nullopt;
        BeatmapsIO::Beatmap beatmap;
        beatmap.Deserialize(json.value().GetObject());
        return beatmap;
    }

    std::optional<BeatmapsIO::Beatmap> GetBeatmapByHash(std::string hash) {
        auto json = WebUtils::GetJSON(API_URL + "/maps/hash/" + hash);
        if (!json.has_value())
            return std::nullopt;
        BeatmapsIO::Beatmap beatmap;
        beatmap.Deserialize(json.value().GetObject());
        return beatmap;
    }

    std::optional<BeatmapsIO::Page> SearchPaged(std::string query, int pageIndex) {
        auto json = WebUtils::GetJSON(API_URL + "/search/text/" + std::to_string(pageIndex) + "?q=" + query);
        if (!json.has_value())
            return std::nullopt;
        BeatmapsIO::Page page;
        page.Deserialize(json.value().GetObject());
        return page;
    }

    bool DownloadBeatmap(const BeatmapsIO::Beatmap& beatmap) {
        auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + beatmap.GetVersions().front().GetKey() + " ()[]{}%&.:,;=!-_ (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")";
        std::string data;
        WebUtils::Get(CDN_URL + beatmap.GetVersions().front().GetHash() + ".zip", FILE_DOWNLOAD_TIMEOUT, data);
        int args = 2;
        int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
            return 0;
            }, &args);
        return statusCode;
    }

    std::vector<uint8_t> GetCoverImage(const BeatmapsIO::Beatmap& beatmap) {
        std::string data;
        WebUtils::Get(CDN_URL + beatmap.GetVersions().front().GetHash() + ".jpg", FILE_DOWNLOAD_TIMEOUT, data);
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    void GetBeatmapByBeatSaverKeyAsync(std::string key, std::function<void(std::optional<BeatmapsIO::Beatmap>)> finished) {
        WebUtils::GetJSONAsync(API_URL + "/maps/beatsaver/" + key,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    BeatmapsIO::Beatmap beatmap;
                    beatmap.Deserialize(document.GetObject());
                    finished(beatmap);
                }
            }
        );
    }

    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatmapsIO::Beatmap>)> finished) {
        WebUtils::GetJSONAsync(API_URL + "/maps/id/" + key,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    BeatmapsIO::Beatmap beatmap;
                    beatmap.Deserialize(document.GetObject());
                    finished(beatmap);
                }
            }
        );
    }

    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatmapsIO::Beatmap>)> finished) {
        WebUtils::GetJSONAsync(API_URL + "/maps/hash/" + hash,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    BeatmapsIO::Beatmap beatmap;
                    beatmap.Deserialize(document.GetObject());
                    finished(beatmap);
                }
            }
        );
    }

    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatmapsIO::Page>)> finished) {
        WebUtils::GetJSONAsync(API_URL + "/search/text/" + std::to_string(pageIndex) + "?q=" + query + "&sortOrder=Latest", // TODO: Let users probably set the sort order
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    BeatmapsIO::Page page;
                    page.Deserialize(document.GetObject());
                    finished(page);
                }
            }
        );
    }

    void DownloadBeatmapAsync(const BeatmapsIO::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(CDN_URL + beatmap.GetVersions().front().GetHash() + ".zip", FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + FileUtils::FixIlegalName(beatmap.GetVersions().front().GetKey() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")");
                int args = 2;
                int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
                    return 0;
                    }, &args);
                finished(statusCode);
            }, progressUpdate
        );
    }

    void GetCoverImageAsync(const BeatmapsIO::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(CDN_URL + beatmap.GetVersions().front().GetHash() + ".jpg", FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

}