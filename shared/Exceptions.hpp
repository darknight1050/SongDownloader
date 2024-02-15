#pragma once

#include <string>
#include <exception>
#include "./_config.h"

namespace SongDownloader {
    enum class SONGDOWNLOADER_EXPORT Exceptions {
        SiteError,
        NoMember,
        WrongType
    };

    class SONGDOWNLOADER_EXPORT JsonException : public std::exception {
    private:
        const char* error;
        std::string message;
    public:
        explicit JsonException(SongDownloader::Exceptions key, const std::string& message);
        const char* what() const noexcept override {
            return error;
        }
    };
}
