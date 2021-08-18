#include "Types/BeatSaver/User.hpp"
#include "CustomLogger.hpp"

DESERIALIZE_METHOD(BeatSaver, UserDetail,
    DESERIALIZE_VALUE(Id, id, Int)
    DESERIALIZE_VALUE(Username, name, String)
    DESERIALIZE_VALUE_OPTIONAL(Hash, hash, String)
    DESERIALIZE_VALUE(AvatarURL, avatar, String)
    DESERIALIZE_VALUE_OPTIONAL(Testplay, testplay, Bool)
    DESERIALIZE_CLASS_OPTIONAL(Stats, stats, BeatSaver::UserStats)
)