#include "CustomTypes/DownloadSongsOptionsViewController.hpp"

#include "CustomLogger.hpp"
#include "ModConfig.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/QuestUI.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "HMUI/Touchable.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace TMPro;
using namespace SongDownloader;

DEFINE_TYPE(DownloadSongsOptionsViewController);

void DownloadSongsOptionsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();
        
        GameObject* mainLayout = GameObject::New_ctor();
        RectTransform* parent = mainLayout->AddComponent<RectTransform*>();
        parent->SetParent(get_transform(), false);
        parent->set_localPosition(UnityEngine::Vector3(0.0f, 8.0f));

        VerticalLayoutGroup* settingsLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(parent);
		RectTransform* settingsLayoutTransform = settingsLayout->GetComponent<RectTransform*>();
		settingsLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
		settingsLayout->set_spacing(1.2f);
		settingsLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

		ContentSizeFitter* contentSizeFitter = settingsLayout->get_gameObject()->AddComponent<ContentSizeFitter*>();
		contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        
        AddConfigValueToggle(settingsLayoutTransform, getModConfig().AutoMapper)->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(45.0f);
        AddConfigValueToggle(settingsLayoutTransform, getModConfig().BsrSearch)->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(45.0f);
    }
}