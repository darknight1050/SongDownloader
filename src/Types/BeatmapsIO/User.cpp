#include "Types/BeatmapsIO/User.hpp"

DESERIALIZE_METHOD(BeatmapsIO, User, 
    DESERIALIZE_VALUE(AvatarURL, avatar, String)
	DESERIALIZE_VALUE(Hash, hash, String)
    DESERIALIZE_VALUE(Id, id, Int)
    DESERIALIZE_VALUE(Username, name, String)
)