#pragma once
#include "../TypeMacros.hpp"
#include "Beatmap.hpp"
#include "User.hpp"

#include <vector>

DECLARE_JSON_CLASS(BeatSaver, Page,
    GETTER_VALUE(std::vector<BeatSaver::Beatmap>, Docs);
    GETTER_VALUE(int, Redirect);
    // Returns UserDetail for a Username with the searchQuery term, or std::nullopt if none is found.
    GETTER_VALUE_OPTIONAL(BeatSaver::UserDetail, User)
)
