#include "Types/BeatSaver/Beatmap.hpp"

DESERIALIZE_METHOD(BeatSaver, Beatmap, 
    DESERIALIZE_VALUE(Key, key, String)
    DESERIALIZE_VALUE(Name, name, String)
    DESERIALIZE_VALUE_OPTIONAL(Description, description, String)
    Uploader.Deserialize(jsonValue["uploader"]);
    DESERIALIZE_VALUE(Uploaded, uploaded, String)
    Metadata.Deserialize(jsonValue["metadata"]);
    Stats.Deserialize(jsonValue["stats"]);
    DESERIALIZE_VALUE(DirectDownload, directDownload, String)
    DESERIALIZE_VALUE(DownloadURL, downloadURL, String)
    DESERIALIZE_VALUE(CoverURL, coverURL, String)
    DESERIALIZE_VALUE(Hash, hash, String)
)