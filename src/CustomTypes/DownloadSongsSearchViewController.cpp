#include "CustomTypes/DownloadSongsSearchViewController.hpp"

#include "CustomLogger.hpp"
#include "ModConfig.hpp"

#include "BeatSaverAPI.hpp"
#include "BeastSaberAPI.hpp"

#include "songloader/shared/API.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Resources.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/Touchable.hpp"
#include "System/Action.hpp"

#include "GlobalNamespace/LevelBar.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/QuestUI.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
using namespace TMPro;
using namespace GlobalNamespace;

using namespace SongDownloader;

namespace SongDownloader {
    DownloadSongsSearchViewController* searchViewController;
}

int SearchEntry::spriteCount = 0;

SearchEntry::SearchEntry(GameObject* _gameObject, TextMeshProUGUI* _line1Component, TextMeshProUGUI* _line2Component, HMUI::ImageView* _coverImageView, Button* _downloadButton) : gameObject(_gameObject), line1Component(_line1Component), line2Component(_line2Component), coverImageView(_coverImageView), downloadButton(_downloadButton) {
}

const BeatSaver::Beatmap& SearchEntry::GetBeatmap() {
    return map;
}

const BeastSaber::Song& SearchEntry::GetSong() {
    return song;
}

void SearchEntry::SetBeatmap(const BeatSaver::Beatmap& _map) {
    map = _map;
    gameObject->SetActive(true);
    IsBeatSaverBeatmap = true;

    line1Component->SetText(il2cpp_utils::newcsstr(map.GetMetadata().GetSongName()));

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
    if (ModsUsed.empty()) {
        line2Component->SetText(il2cpp_utils::newcsstr("<size=80%><noparse>" + map.GetMetadata().GetSongAuthorName() + "</noparse>" + " <size=90%>[<color=#67c16f><noparse>" + map.GetMetadata().GetLevelAuthorName() + "</noparse></color>]"));
    }
    else {
        line2Component->SetText(il2cpp_utils::newcsstr("<size=80%><noparse>" + map.GetMetadata().GetSongAuthorName() + "</noparse>" + " <size=90%>[<color=#67c16f><noparse>" + map.GetMetadata().GetLevelAuthorName() + "</noparse></color>] [" + ModsUsed + "]"));
    }

    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
    
    coverImageView->set_enabled(false);
    BeatSaver::API::GetCoverImageAsync(map, [this, currentSearchIndex](std::vector<uint8_t> bytes) {
        if(currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
            MainThreadScheduler::Schedule([this, currentSearchIndex, bytes] {
                if(currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    std::vector<uint8_t> data = bytes;
                    
                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
                    Sprite* sprite = BeatSaberUI::ArrayToSprite(spriteArray);
                    coverImageView->set_sprite(sprite);
                    coverImageView->set_enabled(true);
                    SearchEntry::spriteCount++;
                }
            });
        }
    });
    UpdateDownloadProgress(true);
}

void SearchEntry::SetBeatmap(const BeastSaber::Song& _song) {
    song = _song;
    gameObject->SetActive(true);
    IsBeatSaverBeatmap = false;

    line1Component->SetText(il2cpp_utils::newcsstr(song.GetTitle()));

    line1Component->set_color(UnityEngine::Color(1, 1, 1, 1));

    if (song.GetCurated_by().has_value()) {
        line2Component->SetText(il2cpp_utils::newcsstr("<size=90%>[<color=#67c16f><noparse>" + song.GetLevel_author_name() + "</noparse></color>] " + "<color=#FFFFFFFF><size=75%>Curated by:</color> <color=#ADADADFF><size=80%><noparse>" + song.GetCurated_by().value() + "</noparse></color>"));
    }
    else {
        line2Component->SetText(il2cpp_utils::newcsstr("<size=90%>[<color=#67c16f><noparse>" + song.GetLevel_author_name() + "</noparse></color>]"));
    }

    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;

    coverImageView->set_enabled(false);
    BeatSaver::API::GetCoverImageByHashAsync(song.GetHash(), [this, currentSearchIndex](std::vector<uint8_t> bytes) {
        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
            MainThreadScheduler::Schedule([this, currentSearchIndex, bytes] {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    std::vector<uint8_t> data = bytes;

                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
                    Sprite* sprite = BeatSaberUI::ArrayToSprite(spriteArray);
                    coverImageView->set_sprite(sprite);
                    coverImageView->set_enabled(true);
                    SearchEntry::spriteCount++;
                }
                });
        }
        });
    UpdateDownloadProgress(true);
}

void SearchEntry::UpdateDownloadProgress(bool checkLoaded) {
    if(checkLoaded) {
        downloadProgress = -1.0f;
        std::string hash;
        if (IsBeatSaverBeatmap) {
            hash = map.GetVersions().front().GetHash();
        }
        else {
            hash = song.GetHash();
        }
        std::transform(hash.begin(), hash.end(), hash.begin(), toupper);
        for(auto& song : RuntimeSongLoader::API::GetLoadedSongs()) {
            if(to_utf8(csstrtostr(song->levelID)).ends_with(hash)) {
                downloadProgress = 100.0f;
                break;
            }
        }
    }
    if(downloadProgress <= -1.0f) {
        BeatSaberUI::SetButtonText(downloadButton, "Download");
        downloadButton->set_interactable(true);
    } else if(downloadProgress >= 100.0f) {
        BeatSaberUI::SetButtonText(downloadButton, "Loaded");
        downloadButton->set_interactable(false);
    } else {
        BeatSaberUI::SetButtonText(downloadButton, string_format("%.0f%%", downloadProgress));
        downloadButton->set_interactable(false);
    }
}

void SearchEntry::Disable() {
    gameObject->SetActive(false);
}

bool SearchEntry::IsEnabled() {
    return gameObject->get_activeSelf();
}

DEFINE_TYPE(SongDownloader, DownloadSongsSearchViewController);

int DownloadSongsSearchViewController::searchIndex = 0;

std::string DownloadSongsSearchViewController::SearchQuery = "";

void DownloadSongsSearchViewController::CreateEntries(Transform* parent) {
    HorizontalLayoutGroup* levelBarLayout = BeatSaberUI::CreateHorizontalLayoutGroup(parent);
    GameObject* prefab = levelBarLayout->get_gameObject();
    levelBarLayout->set_childControlWidth(false);
    levelBarLayout->set_childForceExpandWidth(true);

    LayoutElement* levelBarLayoutElement = levelBarLayout->GetComponent<LayoutElement*>();
    levelBarLayoutElement->set_minHeight(15.0f);
    levelBarLayoutElement->set_minWidth(90.0f);

    GameObject* existingLevelBar = Resources::FindObjectsOfTypeAll<LevelBar*>()->values[0]->get_gameObject();
    GameObject* levelBarGameObject = UnityEngine::GameObject::Instantiate(existingLevelBar, levelBarLayout->get_transform());
    auto levelBarTransform = levelBarGameObject->get_transform();
    LevelBar* levelBar = levelBarGameObject->GetComponent<LevelBar*>();
    auto songNameTextComponent = levelBar->songNameText;
    songNameTextComponent->set_fontSize(4.2f);
    songNameTextComponent->set_overflowMode(TextOverflowModes::Ellipsis);
    songNameTextComponent->set_margin(Vector4(-2.0f, 0.0f, 9.0f, 0.0f));

    auto authorNameTextComponent = levelBar->authorNameText;
    authorNameTextComponent->set_richText(true);
    authorNameTextComponent->set_fontSize(3.2f);
    authorNameTextComponent->set_overflowMode(TextOverflowModes::Ellipsis);
    authorNameTextComponent->set_margin(Vector4(-2.0f, 0.0f, 9.0f, 0.0f));

    static auto bgName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BG");
    Transform* backgroundTransform = levelBarTransform->Find(bgName);
    backgroundTransform->set_localScale(Vector3(1.5f, 1.0f, 1.0f));

    static auto songArtworkName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("SongArtwork");
    levelBar->get_transform()->Find(songArtworkName)->set_localScale(Vector3(0.96f, 0.96f, 0.96f));

    Button* prefabDownloadButton = BeatSaberUI::CreateUIButton(levelBarTransform, "Download");
    prefab->SetActive(false);

    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
        auto copy = Object::Instantiate(prefab, parent);
        LevelBar* copyLevelBar = copy->GetComponentInChildren<LevelBar*>();
        Button* downloadButton = copy->GetComponentInChildren<Button*>();
        downloadButton->get_transform()->set_localPosition(Vector3(36.5f, -7.0f, 0.0f));
        Transform* artworkTransform = copyLevelBar->get_transform()->Find(songArtworkName);
        HMUI::ImageView* artwork = artworkTransform->GetComponent<HMUI::ImageView*>();

        searchEntries[i] = SearchEntry(copy, copyLevelBar->songNameText, copyLevelBar->authorNameText, artwork, downloadButton);
        auto entry = &searchEntries[i];
        downloadButton->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityAction*>(classof(UnityAction*), 
            (std::function<void()>) [this, entry] {
                if (entry->IsBeatSaverBeatmap) {
                    auto hash = entry->GetBeatmap().GetVersions().front().GetHash();
                    BeatSaver::API::DownloadBeatmapAsync(entry->GetBeatmap(),
                        [this](bool error) {
                            if (!error) {
                                QuestUI::MainThreadScheduler::Schedule(
                                    [] {
                                        RuntimeSongLoader::API::RefreshSongs(false);
                                    }
                                );
                            }
                        },
                        [entry, hash](float percentage) {
                            if (entry->GetBeatmap().GetVersions().front().GetHash() == hash) {
                                entry->downloadProgress = percentage;
                                QuestUI::MainThreadScheduler::Schedule(
                                    [entry] {
                                        entry->UpdateDownloadProgress(false);
                                    }
                                );
                            }
                        }
                    );
                }
                else {
                    auto hash = entry->GetSong().GetHash();
                    BeatSaver::API::DownloadBeatmapAsync(entry->GetSong(),
                        [this](bool error) {
                            if (!error) {
                                QuestUI::MainThreadScheduler::Schedule(
                                    [] {
                                        RuntimeSongLoader::API::RefreshSongs(false);
                                    }
                                );
                            }
                        },
                        [entry, hash](float percentage) {
                            if (entry->GetSong().GetHash() == hash) {
                                entry->downloadProgress = percentage;
                                QuestUI::MainThreadScheduler::Schedule(
                                    [entry] {
                                        entry->UpdateDownloadProgress(false);
                                    }
                                );
                            }
                        }
                    );
                }
            }
        ));
    }
    Object::Destroy(prefab);
}

void DownloadSongsSearchViewController::SearchKey(int currentSearchIndex) {
    if (!DownloadSongsSearchViewController::SearchQuery.empty()) {
        BeatSaver::API::GetBeatmapByKeyAsync(DownloadSongsSearchViewController::SearchQuery,
            [this, currentSearchIndex](std::optional<BeatSaver::Beatmap> beatmap) {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    QuestUI::MainThreadScheduler::Schedule(
                        [this, currentSearchIndex, beatmap] {
                            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                if (beatmap.has_value()) {
                                    loadingControl->Hide();
                                    searchEntries[0].SetBeatmap(beatmap.value());
                                    for (int i = 1; i < ENTRIES_PER_PAGE; i++) {
                                        searchViewController->searchEntries[i].Disable();
                                    }
                                }
                                else {
                                    if (!BeatSaver::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(BeatSaver::API::exception), true);
                                    else loadingControl->ShowText(il2cpp_utils::newcsstr("No Song Found for Key!"), true);
                                }
                            }
                        }
                    );
                }
            }
        );
    }
    else loadingControl->ShowText(il2cpp_utils::newcsstr("Please type in a key!"), false);
}

void DownloadSongsSearchViewController::SearchSongs(int currentSearchIndex) {
    BeatSaver::API::SearchPagedAsync(DownloadSongsSearchViewController::SearchQuery, 0,
        [this, currentSearchIndex](std::optional<BeatSaver::Page> page) {
            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, page] {
                        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                            if (page.has_value() && !page.value().GetDocs().empty()) {
                                auto maps = page.value().GetDocs();
                                auto mapsSize = maps.size();
                                int mapIndex = 0;
                                for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                    auto& searchEntry = searchEntries[i];
                                    if (mapIndex < mapsSize) {
                                        loadingControl->Hide();
                                        auto& map = maps.at(mapIndex);
                                        searchEntry.SetBeatmap(map);
                                    }
                                    else {
                                        searchEntry.Disable();
                                    }
                                    mapIndex++;
                                }
                            }
                            else {
                                if (!BeatSaver::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(BeatSaver::API::exception), true);
                                else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found!"), true);
                            }
                        }
                    }
                );
            }
        },
        getModConfig().SortOrder.GetValue(),
            getModConfig().AutoMapper.GetValue(),
            getModConfig().Ranked.GetValue(),
            getModConfig().ME.GetValue(),
            getModConfig().NE.GetValue(),
            getModConfig().Chroma.GetValue());
}

void DownloadSongsSearchViewController::SearchUser(int currentSearchIndex) {
    if (!DownloadSongsSearchViewController::SearchQuery.empty()) {
        BeatSaver::API::SearchPagedAsync(DownloadSongsSearchViewController::SearchQuery, 0,
            [this, currentSearchIndex](std::optional<BeatSaver::Page> UserPage) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, UserPage]() {
                        if (UserPage.has_value() && UserPage.value().GetUser().has_value()) {
                            BeatSaver::API::GetBeatmapByUserIdAsync(UserPage.value().GetUser().value().GetId(), 0,
                                [this, currentSearchIndex](std::optional<BeatSaver::Page> page) {
                                    QuestUI::MainThreadScheduler::Schedule(
                                        [this, currentSearchIndex, page]() {
                                            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                                if (page.has_value() && !page.value().GetDocs().empty()) {
                                                    auto maps = page.value().GetDocs();
                                                    auto mapsSize = maps.size();
                                                    int mapIndex = 0;
                                                    for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                                        auto& searchEntry = searchEntries[i];
                                                        if (mapIndex < mapsSize) {
                                                            loadingControl->Hide();
                                                            auto& map = maps.at(mapIndex);
                                                            searchEntry.SetBeatmap(map);
                                                        }
                                                        else {
                                                            searchEntry.Disable();
                                                        }
                                                        mapIndex++;
                                                    }
                                                }
                                                else {
                                                    if (!BeatSaver::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(BeatSaver::API::exception), true);
                                                    else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found for given User!"), true);
                                                }
                                            }
                                        }
                                    );
                                }
                            );
                        }
                        else {
                            if (!BeatSaver::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(BeatSaver::API::exception), true);
                            else loadingControl->ShowText(il2cpp_utils::newcsstr("No User Found!"), true);
                        }
                    }
                );
            }
        );
    }
    else loadingControl->ShowText(il2cpp_utils::newcsstr("Please type in a Username!"), false);
}

void DownloadSongsSearchViewController::GetCuratorRecommended(int currentSearchIndex) {
        BeastSaber::API::CuratorRecommendedAsync(
            [this, currentSearchIndex](std::optional<BeastSaber::Page> page) {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    QuestUI::MainThreadScheduler::Schedule(
                        [this, currentSearchIndex, page] {
                            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                if (page.has_value()) {
                                    auto songs = page.value().GetSongs();
                                    auto songsSize = songs.size();
                                    int songIndex = 0;
                                    for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                        auto& searchEntry = searchEntries[i];
                                        if (songIndex < songsSize) {
                                            loadingControl->Hide();
                                            auto& song = songs.at(songIndex);
                                            searchEntry.SetBeatmap(song);
                                        }
                                        else {
                                            searchEntry.Disable();
                                        }
                                        songIndex++;
                                    }
                                }
                                else {
                                    if (!BeastSaber::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(BeastSaber::API::exception), true);
                                    else loadingControl->ShowText(il2cpp_utils::newcsstr("Damn, Curators didn't recommend anything!"), true);
                                }
                            }
                        }
                    );
                }
            }
        , 0);
}

void DownloadSongsSearchViewController::GetBookmarks(int currentSearchIndex) {
    if (!DownloadSongsSearchViewController::SearchQuery.empty()) {
        BeastSaber::API::BookmarkedAsync(DownloadSongsSearchViewController::SearchQuery,
            [this, currentSearchIndex](std::optional<BeastSaber::Page> page) {
                if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                    QuestUI::MainThreadScheduler::Schedule(
                        [this, currentSearchIndex, page] {
                            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                if (page.has_value() && !page.value().GetSongs().empty()) {
                                    auto songs = page.value().GetSongs();
                                    auto songsSize = songs.size();
                                    int songIndex = 0;
                                    for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                        auto& searchEntry = searchEntries[i];
                                        if (songIndex < songsSize) {
                                            loadingControl->Hide();
                                            auto& song = songs.at(songIndex);
                                            searchEntry.SetBeatmap(song);
                                        }
                                        else {
                                            searchEntry.Disable();
                                        }
                                        songIndex++;
                                    }
                                }
                                else {
                                    if (!BeastSaber::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(BeastSaber::API::exception), true);
                                    else loadingControl->ShowText(il2cpp_utils::newcsstr("No bookmarks found!"), true);
                                }
                            }
                        }
                    );
                }
            }, 
        0);
    }
    else loadingControl->ShowText(il2cpp_utils::newcsstr("Please type in a Username!"), false);
}

void DownloadSongsSearchViewController::Search() {
    for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
        searchViewController->searchEntries[i].Disable();
    }
    searchViewController->loadingControl->ShowLoading(il2cpp_utils::newcsstr("Loading..."));
    DownloadSongsSearchViewController::searchIndex++;
    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
    searchViewController->SearchField->get_gameObject()->SetActive(true);
    if (getModConfig().SearchType.GetValue() == "Key") {
        searchViewController->SearchKey(currentSearchIndex);
    }
    else if (getModConfig().SearchType.GetValue() == "Search") {
        searchViewController->SearchSongs(currentSearchIndex);
    }
    else if (getModConfig().SearchType.GetValue() == "User") {
        searchViewController->SearchUser(currentSearchIndex);
    }
    else if (getModConfig().SearchType.GetValue() == "Curator Recommended") {
        searchViewController->SearchField->get_gameObject()->SetActive(false);
        searchViewController->GetCuratorRecommended(currentSearchIndex);
    }
    else if (getModConfig().SearchType.GetValue() == "Bookmarks") {
        searchViewController->GetBookmarks(currentSearchIndex);
    }
    if (SearchEntry::spriteCount > MAX_SPRITES) {
        SearchEntry::spriteCount = 0;
        Resources::UnloadUnusedAssets();
    }

}

void DownloadSongsSearchViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        searchViewController = this;
        get_gameObject()->AddComponent<Touchable*>();

        SearchField = BeatSaberUI::CreateStringSetting(get_transform(), "Search", "", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector3(0.0f, -38.0f, 0.0f),
            [this](std::string value) {
                DownloadSongsSearchViewController::SearchQuery = value;
                if (getModConfig().SearchType.GetValue() == "Bookmarks") getModConfig().BookmarkUsername.SetValue(value);
                Search();
            }
        );
        if (getModConfig().SearchType.GetValue() == "Bookmarks") {
            DownloadSongsSearchViewController::SearchQuery = getModConfig().BookmarkUsername.GetValue();
            searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(getModConfig().BookmarkUsername.GetValue()));
        }

        auto container = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -4.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -8.0f));
        CreateEntries(container->get_transform());

        // LoadingControl has to be added after the ScrollView, as otherwise it will be behind it and the RefreshButton unselectable
        GameObject* existingLoadinControl = Resources::FindObjectsOfTypeAll<LoadingControl*>()->values[0]->get_gameObject();
        GameObject* loadinControlGameObject = UnityEngine::GameObject::Instantiate(existingLoadinControl, get_transform());
        auto loadingControlTransform = loadinControlGameObject->get_transform();
        loadingControl = loadinControlGameObject->GetComponent<LoadingControl*>();
        loadingControl->add_didPressRefreshButtonEvent(il2cpp_utils::MakeDelegate<System::Action*>(classof(System::Action*),
            (std::function<void()>) [this]() {
                Search();
            }
        ));
        loadingControl->loadingText->set_text(il2cpp_utils::newcsstr("Loading..."));
        loadingControl->set_enabled(true);

        Search();
    }
}

void DownloadSongsSearchViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
    if(SearchEntry::spriteCount > 0) {
        SearchEntry::spriteCount = 0;
        Resources::UnloadUnusedAssets();
    }
}