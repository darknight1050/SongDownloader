#include "Types/BeatSaver/Beatmap.hpp"

DESERIALIZE_METHOD(BeatSaver, Beatmap,
    DESERIALIZE_VALUE(Id, id, String)
    DESERIALIZE_VALUE(Name, name, String)
    DESERIALIZE_VALUE(Description, description, String)
    DESERIALIZE_CLASS(Uploader, uploader)
    DESERIALIZE_CLASS(Metadata, metadata)
    DESERIALIZE_CLASS(Stats, stats)
    DESERIALIZE_VALUE(Uploaded, uploaded, String)
    DESERIALIZE_VALUE(Automapper, automapper, Bool)
    DESERIALIZE_VALUE(Ranked, ranked, Bool)
    DESERIALIZE_VALUE(Qualified, qualified, Bool)
    DESERIALIZE_VECTOR(Versions, versions, BeatmapVersion)
    DESERIALIZE_VALUE_OPTIONAL(Curator, curator, String)
)