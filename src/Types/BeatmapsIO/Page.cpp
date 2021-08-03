#include "Types/BeatmapsIO/Page.hpp"

DESERIALIZE_METHOD(BeatmapsIO, Page,
    DESERIALIZE_VECTOR(Docs, docs, Beatmap)
    //DESERIALIZE_VALUE(TotalDocs, totalDocs, Int)
    //DESERIALIZE_VALUE(LastPage, lastPage, Int)
    //DESERIALIZE_VALUE_OPTIONAL(PreviousPage, previousPage, Int)
    //DESERIALIZE_VALUE_OPTIONAL(NextPage, nextPage, Int)
)