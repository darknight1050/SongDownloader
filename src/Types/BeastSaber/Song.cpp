#include "Types/BeastSaber/Song.hpp"

DESERIALIZE_METHOD(BeastSaber, Song,
    DESERIALIZE_VALUE(Title, title, String)
    DESERIALIZE_VALUE(Song_key, song_key, String)
    DESERIALIZE_VALUE(Hash, hash, String)
    DESERIALIZE_VALUE(Level_author_name, level_author_name, String)
    DESERIALIZE_VALUE_OPTIONAL(Curated_by, curated_by, String)
)