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
        
        VerticalLayoutGroup* titleLayout = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        titleLayout->GetComponent<RectTransform*>()->set_anchoredPosition(UnityEngine::Vector2(0.0f, 36.0f));
        titleLayout->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        titleLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
        titleLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 1, 0));
        TextMeshProUGUI* titleText = BeatSaberUI::CreateText(titleLayout->get_transform(), "OPTIONS");
        titleText->set_alignment(TextAlignmentOptions::Center);
        titleText->set_fontSize(7.4f);

        VerticalLayoutGroup* settingsLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
		RectTransform* settingsLayoutTransform = settingsLayout->GetComponent<RectTransform*>();
        settingsLayoutTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 18.0f));
		settingsLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
		settingsLayout->set_spacing(1.2f);
		settingsLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

		ContentSizeFitter* contentSizeFitter = settingsLayout->get_gameObject()->AddComponent<ContentSizeFitter*>();
		contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        
        AddConfigValueToggle(settingsLayoutTransform, getModConfig().AutoMapper)->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(45.0f);

    }
}
