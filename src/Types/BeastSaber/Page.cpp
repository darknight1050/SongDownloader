#include "Types/BeastSaber/Page.hpp"

DESERIALIZE_METHOD(BeastSaber, Page,
    DESERIALIZE_VECTOR(Songs, songs, BeastSaber::Song)
    DESERIALIZE_VALUE_OPTIONAL(Next_page, next_page, Int)
)
