#include "CustomTypes/DownloadSongsSearchViewController.hpp"

#include "CustomLogger.hpp"
#include "ModConfig.hpp"

//#include "BeatSaverAPI.hpp"
#include "BeatmapsIO-API.hpp"

#include "songloader/shared/API.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest_UnityWebRequestError.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Resources.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/Touchable.hpp"

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
using namespace UnityEngine::Networking;
using namespace HMUI;
using namespace TMPro;
using namespace GlobalNamespace;

using namespace SongDownloader;

int SearchEntry::spriteCount = 0;

SearchEntry::SearchEntry(GameObject* _gameObject, TextMeshProUGUI* _line1Component, TextMeshProUGUI* _line2Component, HMUI::ImageView* _coverImageView, Button* _downloadButton) : gameObject(_gameObject), line1Component(_line1Component), line2Component(_line2Component), coverImageView(_coverImageView), downloadButton(_downloadButton) {
}

const BeatmapsIO::Beatmap& SearchEntry::GetBeatmap() {
    return map;
}

void SearchEntry::SetBeatmap(const BeatmapsIO::Beatmap& _map) {
    map = _map;
    gameObject->SetActive(true);

    line1Component->SetText(il2cpp_utils::newcsstr(map.GetMetadata().GetSongName()));

    line2Component->SetText(il2cpp_utils::newcsstr(map.GetMetadata().GetSongAuthorName() + " <color=#ADADADFF>[" + map.GetMetadata().GetLevelAuthorName() + "]</color>"));
    int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
    
    coverImageView->set_enabled(false);
    BeatmapsIO::API::GetCoverImageAsync(map, [this, currentSearchIndex](std::vector<uint8_t> bytes) {
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

void SearchEntry::UpdateDownloadProgress(bool checkLoaded) {
    if(checkLoaded) {
        downloadProgress = -1.0f;
        auto hash = map.GetVersions().front().GetHash();
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
    authorNameTextComponent->set_fontSize(3.4f);
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
                auto hash = entry->GetBeatmap().GetVersions().front().GetHash();
                BeatmapsIO::API::DownloadBeatmapAsync(entry->GetBeatmap(), 
                    [this] (bool error) {
                        if(!error) {
                            QuestUI::MainThreadScheduler::Schedule(
                                [] {
                                    RuntimeSongLoader::API::RefreshSongs(false);
                                }
                            );
                        }
                    },
                    [entry, hash] (float percentage) {
                        if(entry->GetBeatmap().GetVersions().front().GetHash() == hash) {
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
        ));
    }
    Object::Destroy(prefab);
}

void DownloadSongsSearchViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();

        auto searchSetting = BeatSaberUI::CreateStringSetting(get_transform(), "Search", "", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector3(0.0f, -38.0f, 0.0f),
            [this] (std::string value) {
                DownloadSongsSearchViewController::searchIndex++;
                int currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
                if(value.empty()) {
                    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
                        searchEntries[i].Disable();
                    }
                } else {
                    // TODO: Replace 'GetBeatmapByKeyAsync' with 'GetBeatmapByIdAsync' as keys all now have an int representation for example: 0xff9 -> 4089 
                    if(getModConfig().BsrSearch.GetValue()) {
                        BeatmapsIO::API::GetBeatmapByKeyAsync(value,
                            [this, currentSearchIndex] (std::optional<BeatmapsIO::Beatmap> beatmap) {
                                if(currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                    QuestUI::MainThreadScheduler::Schedule(
                                        [this, currentSearchIndex, beatmap] {
                                            if(currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                                if(beatmap.has_value()) {
                                                    searchEntries[0].SetBeatmap(beatmap.value());
                                                    for(int i = 1; i < ENTRIES_PER_PAGE; i++) {
                                                        searchEntries[i].Disable();
                                                    }
                                                } else {
                                                    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                                        searchEntries[i].Disable();
                                                    }
                                                }
                                                if(SearchEntry::spriteCount > MAX_SPRITES) {
                                                    SearchEntry::spriteCount = 0;
                                                    Resources::UnloadUnusedAssets();
                                                }
                                            }
                                        }
                                    );
                                }
                            }
                        );
                    } else {
                        BeatmapsIO::API::SearchPagedAsync(value, 0,
                            [this, currentSearchIndex] (std::optional<BeatmapsIO::Page> page) {
                                if(currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                    QuestUI::MainThreadScheduler::Schedule(
                                        [this, currentSearchIndex, page] {
                                            if(currentSearchIndex == DownloadSongsSearchViewController::searchIndex) {
                                                if(page.has_value()) {
                                                    auto maps = page.value().GetDocs();
                                                    auto mapsSize = maps.size();
                                                    int mapIndex = 0;
                                                    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                                        auto& searchEntry = searchEntries[i];
                                                        if(mapIndex < mapsSize) {
                                                            auto& map = maps.at(mapIndex);
                                                            if(getModConfig().AutoMapper.GetValue()) {
                                                                searchEntry.SetBeatmap(map);
                                                            } else {
                                                                while(map.GetAutomapper()) {
                                                                    mapIndex++;
                                                                    if(mapIndex >= mapsSize) {
                                                                        break;
                                                                    }
                                                                    map = maps.at(mapIndex);
                                                                }
                                                                if(!map.GetAutomapper())
                                                                    searchEntry.SetBeatmap(map);
                                                            }
                                                        } else {
                                                            searchEntry.Disable();
                                                        }
                                                        mapIndex++;
                                                    }
                                                } else {
                                                    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
                                                        searchEntries[i].Disable();
                                                    }
                                                }
                                                if(SearchEntry::spriteCount > MAX_SPRITES) {
                                                    SearchEntry::spriteCount = 0;
                                                    Resources::UnloadUnusedAssets();
                                                }
                                            }
                                        }
                                    );
                                }
                            }
                        );
                    }
                }
            }
        );
        auto container = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -4.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -8.0f));
        CreateEntries(container->get_transform());
    } else {
        for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
            if(searchEntries[i].IsEnabled())
                searchEntries[i].UpdateDownloadProgress(true);
        }
    }
}

void DownloadSongsSearchViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
    if(SearchEntry::spriteCount > 0) {
        SearchEntry::spriteCount = 0;
        Resources::UnloadUnusedAssets();
    }
}