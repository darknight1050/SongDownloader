#include "Types/BeatSaver/User.hpp"

DESERIALIZE_METHOD(BeatSaver, User, 
    DESERIALIZE_VALUE(Id, _id, String)
    DESERIALIZE_VALUE(Username, username, String)
)