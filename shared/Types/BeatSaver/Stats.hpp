#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(BeatSaver, Stats, 
    ERROR_CHECK
    GETTER_VALUE(int, Plays, "plays");
    GETTER_VALUE(int, Downloads, "downloads");
    GETTER_VALUE(int, UpVotes, "upvotes");
    GETTER_VALUE(int, DownVotes, "downvotes");
    GETTER_VALUE(float, Score, "score");
)
