#include "BeatSaverAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songcore/shared/SongCore.hpp"

#include "Exceptions.hpp"

#define BASE_URL "beatsaver.com"
#define FALLBACK_URL "beatmaps.io"
#define API_URL "https://api." BASE_URL
#define CDN_URL "https://cdn." BASE_URL "/"
#define FILE_DOWNLOAD_TIMEOUT 64

namespace BeatSaver::API {

    std::string exception;

    std::optional<BeatSaver::Beatmap> GetBeatmapByKey(std::string key) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL "/maps/id/{}", key));
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::Beatmap beatmap;
            beatmap.Deserialize(json.value());
            return beatmap;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL "/maps/hash/{}", hash));
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::Beatmap beatmap;
            beatmap.Deserialize(json.value());
            return beatmap;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeatSaver::UserDetail> GetUserById(int id) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL "/users/id/{}", id));
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::UserDetail user;
            user.Deserialize(json.value());
            return user;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeatSaver::UserDetail> GetUserByName(std::string username) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL "/users/name/{}", username));
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::UserDetail user;
            user.Deserialize(json.value());
            return user;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex, std::string sortOrder, std::string automapper, std::string ME, std::string NE, std::string Chroma) {
        exception.clear();
        std::stringstream searchStream;
        searchStream << API_URL "/search/text/" << pageIndex;
        searchStream << "?sortOrder=" << sortOrder;
        if (!automapper.empty()) searchStream << "&automapper=" << automapper;
        if (!query.empty()) searchStream << "&q=" << query;
        if (!ME.empty()) searchStream << "&me=" << ME;
        if (!NE.empty()) searchStream << "&noodle=" << NE;
        if (!Chroma.empty()) searchStream << "&chroma=" << Chroma;
        auto searchPath = searchStream.str();
        auto json = WebUtils::GetJSON(searchPath);
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::Page page;
            page.Deserialize(json.value());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeatSaver::Page> LatestPaged(bool automapper, std::string after) {
        exception.clear();
        std::optional<rapidjson::Document> json;
        if (after.empty()) {
            json = WebUtils::GetJSON(fmt::format(API_URL "/maps/latest?automapper={}", automapper));
        }
        else {
            json = WebUtils::GetJSON(fmt::format(API_URL "/maps/latest?after={}&automapper={}", after, automapper));
        }
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::Page page;
            page.Deserialize(json.value());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeatSaver::Page> PlaysPaged(int pageIndex) {
        exception.clear();
        auto json = WebUtils::GetJSON(fmt::format(API_URL "/maps/plays/{}", pageIndex));
        if (!json.has_value())
            return std::nullopt;
        try {
            BeatSaver::Page page;
            page.Deserialize(json.value());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    bool DownloadBeatmap(const BeatSaver::Beatmap& beatmap) {
        auto levelsPath = SongCore::API::Loading::GetPreferredCustomLevelPath();
        auto targetFolderName = fmt::format("{} ()[]{{}}%&.:,;=!-_ ({} - {})", beatmap.GetId(), beatmap.GetMetadata().GetSongName(), beatmap.GetMetadata().GetLevelAuthorName());
        auto targetFolder = levelsPath / targetFolderName;
        std::string data;
        WebUtils::Get(beatmap.GetVersions().front().GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT, data);
        int args = 2;
        int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
            return 0;
            }, &args);
        return statusCode;
    }

    std::vector<uint8_t> GetPreview(const BeatSaver::Beatmap& beatmap) {
        std::string data;
        WebUtils::Get(beatmap.GetVersions().front().GetPreviewURL(), FILE_DOWNLOAD_TIMEOUT, data);
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    std::vector<uint8_t> GetCoverImage(const BeatSaver::Beatmap& beatmap) {
        std::string data;
        WebUtils::Get(beatmap.GetVersions().front().GetCoverURL(), FILE_DOWNLOAD_TIMEOUT, data);
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    // This isn't really ideal, but it could be kinda useful to be able to get the coverImage just by hash directly
    std::optional<std::vector<uint8_t>> GetCoverImage(std::string hash) {
        std::string data;
        std::transform(hash.begin(), hash.end(), hash.begin(), tolower);
        WebUtils::Get(fmt::format(CDN_URL "{}.jpg", hash), FILE_DOWNLOAD_TIMEOUT, data);
        if (data.empty()) return std::nullopt;
        std::vector<uint8_t> bytes(data.begin(), data.end());
        return bytes;
    }

    void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatSaver::Beatmap>)> finished) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL "/maps/id/{}", key),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        BeatSaver::Beatmap beatmap;
                        beatmap.Deserialize(document);
                        finished(beatmap);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetBeatmapByKeyAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished) {
        exception.clear();
        WebUtils::GetJSONAsync(fmt::format(API_URL "/maps/hash/{}", hash),
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error || ++document.MemberBegin() == document.MemberEnd()) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        BeatSaver::Beatmap beatmap;
                        beatmap.Deserialize(document);
                        finished(beatmap);
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

    void GetBeatmapByUserIdAsync(int userID, int page, std::function<void(std::optional<BeatSaver::Page>)> finished) {
        exception.clear();
        std::string searchPath = fmt::format(API_URL "/maps/uploader/{}/{}", userID, page);
        LOG_DEBUG("GetBeatmapByUserIdAsync: {}", searchPath);
        WebUtils::GetJSONAsync(searchPath,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    //// Convert the document into a string and log/write to file for debug purposes
                    //rapidjson::StringBuffer buffer;
                    //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    //document.Accept(writer);
                    //writefile("/sdcard/ModData/GetBeatmapByUserIdAsync.json", buffer.GetString());
                    try {
                        BeatSaver::Page page;
                        page.Deserialize(document);
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                    }
                }
            }
        );
    }

    void GetUserByNameAsync(std::string username, std::function<void(std::optional<BeatSaver::UserDetail>)> finished) {
        exception.clear();
        std::string searchPath = fmt::format(API_URL "/users/name/{}", username);
        LOG_DEBUG("GetUserByNameAsync: {}", searchPath);
        WebUtils::GetJSONAsync(searchPath,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    // Convert the document into a string and log/write to file for debug purposes
                    //rapidjson::StringBuffer buffer;
                    //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    //document.Accept(writer);
                    //writefile("/sdcard/ModData/GetUserByNameAsync.json", buffer.GetString());
                    try {
                        BeatSaver::UserDetail user;
                        user.Deserialize(document);
                        finished(user);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                    }
                }
            }
        );
    }


    void GetUserByIdAsync(int id, std::function<void(std::optional<BeatSaver::UserDetail>)> finished) {
        exception.clear();
        auto searchPath = fmt::format(API_URL "/users/id/{}", id);
        LOG_DEBUG("GetUserByIdAsync: {}", searchPath);
        WebUtils::GetJSONAsync(searchPath,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        BeatSaver::UserDetail user;
                        user.Deserialize(document);
                        finished(user);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        //// Convert the document into a string and log/write to file for debug purposes
                        //rapidjson::StringBuffer buffer;
                        //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        //document.Accept(writer);
                        //writefile("/sdcard/ModData/GetUserByIdAsync.json", buffer.GetString());
                    }
                }
            }
        );
    }

    void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished, std::string sortOrder, std::string automapper, std::string ranked, std::string ME, std::string NE, std::string Chroma) {
        exception.clear();
        std::stringstream searchStream;
        searchStream << API_URL "/search/text/" << pageIndex;
        searchStream << "?sortOrder=" << sortOrder;
        if (!query.empty()) searchStream << "&q=" << query;
        if (!automapper.empty()) searchStream << "&automapper=" << automapper;
        if (!ranked.empty()) searchStream << "&ranked=" << ranked;
        if (!ME.empty()) searchStream << "&me=" << ME;
        if (!NE.empty()) searchStream << "&noodle=" << NE;
        if (!Chroma.empty()) searchStream << "&chroma=" << Chroma;
        auto searchPath = searchStream.str();
        LOG_DEBUG("SearchPagedAsync: {}", searchPath);
        WebUtils::GetJSONAsync(searchPath,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    //// Convert the document into a string and log/write to file for debug purposes
                    //rapidjson::StringBuffer buffer;
                    //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    //document.Accept(writer);
                    //LOG_DEBUG("{}", buffer.GetString());
                    //writefile("/sdcard/ModData/SearchQuery.json", buffer.GetString());
                    try {
                        BeatSaver::Page page;
                        page.Deserialize(document);
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        LOG_ERROR("{}", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                    }
                }
            }
        );
    }

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(beatmap.GetVersions().front().GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                auto levelsPath = SongCore::API::Loading::GetPreferredCustomLevelPath();
                auto targetFolderName = FileUtils::FixIlegalName(fmt::format("{} ({} - {})", beatmap.GetId(), beatmap.GetMetadata().GetSongName(), beatmap.GetMetadata().GetLevelAuthorName()));
                auto targetFolder = levelsPath / targetFolderName;
                int args = 2;
                int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
                    return 0;
                    }, &args);
                finished(statusCode);
            }, progressUpdate
        );
    }

    void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, const BeatSaver::BeatmapVersion& beatmapVer, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(beatmapVer.GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                auto levelsPath = SongCore::API::Loading::GetPreferredCustomLevelPath();
                auto targetFolderName = FileUtils::FixIlegalName(fmt::format("{} ({} - {})", beatmap.GetId(), beatmap.GetMetadata().GetSongName(), beatmap.GetMetadata().GetLevelAuthorName()));
                auto targetFolder = levelsPath / targetFolderName;

                int args = 2;
                int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
                    return 0;
                    }, &args);
                finished(statusCode);
            }, progressUpdate
        );
    }

    void DownloadBeatmapAsync(const BeastSaber::Song& song, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        // Probably a deprecated method of downloading maps, but at least will work if map has isn't up to date https://api.beatsaver.com/download/key/1b236
        WebUtils::GetAsync(fmt::format(API_URL "/download/key/{}", song.GetSong_key()), FILE_DOWNLOAD_TIMEOUT,
            [song, finished](long httpCode, std::string data) {
                auto levelsPath = SongCore::API::Loading::GetPreferredCustomLevelPath();
                auto targetFolderName = FileUtils::FixIlegalName(fmt::format("{} ({} - {})", song.GetSong_key(), song.GetTitle(), song.GetLevel_author_name()));
                auto targetFolder = levelsPath / targetFolderName;

                int args = 2;
                int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
                    return 0;
                    }, &args);
                finished(statusCode);
            }, progressUpdate
        );
    }

    void DownloadBeatmapAsync(const ScoreSaber::Song& song, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        // Probably a deprecated method of downloading maps, but at least will work if map has isn't up to date https://api.beatsaver.com/download/key/1b236
        std::string hash = song.GetId();
        std::transform(hash.begin(), hash.end(), hash.begin(), tolower);
        WebUtils::GetAsync(fmt::format(CDN_URL "{}.zip", hash), FILE_DOWNLOAD_TIMEOUT,
            [song, finished](long httpCode, std::string data) {
                auto levelsPath = SongCore::API::Loading::GetPreferredCustomLevelPath();
                auto targetFolderName = FileUtils::FixIlegalName(fmt::format("{} ({} - {})", song.GetUid(), song.GetName(), song.GetLevelAuthorName()));
                auto targetFolder = levelsPath / targetFolderName;

                int args = 2;
                int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
                    return 0;
                    }, &args);
                finished(statusCode);
            }, progressUpdate
        );
    }

    void DownloadBeatmapAsync(const ScoreSaber::Leaderboard& ldb, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
        // Probably a deprecated method of downloading maps, but at least will work if map has isn't up to date https://api.beatsaver.com/download/key/1b236
        GetBeatmapByHashAsync(ldb.GetSongHash(), [finished, progressUpdate](const std::optional<BeatSaver::Beatmap>& beatmap) {
            if (beatmap.has_value())
                DownloadBeatmapAsync(beatmap.value(), finished, progressUpdate);
            else
                finished(false);
            }
        );

        //WebUtils::GetAsync(fmt::format(CDN_URL "{}.zip", hash), FILE_DOWNLOAD_TIMEOUT,
        //    [song, finished](long httpCode, std::string data) {
        //        auto levelsPath = SongCore::API::Loading::GetPreferredCustomLevelPath();
        //        auto targetFolderName = FileUtils::FixIlegalName(fmt::format("{} ({} - {})", song.GetUid(), song.GetName(), song.GetLevelAuthorName()));
        //        auto targetFolder = levelsPath / targetFolderName;
        //
        //        int args = 2;
        //        int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
        //            return 0;
        //            }, &args);
        //        finished(statusCode);
        //    }, progressUpdate
        //);
    }

    void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(beatmap.GetVersions().front().GetCoverURL(), FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

    void GetCoverImageAsync(const BeatSaver::BeatmapVersion& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(beatmap.GetCoverURL(), FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

    void GetCoverImageByHashAsync(std::string hash, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        auto coverURL = fmt::format(CDN_URL "{}.jpg", hash);
        //LOG_DEBUG("CoverURL: {}", coverURL);
        WebUtils::GetAsync(coverURL, FILE_DOWNLOAD_TIMEOUT,
            [hash, finished](long httpCode, std::string data) {
                //LOG_DEBUG("HTTP Code when loading by hash: {}", httpCode);
                if (httpCode != 200) return;
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }

    void GetPreviewAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        WebUtils::GetAsync(beatmap.GetVersions().front().GetPreviewURL(), FILE_DOWNLOAD_TIMEOUT,
            [beatmap, finished](long httpCode, std::string data) {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                finished(bytes);
            }, progressUpdate
        );
    }


}
