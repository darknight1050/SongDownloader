#pragma once
#include "../TypeMacros.hpp"
#include "Beatmap.hpp"
#include "User.hpp"

#include <vector>

DECLARE_JSON_CLASS(BeatSaver, Page,
    GETTER_VALUE(std::vector<BeatSaver::Beatmap>, Docs);
    GETTER_VALUE(int, Redirect);
    GETTER_VALUE(BeatSaver::UserDetail, User)

)
