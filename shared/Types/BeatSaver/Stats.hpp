#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, Stats, 
    GETTER_VALUE(int, Downloads);
    GETTER_VALUE(int, Plays);
    GETTER_VALUE(int, UpVotes);
    GETTER_VALUE(int, DownVotes);
    GETTER_VALUE(float, Rating);
    GETTER_VALUE(float, Heat);
)