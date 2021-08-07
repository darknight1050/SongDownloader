#include "Types/BeatSaver/Stats.hpp"

DESERIALIZE_METHOD(BeatSaver, Stats, 
    DESERIALIZE_VALUE(Plays, plays, Int)
    DESERIALIZE_VALUE(Downloads, downloads, Int)
    DESERIALIZE_VALUE(UpVotes, upvotes, Int)
    DESERIALIZE_VALUE(DownVotes, downvotes, Int)
    DESERIALIZE_VALUE(Score, score, Float)
)