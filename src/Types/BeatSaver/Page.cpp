#include "Types/BeatSaver/Page.hpp"

DESERIALIZE_METHOD(BeatSaver, Page,
    DESERIALIZE_VECTOR(Docs, docs, Beatmap)
    DESERIALIZE_CLASS_OPTIONAL(User, user)
)
