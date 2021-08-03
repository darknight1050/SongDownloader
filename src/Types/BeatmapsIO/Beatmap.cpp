#include "Types/BeatmapsIO/Beatmap.hpp"

DESERIALIZE_METHOD(BeatmapsIO, Beatmap,
	DESERIALIZE_VALUE(Automapper, automapper, Bool)
	DESERIALIZE_VALUE_OPTIONAL(Curator, curator, String)
	DESERIALIZE_VALUE_OPTIONAL(Description, description, String)
	DESERIALIZE_VALUE(Id, id, Int)
	DESERIALIZE_CLASS(Metadata, metadata)
	DESERIALIZE_VALUE(Name, name, String)
	DESERIALIZE_VALUE(Qualified, qualified, Bool)
	DESERIALIZE_VALUE(Ranked, ranked, Bool)
	//DESERIALIZE_CLASS(Uploaded, uploaded)
	//DESERIALIZE_CLASS(Uploader, uploader)
	DESERIALIZE_VECTOR(Versions, versions, BeatmapVersion)
	DESERIALIZE_CLASS(Stats, stats)
	//DESERIALIZE_VALUE(DirectDownload, directDownload, String)
	//DESERIALIZE_VALUE(DownloadURL, downloadURL, String)
	//DESERIALIZE_VALUE(CoverURL, coverURL, String)
	//DESERIALIZE_VALUE(Hash, hash, String)
	//DESERIALIZE_VALUE(Key, key, String)
)