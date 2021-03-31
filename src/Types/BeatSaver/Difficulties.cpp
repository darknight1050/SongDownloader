#include "Types/BeatSaver/Difficulties.hpp"

DESERIALIZE_METHOD(BeatSaver, Difficulties, 
    DESERIALIZE_VALUE(Easy, easy, Bool)
    DESERIALIZE_VALUE(Normal, normal, Bool)
    DESERIALIZE_VALUE(Hard, hard, Bool)
    DESERIALIZE_VALUE(Expert, expert, Bool)
    DESERIALIZE_VALUE(ExpertPlus, expertPlus, Bool)
)