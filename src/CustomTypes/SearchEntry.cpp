#include "CustomTypes/DownloadSongsSearchViewController.hpp"

#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML-Lite/Creation/Image.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace BSML::Lite;
using namespace TMPro;
using namespace SongDownloader;

SearchEntry::SearchEntry(GameObject* _gameObject, TextMeshProUGUI* _line1Component, TextMeshProUGUI* _line2Component, HMUI::ImageView* _coverImageView, Button* _downloadButton) : gameObject(_gameObject), line1Component(_line1Component), line2Component(_line2Component), coverImageView(_coverImageView), downloadButton(_downloadButton) {
}

#pragma region Map/Song Getters

const BeatSaver::Beatmap& SearchEntry::GetBeatmap() {
    return map;
}

const BeastSaber::Song& SearchEntry::GetSongBeastSaber() {
    return BSsong;
}

const ScoreSaber::Leaderboard& SearchEntry::GetSongScoreSaber() {
    return SSsong;
}

#pragma endregion

#pragma region SetBeatmap

void SearchEntry::SetBeatmap(const BeatSaver::Beatmap& _map) {
    map = _map;
    gameObject->SetActive(true);
    MapType = SearchEntry::MapType::BeatSaver;

    line1Component->SetText(map.GetMetadata().GetSongName(), true);

    if (map.GetRanked()) {
        line1Component->set_color(UnityEngine::Color(1, 0.68f, 0, 1));
    }
    else {
        line1Component->set_color(UnityEngine::Color(1, 1, 1, 1));
    }

    std::string ModsUsed;
    std::vector<BeatSaver::BeatmapDifficulty> Difficulties = map.GetVersions().front().GetDiffs();
    for (BeatSaver::BeatmapDifficulty& elem : Difficulties) {
        if (elem.GetME() && ModsUsed.find("ME") == std::string::npos) ModsUsed += "ME, ";
        if (elem.GetNE() && ModsUsed.find("NE") == std::string::npos) ModsUsed += "NE, ";
        if (elem.GetChroma() && ModsUsed.find("Chroma") == std::string::npos) ModsUsed += "Chroma, ";
        if (elem.GetCinema() && ModsUsed.find("Cinema") == std::string::npos) ModsUsed += "<color=#ADADADFF>Cinema</color>, ";
    }
    if (ModsUsed.ends_with(", ")) ModsUsed.erase(ModsUsed.length() - 2);

    std::stringstream line2;
    line2 << "<size=80%><noparse>" << map.GetMetadata().GetSongAuthorName() << "</noparse>";
    line2 << " " << "<size=90%>[<color=#67c16f><noparse>" << map.GetMetadata().GetLevelAuthorName() << "</noparse></color>]";
    if (!ModsUsed.empty()) {
        line2 << " " << "[" << ModsUsed << "]";
    }

    line2Component->SetText(line2.str(), true);

    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;

    coverImageView->set_enabled(false);
    BeatSaver::API::GetCoverImageAsync(map, [this, currentSearchIndex](std::vector<uint8_t> bytes) {
        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
            BSML::MainThreadScheduler::Schedule([this, currentSearchIndex, bytes] {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    std::vector<uint8_t> data = bytes;

                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
                    Sprite* sprite = ArrayToSprite(spriteArray);
                    coverImageView->set_sprite(sprite);
                    coverImageView->set_enabled(true);
                }
                });
        }
        });
    UpdateDownloadProgress(true);
}

void SearchEntry::SetBeatmap(const BeastSaber::Song& _song) {
    BSsong = _song;
    gameObject->SetActive(true);
    MapType = SearchEntry::MapType::BeastSaber;

    line1Component->SetText(BSsong.GetTitle(), true);
    line1Component->set_color(UnityEngine::Color(1, 1, 1, 1));

    std::stringstream line2;
    line2 << "<size=90%>[<color=#67c16f><noparse>" << BSsong.GetLevel_author_name() << "</noparse></color>]";
    if (BSsong.GetCurated_by().has_value()) {
        line2 << " " << "<color=#FFFFFFFF><size=75%>Curated by:</color> <color=#ADADADFF><size=80%><noparse>" << BSsong.GetCurated_by().value() << "</noparse></color>";
    }

    line2Component->SetText(line2.str(), true);

    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;

    coverImageView->set_enabled(false);
    BeatSaver::API::GetCoverImageByHashAsync(BSsong.GetHash(), [this, currentSearchIndex](std::vector<uint8_t> bytes) {
        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
            BSML::MainThreadScheduler::Schedule([this, currentSearchIndex, bytes] {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    std::vector<uint8_t> data = bytes;

                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
                    Sprite* sprite = ArrayToSprite(spriteArray);
                    coverImageView->set_sprite(sprite);
                    coverImageView->set_enabled(true);
                }
                });
        }
        });
    UpdateDownloadProgress(true);
}

//void SearchEntry::SetBeatmap(const ScoreSaber::Song& _song) {
//    SSsong = _song;
//    gameObject->SetActive(true);
//    MapType = SearchEntry::MapType::ScoreSaber;
//
//    line1Component->SetText(SSsong.GetName());
//
//    if (SSsong.GetRanked() > 0) {
//        line1Component->set_color(UnityEngine::Color(1, 0.68f, 0, 1));
//    }
//    else {
//        line1Component->set_color(UnityEngine::Color(1, 1, 1, 1));
//    }
//
//    line2Component->SetText("<size=80%><noparse>" + SSsong.GetSongAuthorName() + "</noparse>" + " <size=90%>[<color=#67c16f><noparse>" + SSsong.GetLevelAuthorName() + "</noparse></color>]");
//
//    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
//
//    coverImageView->set_enabled(false);
//    ScoreSaber::API::GetCoverImageAsync(SSsong, [this, currentSearchIndex](std::vector<uint8_t> bytes) {
//        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
//            MainThreadScheduler::Schedule([this, currentSearchIndex, bytes] {
//                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
//                    std::vector<uint8_t> data = bytes;
//
//                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
//                    Sprite* sprite = BeatSaberUI::ArrayToSprite(spriteArray);
//                    coverImageView->set_sprite(sprite);
//                    coverImageView->set_enabled(true);
//                }
//                });
//        }
//        });
//    UpdateDownloadProgress(true);
//}

void SearchEntry::SetBeatmap(const ScoreSaber::Leaderboard& _song) {
    SSsong = _song;
    gameObject->SetActive(true);
    MapType = SearchEntry::MapType::ScoreSaber;

    line1Component->SetText(SSsong.GetSongName(), true);

    if (SSsong.GetRanked() > 0) {
        line1Component->set_color(UnityEngine::Color(1, 0.68f, 0, 1));
    }
    else {
        line1Component->set_color(UnityEngine::Color(1, 1, 1, 1));
    }

    line2Component->SetText("<size=80%><noparse>" + SSsong.GetSongAuthorName() + "</noparse>" + " <size=90%>[<color=#67c16f><noparse>" + SSsong.GetLevelAuthorName() + "</noparse></color>]", true);

    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;

    coverImageView->set_enabled(false);
    ScoreSaber::API::GetCoverImageAsync(SSsong, [this, currentSearchIndex](std::vector<uint8_t> bytes) {
        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
            BSML::MainThreadScheduler::Schedule([this, currentSearchIndex, bytes] {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    std::vector<uint8_t> data = bytes;

                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
                    Sprite* sprite = ArrayToSprite(spriteArray);
                    coverImageView->set_sprite(sprite);
                    coverImageView->set_enabled(true);
                }
                });
        }
        });
    UpdateDownloadProgress(true);
}

#pragma endregion

void SearchEntry::UpdateDownloadProgress(bool checkLoaded) {
    if (checkLoaded) {
        downloadProgress = -1.0f;
        std::string hash;
        if (MapType == SearchEntry::MapType::BeatSaver) {
            hash = map.GetVersions().front().GetHash();
        }
        else if (MapType == SearchEntry::MapType::BeastSaber) {
            hash = BSsong.GetHash();
        }
        else {
            hash = SSsong.GetSongHash();
        }
        std::transform(hash.begin(), hash.end(), hash.begin(), toupper);
        for (auto& song : SongCore::API::Loading::GetAllLevels()) {
            if (song->levelID.ends_with(hash)) {
                downloadProgress = 100.0f;
                break;
            }
        }
    }
    if (downloadProgress <= -1.0f) {
        SetButtonText(downloadButton, "Download");
        downloadButton->set_interactable(true);
    }
    else if (downloadProgress >= 100.0f) {
        SetButtonText(downloadButton, "Loaded");
        downloadButton->set_interactable(false);
    }
    else {
        SetButtonText(downloadButton, fmt::format("{}%", (int) downloadProgress));
        downloadButton->set_interactable(false);
    }
}

void SearchEntry::Disable() {
    gameObject->SetActive(false);
}

bool SearchEntry::IsEnabled() {
    return gameObject->get_activeSelf();
}
