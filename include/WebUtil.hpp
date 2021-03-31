#pragma once
#include <string_view>

namespace WebUtil {

    long Get(std::string_view url, std::string& val);
    
    long GetToFile(std::string_view url, const std::string& file);

}