#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, Stats, 
    GETTER_VALUE(int, Plays, "plays");
    GETTER_VALUE(int, Downloads, "downloads");
    GETTER_VALUE(int, UpVotes, "upvotes");
    GETTER_VALUE(int, DownVotes, "downVotes");
    GETTER_VALUE(float, Score, "score");
    ERROR_CHECK
)
