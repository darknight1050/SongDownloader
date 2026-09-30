#pragma once

#include <algorithm>
#include "../TypeMacros.hpp"

namespace ScoreSaber {
    DECLARE_JSON_STRUCT(MapRealm) {
        ERROR_CHECK
        GETTER_VALUE(std::string, LeaderboardStatus, "leaderboardStatus");
        GETTER_VALUE(float, Stars, "stars");
        GETTER_VALUE_OPTIONAL(std::string, RankedAt, "rankedAt");
        GETTER_VALUE_OPTIONAL(std::string, QualifiedAt, "qualifiedAt");
        GETTER_VALUE_OPTIONAL(std::string, LovedAt, "lovedAt");
    };

    DECLARE_JSON_STRUCT(MapLeaderboard) {
        ERROR_CHECK
        GETTER_VALUE(int, Id, "id");
        GETTER_VALUE(int, Difficulty, "difficulty");
        GETTER_VALUE(std::string, GameMode, "gameMode");
        GETTER_VALUE(std::string, RawDifficulty, "rawDifficulty");
        GETTER_VALUE(MapRealm, Realm, "realm");
    };

    DECLARE_JSON_STRUCT(Map) {
        ERROR_CHECK
        GETTER_VALUE(int, Id, "id");
        GETTER_VALUE(std::string, Hash, "hash");
        GETTER_VALUE_OPTIONAL(std::string, Bsid, "bsid");
        GETTER_VALUE(std::string, SongName, "songName");
        GETTER_VALUE(std::string, SongSubName, "songSubName");
        GETTER_VALUE(std::string, SongAuthorName, "songAuthorName");
        GETTER_VALUE(std::string, LevelAuthorName, "levelAuthorName");
        GETTER_VALUE(std::string, CoverUrl, "coverUrl");
        GETTER_VALUE(int, TotalScores, "totalScores");
        GETTER_VALUE(int, DailyScores, "dailyScores");
        GETTER_VALUE(std::vector<MapLeaderboard>, Leaderboards, "leaderboards");

        bool GetRanked() const {
            return std::any_of(_Leaderboards.begin(), _Leaderboards.end(), [](const auto& leaderboard) {
                return leaderboard.GetRealm().GetLeaderboardStatus() == "RANKED";
            });
        }
    };

    DECLARE_JSON_STRUCT(MapsMetadata) {
        ERROR_CHECK
        GETTER_VALUE(int, Page, "page");
        GETTER_VALUE(int, ItemsPerPage, "itemsPerPage");
        GETTER_VALUE(unsigned int, TotalItems, "totalItems");
        GETTER_VALUE(int, TotalPages, "totalPages");
    };

    DECLARE_JSON_STRUCT(Maps) {
        ERROR_CHECK
        GETTER_VALUE(std::vector<Map>, Data, "data");
        GETTER_VALUE(MapsMetadata, Metadata, "metadata");
    };
}
