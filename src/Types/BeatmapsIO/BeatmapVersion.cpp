#include "Types/BeatmapsIO/BeatmapVersion.hpp"

DESERIALIZE_METHOD(BeatmapsIO, BeatmapVersion,
    DESERIALIZE_VALUE(CreatedAt, createdAt, String)
    DESERIALIZE_VALUE(SageScore, sageScore, Int) 
    DESERIALIZE_VECTOR(Diffs, diffs, BeatmapDifficulty)
    DESERIALIZE_VALUE_OPTIONAL(Feedback, feedback, String)
    DESERIALIZE_VALUE(Hash, hash, String)
    DESERIALIZE_VALUE(Key, key, String)
    DESERIALIZE_VALUE(State, state, String) // Contains stringified Enum I think
    //DESERIALIZE_CLASS(TestplayAt, testplayAt)
    //DESERIALIZE_CLASS(Testplays, testplays) // Not going to add that all in now
    DESERIALIZE_VALUE(DownloadURL, downloadURL, String)
    DESERIALIZE_VALUE(CoverURL, coverURL, String)
    DESERIALIZE_VALUE(PreviewURL, previewURL, String)
)