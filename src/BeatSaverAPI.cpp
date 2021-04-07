#include "BeatSaverAPI.hpp"

#include "CustomLogger.hpp"

#include "WebUtil.hpp"

#include "zip.h"

#include "songloader/shared/API.hpp"

#define BASE_URL std::string("https://beatsaver.com")

#define FILE_DOWNLOAD_TIMEOUT 64

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key) {
        auto json = WebUtil::GetJSON(BASE_URL + "/api/maps/detail/" + key);
        if(!json.has_value())
            return std::nullopt;
        BeatSaver::Beatmap beatmap;
        beatmap.Deserialize(json.value().GetObject());
        return beatmap;
    }

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash) {
        auto json = WebUtil::GetJSON(BASE_URL + "/api/maps/by-hash/" + hash);
        if(!json.has_value())
            return std::nullopt;
        BeatSaver::Beatmap beatmap;
        beatmap.Deserialize(json.value().GetObject());
        return beatmap;
    }

    std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex) {
        auto json = WebUtil::GetJSON(BASE_URL + "/api/search/text/" + std::to_string(pageIndex) + "?q=" + query);
        if(!json.has_value())
            return std::nullopt;
        BeatSaver::Page page;
        page.Deserialize(json.value().GetObject());
        return page;
    }

    void DownloadBeatmap(const BeatSaver::Beatmap& beatmap) {
        auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + beatmap.GetKey() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")";
        std::string data;
        WebUtil::Get(BASE_URL + beatmap.GetDownloadURL(), data);
        int args = 2;
        zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char *name, void *arg) -> int {
            return 0;
        }, &args);
    }
    

    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatSaver::Beatmap>)> finished) {
        WebUtil::GetJSONAsync(BASE_URL + "/api/maps/detail/" + key,
            [finished] (long httpCode, bool error, rapidjson::Document& document) {
                if(error) {
                    finished(std::nullopt);
                } else {
                    BeatSaver::Beatmap beatmap;
                    beatmap.Deserialize(document.GetObject());
                    finished(beatmap);
                }
            }
        );
    }

    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished) {
        WebUtil::GetJSONAsync(BASE_URL + "/api/maps/by-hash/" + hash,
            [finished] (long httpCode, bool error, rapidjson::Document& document) {
                if(error) {
                    finished(std::nullopt);
                } else {
                    BeatSaver::Beatmap beatmap;
                    beatmap.Deserialize(document.GetObject());
                    finished(beatmap);
                }
            }
        );
    }

    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished) {
        WebUtil::GetJSONAsync(BASE_URL + "/api/search/text/" + std::to_string(pageIndex) + "?q=" + query,
            [finished] (long httpCode, bool error, rapidjson::Document& document) {
                if(error) {
                    finished(std::nullopt);
                } else {
                    BeatSaver::Page page;
                    page.Deserialize(document.GetObject());
                    finished(page);
                }
            }
        );
    }

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        WebUtil::GetAsync(BASE_URL + beatmap.GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished] (long httpCode, std::string data) {
                auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + beatmap.GetKey() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")";
                int args = 2;
                int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char *name, void *arg) -> int {
                    return 0;
                }, &args);
                finished(statusCode);
            }, progressUpdate
        );
    }

}