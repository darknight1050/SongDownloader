#pragma once
#include <string_view>

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

namespace WebUtil {

    std::optional<rapidjson::Document> GetJSON(std::string_view url);

    long Get(std::string_view url, std::string& val);

    void GetAsync(std::string url, std::function<void(long, std::string)> finished, std::function<void(float)> progressUpdate = nullptr);

    void GetJSONAsync(std::string url, std::function<void(long, bool, rapidjson::Document&)> finished);

}