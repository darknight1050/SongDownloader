#pragma once

#include "Types/BeatmapsIO/Page.hpp"

#include <string>

namespace BeatmapsIO::API {

	std::optional<BeatmapsIO::Beatmap> GetBeatmapByKey(std::string key);

	std::optional<BeatmapsIO::Beatmap> GetBeatmapByBeatSaverKey(std::string key);

	std::optional<BeatmapsIO::Beatmap> GetBeatmapByHash(std::string hash);

	std::optional<BeatmapsIO::Page> SearchPaged(std::string query, int pageIndex);

	bool DownloadBeatmap(const BeatmapsIO::Beatmap& beatmap);

	std::vector<uint8_t> GetCoverImage(const BeatmapsIO::Beatmap& beatmap);

	void GetBeatmapByBeatSaverKeyAsync(std::string key, std::function<void(std::optional<BeatmapsIO::Beatmap>)> finished);

	void GetBeatmapByKeyAsync(std::string key, std::function<void(std::optional<BeatmapsIO::Beatmap>)> finished);

	void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatmapsIO::Beatmap>)> finished);

	void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatmapsIO::Page>)> finished);

	void DownloadBeatmapAsync(const BeatmapsIO::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate = nullptr);

	void GetCoverImageAsync(const BeatmapsIO::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

}