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

DEFINE_TYPE(SongDownloader, DownloadSongsOptionsViewController);

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
        
        AddConfigValueToggle(settingsLayoutTransform, getModConfig().AutoMapper)->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);
        AddConfigValueToggle(settingsLayoutTransform, getModConfig().BsrSearch)->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        HMUI::SimpleTextDropdown* SortOrderDropdown = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().SortOrder.GetName(), getModConfig().SortOrder.GetValue(), { "Latest", "Relevance", "Rating" },
            [] (std::string value) {
                getModConfig().SortOrder.SetValue(value);
            }
        );
        SortOrderDropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string NEConfigSet = "Not Required";
        if (getModConfig().NE.GetValue() == "true") NEConfigSet = "Required";
        else if (getModConfig().NE.GetValue() == "false") NEConfigSet = "Excluded";

        HMUI::SimpleTextDropdown* NEdropdown = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().NE.GetName(), NEConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().NE.SetValue(setting);
            }
        );
        NEdropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string MEConfigSet = "Not Required";
        if (getModConfig().ME.GetValue() == "true") MEConfigSet = "Required";
        else if (getModConfig().ME.GetValue() == "false") MEConfigSet = "Excluded";

        HMUI::SimpleTextDropdown* MEdropdown = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().ME.GetName(), MEConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().ME.SetValue(setting);
            }
        );
        MEdropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string ChromaConfigSet = "Not Required";
        if (getModConfig().Chroma.GetValue() == "true") ChromaConfigSet = "Required";
        else if (getModConfig().Chroma.GetValue() == "false") ChromaConfigSet = "Excluded";

        HMUI::SimpleTextDropdown* ChromaDropdown = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().Chroma.GetName(), ChromaConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().Chroma.SetValue(setting);
            }
        );
        ChromaDropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);
    }
}