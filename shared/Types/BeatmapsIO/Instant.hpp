#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatmapsIO, Instant,
    GETTER_VALUE(int, EpochSeconds);
    GETTER_VALUE(int, NanosecondsOfSecond);
    GETTER_VALUE(std::string, Date);
)
