#include "Types/BeatSaver/BeatmapCharacteristic.hpp"

DESERIALIZE_METHOD(BeatSaver, BeatmapCharacteristic, 
    DESERIALIZE_VALUE(Name, name, String)
    auto& difficulties = jsonValue["difficulties"];
    for(auto it = difficulties.MemberBegin(); it != difficulties.MemberEnd(); it++) {
        if(it->value.IsObject()) {
            BeatmapCharacteristicDifficulty difficulty;
            difficulty.Deserialize(it->value);
            Difficulties[it->name.GetString()] = difficulty;
        } else {
            Difficulties[it->name.GetString()] = std::nullopt;
        }
    }
)