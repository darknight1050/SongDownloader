#include "CustomTypes/DownloadSongsSearchViewController.hpp"

#include "CustomLogger.hpp"
#include "ModConfig.hpp"

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

#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
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

DEFINE_TYPE(SongDownloader, DownloadSongsSearchViewController);

int DownloadSongsSearchViewController::searchIndex = 0;

int DownloadSongsSearchViewController::searchPage = 0;

std::string DownloadSongsSearchViewController::SearchQuery = "";

void DownloadSongsSearchViewController::CreateEntries(Transform* parent) {
    HorizontalLayoutGroup* levelBarLayout = BeatSaberUI::CreateHorizontalLayoutGroup(parent);
    GameObject* prefab = levelBarLayout->get_gameObject();
    levelBarLayout->set_childControlWidth(false);
    levelBarLayout->set_childForceExpandWidth(true);

    LayoutElement* levelBarLayoutElement = levelBarLayout->GetComponent<LayoutElement*>();
    levelBarLayoutElement->set_minHeight(15.0f);
    levelBarLayoutElement->set_minWidth(90.0f);

    GameObject* existingLevelBar = Resources::FindObjectsOfTypeAll<LevelBar*>()[0]->get_gameObject();
    GameObject* levelBarGameObject = UnityEngine::GameObject::Instantiate(existingLevelBar, levelBarLayout->get_transform());
    auto levelBarTransform = levelBarGameObject->get_transform();

    static auto multipleLineTextContainerName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("MultipleLineTextContainer");
    Object::Destroy(levelBarTransform->FindChild(multipleLineTextContainerName)->get_gameObject());

    static auto singleLineTextContainerName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("SingleLineTextContainer");
    levelBarTransform->FindChild(singleLineTextContainerName)->get_gameObject()->set_active(true);

    LevelBar* levelBar = levelBarGameObject->GetComponent<LevelBar*>();
    auto songNameTextComponent = levelBar->dyn__songNameText();
    songNameTextComponent->set_fontSize(4.2f);
    songNameTextComponent->set_overflowMode(TextOverflowModes::Ellipsis);
    songNameTextComponent->set_margin(Vector4(-2.0f, 0.0f, 9.0f, 0.0f));

    auto authorNameTextComponent = levelBar->dyn__authorNameText();
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

        searchEntries[i] = SearchEntry(copy, copyLevelBar->dyn__songNameText(), copyLevelBar->dyn__authorNameText(), artwork, downloadButton);
        auto entry = &searchEntries[i];
        downloadButton->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityAction*>(classof(UnityAction*), 
            (std::function<void()>) [this, entry] {
                if (entry->MapType == SearchEntry::MapType::BeatSaver) {
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
                else if (entry->MapType == SearchEntry::MapType::BeastSaber) {
                    auto hash = entry->GetSongBeastSaber().GetHash();
                    BeatSaver::API::DownloadBeatmapAsync(entry->GetSongBeastSaber(),
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
                            if (entry->GetSongBeastSaber().GetHash() == hash) {
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
                    auto hash = entry->GetSongScoreSaber().GetId();
                    BeatSaver::API::DownloadBeatmapAsync(entry->GetSongScoreSaber(),
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
                            if (entry->GetSongScoreSaber().GetId() == hash) {
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

std::optional<bool> StringToBool(std::string value) {
    if (value == "true") return true;
    else if (value == "false") return false;
    else return std::nullopt;
}

#pragma region SearchType Functions

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
    BeatSaver::API::SearchPagedAsync(DownloadSongsSearchViewController::SearchQuery, DownloadSongsSearchViewController::searchPage,
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
                                else if (DownloadSongsSearchViewController::SearchQuery.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr("No Results,\nis your Internet working?"), true);
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
        BeatSaver::API::GetUserByNameAsync(DownloadSongsSearchViewController::SearchQuery,
            [this, currentSearchIndex](std::optional<BeatSaver::UserDetail> User) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, User]() {
                        if (User.has_value()) {
                            BeatSaver::API::GetBeatmapByUserIdAsync(User.value().GetId(), DownloadSongsSearchViewController::searchPage,
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
                                                    if (BeatSaver::API::exception != "Not Found") loadingControl->ShowText(il2cpp_utils::newcsstr(BeatSaver::API::exception), true);
                                                    else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found for given User!"), true);
                                                }
                                            }
                                        }
                                    );
                                }
                            );
                        }
                        else {
                            if (BeatSaver::API::exception != "Not Found") loadingControl->ShowText(il2cpp_utils::newcsstr(BeatSaver::API::exception), true);
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
        , DownloadSongsSearchViewController::searchPage);
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
                                    else loadingControl->ShowText(il2cpp_utils::newcsstr("No bookmarks found\nfor given User!"), true);
                                }
                            }
                        }
                    );
                }
            }, 
        DownloadSongsSearchViewController::searchPage);
    }
    else loadingControl->ShowText(il2cpp_utils::newcsstr("Please type in a Username!"), false);
}

void DownloadSongsSearchViewController::GetTrending(int currentSearchIndex) {
    ScoreSaber::API::SearchAsync(DownloadSongsSearchViewController::SearchQuery, ScoreSaber::API::ListCategory::TopRanked,
    //ScoreSaber::API::GetTrendingAsync(
        [this, currentSearchIndex](std::optional<ScoreSaber::Leaderboards> page) {
            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, page] {
                        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                            if (page.has_value() && !page.value().GetLeaderboards().empty()) {
                                auto songs = page.value().GetLeaderboards();
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
                                if (!ScoreSaber::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(ScoreSaber::API::exception), true);
                                else if (DownloadSongsSearchViewController::SearchQuery.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr("No Results,\nis your Internet working?"), true);
                                else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found!"), true);
                            }
                        }
                    }
                );
            }
        },
        StringToBool(getModConfig().Ranked.GetValue()), std::nullopt, true, DownloadSongsSearchViewController::searchPage); // TODO: Possibly add option to search by qualified, for now just nullopt on that parameter
}

void DownloadSongsSearchViewController::GetLatestRanked(int currentSearchIndex) {
    ScoreSaber::API::SearchAsync(DownloadSongsSearchViewController::SearchQuery, ScoreSaber::API::ListCategory::LatestRanked,
    //ScoreSaber::API::SearchSSAsync(DownloadSongsSearchViewController::SearchQuery, ScoreSaber::API::SearchType::LatestRanked,
        [this, currentSearchIndex](std::optional<ScoreSaber::Leaderboards> page) {
            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, page] {
                        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                            if (page.has_value() && !page.value().GetLeaderboards().empty()) {
                                auto songs = page.value().GetLeaderboards();
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
                                if (!ScoreSaber::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(ScoreSaber::API::exception), true);
                                else if (DownloadSongsSearchViewController::SearchQuery.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr("No Results,\nis your Internet working?"), true);
                                else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found!"), true);
                            }
                        }
                    }
                );
            }
        },
        std::nullopt, std::nullopt, true, DownloadSongsSearchViewController::searchPage); // TODO: Possibly add option to search by qualified, for now just nullopt on that parameter
}

void DownloadSongsSearchViewController::GetTopPlayed(int currentSearchIndex) {
    ScoreSaber::API::GetListAsync(ScoreSaber::API::ListCategory::TopPlayed, /*)
    ScoreSaber::API::SearchSSAsync(DownloadSongsSearchViewController::SearchQuery, ScoreSaber::API::SearchType::TopPlayed,*/
        [this, currentSearchIndex](std::optional<ScoreSaber::Leaderboards> page) {
            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, page] {
                        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                            if (page.has_value() && !page.value().GetLeaderboards().empty()) {
                                auto songs = page.value().GetLeaderboards();
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
                                if (!ScoreSaber::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(ScoreSaber::API::exception), true);
                                else if (DownloadSongsSearchViewController::SearchQuery.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr("No Results,\nis your Internet working?"), true);
                                else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found!"), true);
                            }
                        }
                    }
                );
            }
        },
        StringToBool(getModConfig().Ranked.GetValue()), std::nullopt, true, DownloadSongsSearchViewController::searchPage); // TODO: Possibly add option to search by qualified, for now just nullopt on that parameter
}

void DownloadSongsSearchViewController::GetTopRanked(int currentSearchIndex) {
    ScoreSaber::API::SearchAsync(DownloadSongsSearchViewController::SearchQuery, ScoreSaber::API::ListCategory::TopRanked,
    //ScoreSaber::API::SearchSSAsync(DownloadSongsSearchViewController::SearchQuery, ScoreSaber::API::SearchType::TopRanked,
        [this, currentSearchIndex](std::optional<ScoreSaber::Leaderboards> page) {
            if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                QuestUI::MainThreadScheduler::Schedule(
                    [this, currentSearchIndex, page] {
                        if (currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                            if (page.has_value() && !page.value().GetLeaderboards().empty()) {
                                auto songs = page.value().GetLeaderboards();
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
                                if (!ScoreSaber::API::exception.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr(ScoreSaber::API::exception), true);
                                else if (DownloadSongsSearchViewController::SearchQuery.empty()) loadingControl->ShowText(il2cpp_utils::newcsstr("No Results,\nis your Internet working?"), true);
                                else loadingControl->ShowText(il2cpp_utils::newcsstr("No Songs Found!"), true);
                            }
                        }
                    }
                );
            }
        },
        std::nullopt, std::nullopt, true, DownloadSongsSearchViewController::searchPage); // TODO: Possibly add option to search by qualified, for now just nullopt on that parameter
}
#pragma endregion

void DownloadSongsSearchViewController::Search() {
    for (int i = 0; i < ENTRIES_PER_PAGE; i++) {
        searchViewController->searchEntries[i].Disable();
    }
    searchViewController->loadingControl->ShowLoading(il2cpp_utils::newcsstr("Loading..."));
    DownloadSongsSearchViewController::searchIndex++;
    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
    searchViewController->SearchField->get_gameObject()->SetActive(true);
    if (getModConfig().Service.GetValue() == "BeatSaver") {
        if (getModConfig().ListType_BeatSaver.GetValue() == "Key") {
            searchViewController->SearchKey(currentSearchIndex);
        }
        else if (getModConfig().ListType_BeatSaver.GetValue() == "Search") {
            searchViewController->SearchSongs(currentSearchIndex);
        }
        else if (getModConfig().ListType_BeatSaver.GetValue() == "User") {
            searchViewController->SearchUser(currentSearchIndex);
        }
        else {
            searchViewController->loadingControl->ShowText(il2cpp_utils::newcsstr("Invalid Selection for\nService BeatSaver!"), false);
        }
    }
    else if (getModConfig().Service.GetValue() == "BeastSaber") {
        if (getModConfig().ListType_BeastSaber.GetValue() == "Curator Recommended") {
            searchViewController->SearchField->get_gameObject()->SetActive(false);
            searchViewController->GetCuratorRecommended(currentSearchIndex);
        }
        else if (getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") {
            searchViewController->GetBookmarks(currentSearchIndex);
        }
        else {
            searchViewController->loadingControl->ShowText(il2cpp_utils::newcsstr("Invalid Selection for\nService BeastSaber!"), false);
        }
    }
    else if (getModConfig().Service.GetValue() == "ScoreSaber") {
        if (getModConfig().ListType_ScoreSaber.GetValue() == "Trending") {
            searchViewController->GetTrending(currentSearchIndex);
        }
        else if (getModConfig().ListType_ScoreSaber.GetValue() == "Latest Ranked") {
            searchViewController->GetLatestRanked(currentSearchIndex);
        }
        else if (getModConfig().ListType_ScoreSaber.GetValue() == "Top Played") {
            searchViewController->SearchField->get_gameObject()->SetActive(false);
            searchViewController->GetTopPlayed(currentSearchIndex);
        }
        else if (getModConfig().ListType_ScoreSaber.GetValue() == "Top Ranked") {
            searchViewController->GetTopRanked(currentSearchIndex);
        }
        else {
            searchViewController->loadingControl->ShowText(il2cpp_utils::newcsstr("Invalid Selection for\nService ScoreSaber!"), false);
        }
    }
    else {
        searchViewController->loadingControl->ShowText(il2cpp_utils::newcsstr("Invalid Selection\nselected Service Unknown!"), false);
    }
}

void DownloadSongsSearchViewController::SetPage(int page) {
    DownloadSongsSearchViewController::searchPage = page;

    searchViewController->pageIncrement->CurrentValue = page + 1;
    searchViewController->pageIncrement->UpdateValue();
}

void DownloadSongsSearchViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        searchViewController = this;
        get_gameObject()->AddComponent<Touchable*>();

        SearchField = BeatSaberUI::CreateStringSetting(get_transform(), "Search", "", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector3(0.0f, -38.0f, 0.0f),
            [this](std::string_view value) {
                DownloadSongsSearchViewController::SearchQuery = value;
                if (getModConfig().Service.GetValue() == "BeastSaber" && getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") getModConfig().BookmarkUsername.SetValue(std::string(value));
                Search();
            }
        );
        if (getModConfig().Service.GetValue() == "BeastSaber" && getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") {
            DownloadSongsSearchViewController::SearchQuery = getModConfig().BookmarkUsername.GetValue();
            searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(getModConfig().BookmarkUsername.GetValue()));
        }

        auto container = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -1.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -12.0f));
        CreateEntries(container->get_transform());

        pageIncrement = BeatSaberUI::CreateIncrementSetting(get_transform(), "", 0, 1, DownloadSongsSearchViewController::searchPage + 1, true, false, 1, 0, UnityEngine::Vector2(-60.0f, -72.0f),
            [this](float newValue){
                if(newValue - 1 != DownloadSongsSearchViewController::searchPage) {
                    DownloadSongsSearchViewController::searchPage = newValue - 1;
                    Search();
                }
            }
        );
        Object::Destroy(pageIncrement->GetComponentInChildren<TextMeshProUGUI*>());
        Object::Destroy(pageIncrement->GetComponentInChildren<LayoutElement*>());

        // LoadingControl has to be added after the ScrollView, as otherwise it will be behind it and the RefreshButton unselectable
        GameObject* existingLoadinControl = Resources::FindObjectsOfTypeAll<LoadingControl*>()[0]->get_gameObject();
        GameObject* loadinControlGameObject = UnityEngine::GameObject::Instantiate(existingLoadinControl, get_transform());
        auto loadingControlTransform = loadinControlGameObject->get_transform();
        loadingControl = loadinControlGameObject->GetComponent<LoadingControl*>();
        loadingControl->add_didPressRefreshButtonEvent(il2cpp_utils::MakeDelegate<System::Action*>(classof(System::Action*),
            (std::function<void()>) [this]() {
                Search();
            }
        ));
        loadingControl->dyn__loadingText()->set_text(il2cpp_utils::newcsstr("Loading..."));
        loadingControl->set_enabled(true);

        Search();
    }
}