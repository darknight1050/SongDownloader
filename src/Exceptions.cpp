#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "Exceptions.hpp"

namespace SongDownloader {
    JsonException::JsonException(SongDownloader::Exceptions key, const std::string& message) : message(message) {
        switch (key) {
        //case Exceptions::SiteError:
        //    error = message.c_str();
        //    break;
        case Exceptions::NoMember:
            error = string_format("Unexpected Response, expected member in non-optional jsonValue but got false on HasMember!\n JSON Value: %s", message.c_str()).c_str();
            break;
        case Exceptions::WrongType:
            error = string_format("Unexpected Response, expected type in non-optional jsonValue does not match!\n JSON Value: %s", message.c_str()).c_str();
            break;
        }
    }
}
