#pragma once

#include "ScoreSaberAPI.hpp"
#include <algorithm>
#include <stdexcept>
#include <string_view>

namespace ScoreSaber::API::Detail {
    inline std::string EncodeSearch(std::string_view query) {
        constexpr char hex[] = "0123456789ABCDEF";
        std::string encoded;
        for (unsigned char c : query) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
                encoded += c;
            } else {
                encoded += '%';
                encoded += hex[c >> 4];
                encoded += hex[c & 15];
            }
        }
        return encoded;
    }

    inline std::string MapsURL(ListCategory list, std::string_view query,
                               std::optional<bool> ranked, std::optional<bool> qualified, int pageIndex) {
        std::string sort;
        switch (list) {
            case ListCategory::Trending: sort = "trending"; break;
            case ListCategory::LatestRanked: sort = "latestRankedAt"; ranked = true; break;
            case ListCategory::TopPlayed: sort = "totalScores"; break;
            case ListCategory::TopRanked: sort = "highestStars"; ranked = true; break;
            case ListCategory::Author: sort = "createdAt"; break;
        }
        std::string url = "https://scoresaber.com/api/v2/maps?page=" +
            std::to_string(static_cast<long long>(std::max(0, pageIndex)) + 1) +
            "&limit=20&sortBy=" + sort + "&sortDirection=desc";
        if (!query.empty()) url += "&search=" + EncodeSearch(query);
        if (ranked || qualified) {
            std::string statuses;
            for (std::string_view status : {"UNRANKED", "RANKED", "QUALIFIED", "LOVED"}) {
                if (ranked && *ranked != (status == "RANKED")) continue;
                if (qualified && *qualified != (status == "QUALIFIED")) continue;
                if (!statuses.empty()) statuses += ',';
                statuses += status;
            }
            if (statuses.empty()) throw std::invalid_argument("Conflicting ScoreSaber status filters");
            url += "&status=" + statuses;
        }
        return url;
    }
}
