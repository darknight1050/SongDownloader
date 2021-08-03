#include "Types/BeatmapsIO/Page.hpp"

DESERIALIZE_METHOD(BeatmapsIO, Page,
    DESERIALIZE_VECTOR(Docs, docs, Beatmap)
)
