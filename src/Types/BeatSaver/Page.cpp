#include "Types/BeatSaver/Page.hpp"

DESERIALIZE_METHOD(BeatSaver, Page, 
    auto& docs = jsonValue["docs"];
    if(docs.IsArray()) {
        for (auto it = docs.Begin(); it != docs.End(); ++it) {
            Beatmap doc;
            doc.Deserialize(*it);
            Docs.push_back(doc);
        }
    }

    DESERIALIZE_VALUE(TotalDocs, totalDocs, Int)
    DESERIALIZE_VALUE(LastPage, lastPage, Int)
    DESERIALIZE_VALUE_OPTIONAL(PreviousPage, previousPage, Int)
    DESERIALIZE_VALUE_OPTIONAL(NextPage, nextPage, Int)
)