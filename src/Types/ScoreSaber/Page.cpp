#include "Types/ScoreSaber/Page.hpp"

DESERIALIZE_METHOD(ScoreSaber, Page,
    DESERIALIZE_VECTOR(Songs, songs, ScoreSaber::Song)
    //DESERIALIZE_VALUE(Next_page, next_page, Int)
)
