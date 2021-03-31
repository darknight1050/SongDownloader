#include "BeatSaverAPI.hpp"

#include "CustomLogger.hpp"

#include "Types/BeatSaver/Beatmap.hpp"
#include "WebUtil.hpp"

#include "zip.h"

#include "songloader/shared/API.hpp"

#define BASE_URL std::string("https://beatsaver.com")

namespace BeatSaver::API {

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key) {
        std::string data;
        WebUtil::Get(BASE_URL + "/api/maps/detail/" + key, data);
        rapidjson::Document document;
        document.Parse(data);
        if(document.HasParseError() || !document.IsObject())
            return std::nullopt;
        BeatSaver::Beatmap beatmap;
        beatmap.Deserialize(document.GetObject());
        return beatmap;
    }

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash) {
        std::string data;
        WebUtil::Get(BASE_URL + "/api/maps/by-hash/" + hash, data);
        rapidjson::Document document;
        document.Parse(data);
        if(document.HasParseError() || !document.IsObject())
            return std::nullopt;
        BeatSaver::Beatmap beatmap;
        beatmap.Deserialize(document.GetObject());
        return beatmap;
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