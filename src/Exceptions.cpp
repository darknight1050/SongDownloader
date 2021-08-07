#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "Exceptions.hpp"

namespace SongDownloader {
    JsonException::JsonException(const std::string& message) : message(message) {
        error = string_format("Unexpected response, \n%s", message.c_str()).c_str();
    }
}
