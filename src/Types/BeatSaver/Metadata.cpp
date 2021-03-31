#include "Types/BeatSaver/Metadata.hpp"

DESERIALIZE_METHOD(BeatSaver, Metadata, 
    DESERIALIZE_VALUE(SongSubName, songSubName, String)
    DESERIALIZE_VALUE(SongAuthorName, songAuthorName, String)
    DESERIALIZE_VALUE(LevelAuthorName, levelAuthorName, String)
    DESERIALIZE_VALUE(Duration, duration, Int)
    DESERIALIZE_VALUE(BPM, bpm, Float)
    DESERIALIZE_VALUE_OPTIONAL(Automapper, automapper, String)
    Difficulties.Deserialize(jsonValue["difficulties"]);

    auto& characteristics = jsonValue["characteristics"];
    if(characteristics.IsArray()) {
        for (auto it = characteristics.Begin(); it != characteristics.End(); ++it) {
            BeatmapCharacteristic characteristic;
            characteristic.Deserialize(*it);
            Characteristics.push_back(characteristic);
        }
    }
)