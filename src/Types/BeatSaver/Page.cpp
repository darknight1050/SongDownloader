#include "Types/BeatSaver/Page.hpp"

DESERIALIZE_METHOD(BeatSaver, Page,
    DESERIALIZE_VECTOR(Docs, docs, Beatmap)
    DESERIALIZE_VALUE(Redirect, redirect, Int)
    DESERIALIZE_CLASS_OPTIONAL(User, user)
)
