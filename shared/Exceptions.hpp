#pragma once
#include <exception>

namespace SongDownloader {
    class JsonException : public std::exception {
    private:
        const char* error;
        std::string message;
    public:
        explicit JsonException(const std::string& message);
        const char* what() const noexcept override {
            return error;
        }
    };
}
