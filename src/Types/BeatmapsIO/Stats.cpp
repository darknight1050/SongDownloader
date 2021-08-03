#include "Types/BeatmapsIO/Stats.hpp"

DESERIALIZE_METHOD(BeatmapsIO, Stats, 
    DESERIALIZE_VALUE(Downloads, downloads, Int)
    DESERIALIZE_VALUE(Plays, plays, Int)
    DESERIALIZE_VALUE(UpVotes, upVotes, Int)
    DESERIALIZE_VALUE(DownVotes, downVotes, Int)
    DESERIALIZE_VALUE(Score, score, Float)
    //DESERIALIZE_VALUE(Rating, rating, Int)
    //DESERIALIZE_VALUE(Heat, heat, Int)
)