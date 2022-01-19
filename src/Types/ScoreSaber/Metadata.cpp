#include "Types/ScoreSaber/Metadata.hpp"

DESERIALIZE_METHOD(ScoreSaber, Metadata_t,
    DESERIALIZE_VALUE(Total, total, Uint)
    DESERIALIZE_VALUE(Page, page, Int)
    DESERIALIZE_VALUE(ItemsPerPage, itemsPerPage, Int)
)