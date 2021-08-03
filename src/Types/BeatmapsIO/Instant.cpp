#include "Types/BeatmapsIO/Instant.hpp"

DESERIALIZE_METHOD(BeatmapsIO, Instant,
	DESERIALIZE_VALUE(EpochSeconds, epochSeconds, Int)
	DESERIALIZE_VALUE(NanosecondsOfSecond, nanosecondsOfSecond, Int)
	DESERIALIZE_VALUE(Date, value, String)
)