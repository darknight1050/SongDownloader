#pragma once
#include "../TypeMacros.hpp"
#include "Song.hpp"

namespace ScoreSaber {
    DECLARE_JSON_CLASS(Page,
        ERROR_CHECK
        GETTER_VALUE(std::vector<ScoreSaber::Song>, Songs, "songs");
    )
}
