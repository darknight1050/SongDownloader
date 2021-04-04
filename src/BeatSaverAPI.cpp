#include "BeatSaverAPI.hpp"

#include "CustomLogger.hpp"

#include "WebUtil.hpp"

#include "zip.h"

#include "songloader/shared/API.hpp"

#define BASE_URL std::string("https://beatsaver.com")

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

    std::optional<BeatSaver::Page> SearchPage(std::string query, int pageIndex) {
        auto json = WebUtil::GetJSON(BASE_URL + "/api/search/text/" + std::to_string(pageIndex) + "?q=" + query);
        if(!json.has_value())
            return std::nullopt;
        BeatSaver::Page page;
        page.Deserialize(json.value().GetObject());
        return page;
    }

    void DownloadBeatmap(const BeatSaver::Beatmap& beatmap) {
        auto tmpFile = RuntimeSongLoader::API::GetCustomLevelsPath() + beatmap.GetKey() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")" + ".zip";
        auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + beatmap.GetKey() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")";
        WebUtil::GetToFile(BASE_URL + beatmap.GetDownloadURL(), tmpFile);
        int args = 2;
        zip_extract(tmpFile.c_str(), targetFolder.c_str(), +[](const char *name, void *arg) -> int {
            return 0;
        }, &args);
        deletefile(tmpFile);
        RuntimeSongLoader::API::RefreshSongs(false);
    }

}