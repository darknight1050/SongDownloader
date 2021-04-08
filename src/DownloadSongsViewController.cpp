#include "DownloadSongsViewController.hpp"

#include "CustomLogger.hpp"

#include "BeatSaverAPI.hpp"

#include "songloader/shared/API.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/Touchable.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

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

using namespace SongDownloader;

SearchEntry::SearchEntry(GameObject* _gameObject, TextMeshProUGUI* _line1Component, TextMeshProUGUI* _line2Component, Button* _downloadButton) : gameObject(_gameObject), line1Component(_line1Component), line2Component(_line2Component), downloadButton(_downloadButton) {

}

const BeatSaver::Beatmap& SearchEntry::GetBeatmap() {
    return map;
}

void SearchEntry::SetBeatmap(const BeatSaver::Beatmap& _map) {
    map = _map;
    gameObject->SetActive(true);
    line1Component->SetText(il2cpp_utils::createcsstr(map.GetMetadata().GetSongName()));
    line2Component->SetText(il2cpp_utils::createcsstr(map.GetMetadata().GetSongAuthorName() + " <color=#CCCCCCFF>[" + map.GetMetadata().GetLevelAuthorName() + "]</color>"));
    downloadProgress = -1.0f;
    auto hash = _map.GetHash();
    std::transform(hash.begin(), hash.end(), hash.begin(), toupper);
    for(auto song : LoadedSongs) {
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
    auto prefabEntryLayout = BeatSaberUI::CreateHorizontalLayoutGroup(parent);
    auto prefabEntryParent = prefabEntryLayout->get_transform();
    prefabEntryLayout->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 2, 2));
    prefabEntryLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
    
    auto prefabTextLayout = BeatSaberUI::CreateVerticalLayoutGroup(prefabEntryParent);
    prefabTextLayout->GetComponent<LayoutElement*>()->set_preferredWidth(64.0f);
    auto prefabTextLayoutParent = prefabTextLayout->get_transform();
    BeatSaberUI::CreateText(prefabTextLayoutParent, "");
    auto prefabLine2Component = BeatSaberUI::CreateText(prefabTextLayoutParent, "");
    prefabLine2Component->set_fontSize(3.0f);
    BeatSaberUI::CreateUIButton(prefabEntryParent, "Download", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(24.0f, 10.0f), nullptr);

    auto prefabGameObject = prefabEntryLayout->get_gameObject();
    prefabGameObject->SetActive(false);
    
    for(int i = 0; i < ENTRIES_PER_PAGE; i++) {
        auto copy = Object::Instantiate(prefabGameObject, parent);
        auto textLayout = copy->GetComponentInChildren<VerticalLayoutGroup*>();
        auto textComponents = textLayout->get_gameObject()->GetComponentsInChildren<TextMeshProUGUI*>();
        auto downloadButton = copy->GetComponentInChildren<Button*>();
        searchEntries[i] = SearchEntry(copy, textComponents->values[0], textComponents->values[1], downloadButton);
        auto entry = &searchEntries[i];
        downloadButton->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityAction*>(classof(UnityAction*), 
            (std::function<void()>) [this, entry] {
                BeatSaver::API::DownloadBeatmapAsync(entry->GetBeatmap(), 
                    [this] (bool error) {
                        songsRefresh = !error;
                    },
                    [entry] (float percentage) {
                        entry->downloadProgress = percentage;
                    }
                );
            }
        ));
    }
    Object::Destroy(prefabGameObject);
    
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
    if(songsRefresh) {
        LOG_INFO("RefreshSongs");
        songsRefresh = false;
        RuntimeSongLoader::API::RefreshSongs(false);
    }
}