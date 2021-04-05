#include "DownloadSongsViewController.hpp"

#include "CustomLogger.hpp"

#include "Types/BeatSaver/Beatmap.hpp"

#include "BeatSaverAPI.hpp"

#include "songloader/shared/API.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include <sstream>
#include <string>
#include <vector>

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
using namespace TMPro;

DEFINE_CLASS(SongDownloader::DownloadSongsViewController);

Transform* parent;

static bool refresh = false;

void createEntry(BeatSaver::Beatmap map) {
    auto entryLayout = BeatSaberUI::CreateHorizontalLayoutGroup(parent);
    auto entryParent = entryLayout->get_transform();
    entryLayout->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 2, 2));
    entryLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
    
    auto textLayout = BeatSaberUI::CreateVerticalLayoutGroup(entryParent);
    textLayout->GetComponent<LayoutElement*>()->set_preferredWidth(64.0f);
    auto textLayoutParent = textLayout->get_transform();
    auto songName = BeatSaberUI::CreateText(textLayoutParent, map.GetMetadata().GetSongName());
    auto levelAuthorName = BeatSaberUI::CreateText(textLayoutParent, "<color=#CCCCCCFF>Uploaded by " + map.GetMetadata().GetLevelAuthorName() + "</color>");
    levelAuthorName->set_fontSize(3.0f);
    auto button = BeatSaberUI::CreateUIButton(entryParent, "Download", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(24.0f, 10.0f),
        [map] { 
            BeatSaver::API::DownloadBeatmapAsync(map, 
                [] (bool error) {
                    LOG_INFO("Download Finished: %d", error);
                    refresh = !error;
                }, 
                [] (float percentage) {
                    LOG_INFO("Downloading: %f", percentage);
                }
            );
        }
    );
}

void SongDownloader::DownloadSongsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();

        auto searchSetting = BeatSaberUI::CreateStringSetting(get_transform(), "Search", "", 
            [] (std::string value) { 
                if(value.empty())
                    return;
                getLogger().info(value);
                while(parent->get_childCount() > 0) {
                    Object::DestroyImmediate(parent->GetChild(0)->get_gameObject());
                }
                auto search = BeatSaver::API::SearchPaged(value);
                if(search.has_value()) {
                    for(auto map : search.value().GetDocs()) {
                        createEntry(map);
                    }
                }
            }
        );
        auto container = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -4.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -8.0f));
        parent = container->get_transform();
    }
}

void SongDownloader::DownloadSongsViewController::Update() {
    if(refresh)
    {
        LOG_INFO("RefreshSongs");
        refresh = false;
        RuntimeSongLoader::API::RefreshSongs(false);
    }
}