#pragma once

#include "Types/ScoreSaber/Maps.hpp"

namespace ScoreSaber::API::Detail {
    // Error responses can omit "error", e.g. HTTP 500 only supplies statusCode/message.
    DECLARE_JSON_STRUCT(MapsError) {
        GETTER_VALUE_OPTIONAL(int, StatusCode, "statusCode");
        GETTER_VALUE_OPTIONAL(std::string, Error, "error");
        GETTER_VALUE_OPTIONAL(std::string, Message, "message");
    };

    inline std::optional<ScoreSaber::Maps> ParseMapsResponse(
        long httpCode, bool invalidJson, rapidjson::Document& document, std::string& error) {
        error.clear();
        const auto requestError = "ScoreSaber request failed (HTTP " + std::to_string(httpCode) + ")";
        if (invalidJson || !document.IsObject()) {
            error = requestError;
            return std::nullopt;
        }
        try {
            MapsError responseError;
            MapsError::Deserialize(&responseError, document);
            if (httpCode < 200 || httpCode >= 300 || responseError.GetError() ||
                responseError.GetStatusCode().value_or(0) >= 400) {
                error = responseError.GetMessage().value_or(responseError.GetError().value_or(requestError));
                if (error.empty()) error = requestError;
                return std::nullopt;
            }
            ScoreSaber::Maps maps;
            ScoreSaber::Maps::Deserialize(&maps, document);
            return maps;
        } catch (const std::exception& e) {
            error = e.what();
            return std::nullopt;
        }
    }
}
