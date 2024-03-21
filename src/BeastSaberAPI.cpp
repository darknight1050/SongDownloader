#include "BeastSaberAPI.hpp"

#include "CustomLogger.hpp"

#include "Utils/WebUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "zip.h"

#include "songcore/shared/SongCore.hpp"

#include "Exceptions.hpp"

#define BASE_URL "https://bsaber.com/wp-json/bsaber-api/songs/"
#define FILE_DOWNLOAD_TIMEOUT 64

namespace BeastSaber::API {

    std::string exception;

    std::optional<BeastSaber::Page> CuratorRecommended(int pageIndex, int amount) {
        exception.clear();
        std::string searchPath = fmt::format(BASE_URL "?bookmarked_by=curatorrecommended&page={}&count={}", pageIndex++, amount);
        auto json = WebUtils::GetJSON(searchPath);
        if (!json.has_value())
            return std::nullopt;
        try {
            BeastSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    std::optional<BeastSaber::Page> Bookmarked(std::string username, int pageIndex, int amount) {
        exception.clear();
        std::optional<rapidjson::Document> json;
        auto searchPath = fmt::format(BASE_URL "?bookmarked_by={}&page={}&count={}", username, pageIndex++, amount);
        json = WebUtils::GetJSON(searchPath);
        if (!json.has_value())
            return std::nullopt;
        try {
            BeastSaber::Page page;
            page.Deserialize(json.value().GetObject());
            return page;
        }
        catch (const std::exception& e) {
            LOG_ERROR("{}", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    void CuratorRecommendedAsync(std::function<void(std::optional<BeastSaber::Page>)> finished, int pageIndex, int amount) {
        exception.clear();
        std::string searchPath = fmt::format(BASE_URL "?bookmarked_by=curatorrecommended&page={}&count={}", pageIndex++, amount);
        //LOG_DEBUG("{}", searchPath);
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
                        BeastSaber::Page page;
                        page.Deserialize(document.GetObject());
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

    void BookmarkedAsync(std::string username, std::function<void(std::optional<BeastSaber::Page>)> finished, int pageIndex, int amount) {
        exception.clear();
        std::string searchURL = fmt::format(BASE_URL "?bookmarked_by={}&page={}&count={}", username, pageIndex + 1, amount);
        //LOG_DEBUG("{}", searchURL);
        WebUtils::GetJSONAsync(searchURL,
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
                        BeastSaber::Page page;
                        page.Deserialize(document.GetObject());
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
}
