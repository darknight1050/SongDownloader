#pragma once
#include <exception>

namespace SongDownloader {
    enum class Exceptions {
        SiteError,
        NoMember,
        WrongType
    };

    class JsonException : public std::exception {
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
