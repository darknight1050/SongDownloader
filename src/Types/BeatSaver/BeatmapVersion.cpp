#include "Types/BeatSaver/BeatmapVersion.hpp"

DESERIALIZE_METHOD(BeatSaver, BeatmapVersion,
    DESERIALIZE_VALUE(CreatedAt, createdAt, String)
    DESERIALIZE_VALUE_OPTIONAL(SageScore, sageScore, Int)
    DESERIALIZE_VECTOR(Diffs, diffs, BeatmapDifficulty)
    DESERIALIZE_VALUE_OPTIONAL(Feedback, feedback, String)
    DESERIALIZE_VALUE(Hash, hash, String)
    DESERIALIZE_VALUE_OPTIONAL(Key, key, String)
    DESERIALIZE_VALUE(State, state, String) // Contains Enum string
    //DESERIALIZE_CLASS(TestplayAt, testplayAt)
    //DESERIALIZE_CLASS(Testplays, testplays) // Not going to add that all in now
    DESERIALIZE_VALUE(DownloadURL, downloadURL, String)
    DESERIALIZE_VALUE(CoverURL, coverURL, String)
    DESERIALIZE_VALUE(PreviewURL, previewURL, String)
)