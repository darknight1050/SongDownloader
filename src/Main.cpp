#include <chrono>

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "custom-types/shared/register.hpp"

#include "questui/shared/QuestUI.hpp"

#include "CustomLogger.hpp"

#include "WebUtil.hpp"

#include "Types/BeatSaver/Beatmap.hpp"

#include "BeatSaverAPI.hpp"

ModInfo modInfo;

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true)); 
    return *logger; 
}

extern "C" void setup(ModInfo& info) {
    modInfo.id = "SongDownloader";
    modInfo.version = VERSION;
    info = modInfo;
}

#include "HMUI/ViewController.hpp"

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

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
using namespace TMPro;
Transform* parent;

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
            BeatSaver::API::DownloadBeatmap(map);
        }
    );
}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();

        auto searchSetting = BeatSaberUI::CreateStringSetting(self->get_transform(), "Search", "", 
            [] (std::string value) { 
                if(value.empty())
                    return;
                getLogger().info(value);
                while(parent->get_childCount() > 0) {
                    Object::DestroyImmediate(parent->GetChild(0)->get_gameObject());
                }
                auto search = BeatSaver::API::SearchPage(value);
                if(search.has_value()) {
                    for(auto map : search.value().GetDocs()) {
                        createEntry(map);
                    }
                }
            }
        );
        auto container = BeatSaberUI::CreateScrollView(self->get_transform());
        ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -4.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -8.0f));
        parent = container->get_transform();
    }
}

extern "C" void load() {
    LOG_INFO("Starting SongDownloader installation...");
    il2cpp_functions::Init();
    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    LOG_INFO("Successfully installed SongDownloader!");
}