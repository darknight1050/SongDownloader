#include "DownloadSongsViewController.hpp"

#include "CustomLogger.hpp"

#include "BeatSaverAPI.hpp"

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

SearchEntry::SearchEntry(GameObject* _gameObject, TextMeshProUGUI* _line1Component, TextMeshProUGUI* _line2Component, HMUI::ImageView* _coverImageView, Button* _downloadButton) : gameObject(_gameObject), line1Component(_line1Component), line2Component(_line2Component), coverImageView(_coverImageView), downloadButton(_downloadButton) {
}

const BeatSaver::Beatmap& SearchEntry::GetBeatmap() {
    return map;
}

void SearchEntry::SetBeatmap(const BeatSaver::Beatmap& _map) {
    map = _map;
    gameObject->SetActive(true);

    line1Component->SetText(il2cpp_utils::createcsstr(map.GetMetadata().GetSongName()));

    line2Component->SetText(il2cpp_utils::createcsstr(map.GetMetadata().GetSongAuthorName() + " <color=#ADADADFF>[" + map.GetMetadata().GetLevelAuthorName() + "]</color>"));

    BeatSaver::API::GetCoverImageAsync(_map, [this](std::vector<uint8_t> bytes){
        LOG_DEBUG("Downloaded cover image. Length: %d", bytes.size());

        MainThreadScheduler::Schedule([this, bytes] {
            std::vector<uint8_t> data = bytes;

            LOG_INFO("Setting cover image on main thread.");
            Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(data);
            Sprite* sprite = BeatSaberUI::ArrayToSprite(spriteArray);

            this->coverImageView->get_gameObject()->SetActive(true);
            this->coverImageView->set_sprite(sprite);
        });
    });

    downloadProgress = -1.0f;
    auto hash = _map.GetHash();
    std::transform(hash.begin(), hash.end(), hash.begin(), toupper);
    for(auto& song : RuntimeSongLoader::API::GetLoadedSongs()) {
        if(to_utf8(csstrtostr(song->levelID)).ends_with(hash)) {
            downloadProgress = 100.0f;
            break;
        }
    }
}

void SearchEntry::UpdateDownloadProgress() {
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

DEFINE_TYPE(DownloadSongsViewController);

void DownloadSongsViewController::CreateEntries(Transform* parent) {
    GameObject* existingLevelBar = Resources::FindObjectsOfTypeAll<LevelBar*>()->values[0]->get_gameObject();
    GameObject* levelBarPrefab = UnityEngine::GameObject::Instantiate(existingLevelBar, parent);

    LevelBar* levelBar = levelBarPrefab->GetComponent<LevelBar*>();
    levelBar->songNameText->set_fontSize(4.0f);
    levelBar->songNameText->set_overflowMode(TextOverflowModes::Ellipsis);
        
    levelBar->authorNameText->set_richText(true);
    levelBar->authorNameText->set_fontSize(3.0f);
    levelBar->authorNameText->set_overflowMode(TextOverflowModes::Ellipsis);

    static auto bgName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BG");
    Transform* backgroundTransform = levelBarPrefab->get_transform()->Find(bgName);
    backgroundTransform->set_localScale(Vector3(1.5f, 1.0f, 1.0f));

    static auto songArtworkName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("SongArtwork");
    levelBar->get_transform()->Find(songArtworkName)->set_localScale(Vector3(0.95f, 0.95f, 0.95f));

    Button* prefabDownloadButton = BeatSaberUI::CreateUIButton(levelBarPrefab->get_transform(), "Download", nullptr);

    LOG_INFO("Using background: %s", (levelBar->useArtworkBackground ? "true" : "false"));
    prefabDownloadButton->get_transform()->set_localPosition(Vector3(36.5f, -7.0f, 0.0f));
    levelBarPrefab->SetActive(false);
    
    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
        // Jank to make it fit
        HorizontalLayoutGroup* levelBarLayout = BeatSaberUI::CreateHorizontalLayoutGroup(parent);
        levelBarLayout->set_childControlWidth(false);
        levelBarLayout->set_childForceExpandWidth(true);

        LayoutElement* levelBarLayoutElement = levelBarLayout->GetComponent<LayoutElement*>();
        levelBarLayoutElement->set_minHeight(15.0f);
        levelBarLayoutElement->set_minWidth(90.0f);

        auto copy = Object::Instantiate(levelBarPrefab, levelBarLayout->get_transform());
        LevelBar* copyLevelBar = copy->GetComponent<LevelBar*>();
        Button* downloadButton = copy->GetComponentInChildren<Button*>();
        Transform* artworkTransform = copyLevelBar->get_transform()->Find(songArtworkName);
        HMUI::ImageView* artwork = artworkTransform->GetComponent<HMUI::ImageView*>();

        searchEntries[i] = SearchEntry(copy, copyLevelBar->songNameText, copyLevelBar->authorNameText, artwork, downloadButton);
        auto entry = &searchEntries[i];
        downloadButton->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityAction*>(classof(UnityAction*), 
            (std::function<void()>) [this, entry] {
                BeatSaver::API::DownloadBeatmapAsync(entry->GetBeatmap(), 
                    [this] (bool error) {
                        if(!error)
                            RuntimeSongLoader::API::RefreshSongsThreadSafe(false);
                    },
                    [entry] (float percentage) {
                        entry->downloadProgress = percentage;
                    }
                );
            }
        ));
    }
    Object::Destroy(levelBarPrefab);
}

void DownloadSongsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();

        auto searchSetting = BeatSaberUI::CreateStringSetting(get_transform(), "Search", "", 
            [this] (std::string value) { 
                if(value.empty()) {
                    currentPage = std::nullopt;
                    pageChanged = true;
                    return;
                }
                BeatSaver::API::SearchPagedAsync(value, 0,
                    [this] (std::optional<BeatSaver::Page> page) {
                        currentPage = page;
                        pageChanged = true;
                    }
                );
            }
        );
        auto container = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -4.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -8.0f));
        CreateEntries(container->get_transform());
    }
}

void DownloadSongsViewController::Update() {
    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
        searchEntries[i].UpdateDownloadProgress();
    }
    if(pageChanged) {
        pageChanged = false;
        if(currentPage.has_value()) {
            auto maps = currentPage.value().GetDocs();
            auto mapsSize = maps.size();
            for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
                auto& searchEntry = searchEntries[i];
                if(i < mapsSize) {
                    searchEntry.SetBeatmap(maps.at(i));
                } else {
                    searchEntry.Disable();
                }
            }
        } else {
            for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
                searchEntries[i].Disable();
            }
        }
    }
}