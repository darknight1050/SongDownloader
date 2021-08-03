#include "Types/BeatmapsIO/BeatmapVersion.hpp"

DESERIALIZE_METHOD(BeatmapsIO, BeatmapVersion,
	//DESERIALIZE_CLASS(CreatedAt, createdAt)
	//DESERIALIZE_CLASS(Diffs, diffs)
	DESERIALIZE_VALUE_OPTIONAL(Feedback, feedback, String)
	DESERIALIZE_VALUE(Hash, hash, String)
	DESERIALIZE_VALUE(Key, key, String)
	//DESERIALIZE_VALUE(SageScore, sageScore, ?) // Idk, what Type "Short" would be
	//DESERIALIZE_VALUE(State, state, String) // Contains stringified Enum I think
	//DESERIALIZE_CLASS(TestplayAt, testplayAt)
	//DESERIALIZE_CLASS(Testplays, testplays) // Not going to add that all in now
)