#include "Types/ScoreSaber/Song.hpp"

DESERIALIZE_METHOD(ScoreSaber, Song,
    DESERIALIZE_VALUE(Uid, uid, Int)
    DESERIALIZE_VALUE(Id, id, String)
    DESERIALIZE_VALUE(Name, name, String)
    DESERIALIZE_VALUE(SongSubName, songSubName, String)
    DESERIALIZE_VALUE(LevelAuthorName, levelAuthorName, String)
    DESERIALIZE_VALUE(Bpm, bpm, Int)
    DESERIALIZE_VALUE(Diff, diff, String)
    DESERIALIZE_VALUE(Scores, scores, String)
    DESERIALIZE_VALUE(Scores_day, scores_day, Int)
    DESERIALIZE_VALUE(Ranked, ranked, Int)
    DESERIALIZE_VALUE(Stars, stars, Int)
    DESERIALIZE_VALUE(Image, image, String)
)