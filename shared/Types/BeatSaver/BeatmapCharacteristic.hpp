#pragma once
#include "../TypeMacros.hpp"
#include "BeatmapCharacteristicDifficulty.hpp"
#include <map>

namespace BeatSaver
{
    using BeatmapCharacteristicDifficultiesMapType = std::map<std::string, std::optional<BeatSaver::BeatmapCharacteristicDifficulty>>;
}

DECLARE_CLASS(BeatSaver, BeatmapCharacteristic, 
    GETTER_VALUE(std::string, Name);
    GETTER_VALUE(BeatmapCharacteristicDifficultiesMapType, Difficulties);
)