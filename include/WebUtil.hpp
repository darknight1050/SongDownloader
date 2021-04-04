#pragma once
#include <string_view>

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

namespace WebUtil {

    std::optional<rapidjson::Document> GetJSON(std::string_view url);

    long Get(std::string_view url, std::string& val);
    
    long GetToFile(std::string_view url, const std::string& file);

}