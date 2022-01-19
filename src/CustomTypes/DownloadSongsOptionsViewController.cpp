#include "CustomTypes/DownloadSongsOptionsViewController.hpp"
#include "CustomTypes/DownloadSongsSearchViewController.hpp"

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
        parent->set_localPosition({27.5, 0, 0});

        VerticalLayoutGroup* settingsLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(parent);
        RectTransform* settingsLayoutTransform = settingsLayout->GetComponent<RectTransform*>();
        settingsLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::newcsstr("round-rect-panel"));
        settingsLayout->set_spacing(1.2f);
        settingsLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

        ContentSizeFitter* contentSizeFitter = settingsLayout->get_gameObject()->AddComponent<ContentSizeFitter*>();
        contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);       

        Service = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().Service.GetName(), getModConfig().Service.GetValue(), { "BeatSaver", "BeastSaber", "ScoreSaber" },
            [this](std::string_view value) {
                if (value != "BeatSaver") {
                    if (value == "ScoreSaber") {
                        if (getModConfig().ListType_ScoreSaber.GetValue() == "Top Ranked" || getModConfig().ListType_ScoreSaber.GetValue() == "Latest Ranked") {
                            RankedToggle->get_gameObject()->SetActive(false);
                        }
                        else {
                            RankedToggle->get_gameObject()->SetActive(true);
                        }
                        RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                        Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                        ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                        ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                    }
                    else { // if (value == "BeastSaber")
                        RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                        Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                        ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                        ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                    }
                    ListType_BeatSaver->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                    Automapper->get_gameObject()->SetActive(false);
                    SortOrder->get_gameObject()->SetActive(false);
                    NEdropdown->get_gameObject()->SetActive(false);
                    MEdropdown->get_gameObject()->SetActive(false);
                    Chroma->get_gameObject()->SetActive(false);
                    Ranked->get_gameObject()->SetActive(false);
                }
                else {
                    ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                    ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                    ListType_BeatSaver->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                    Automapper->get_gameObject()->SetActive(true);
                    SortOrder->get_gameObject()->SetActive(true);
                    NEdropdown->get_gameObject()->SetActive(true);
                    MEdropdown->get_gameObject()->SetActive(true);
                    Chroma->get_gameObject()->SetActive(true);
                    RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                    Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                    Ranked->get_gameObject()->SetActive(true);
                }
                if (value == "BeastSaber" && getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery = getModConfig().BookmarkUsername.GetValue();
                    searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(getModConfig().BookmarkUsername.GetValue()));
                }
                else if (LastListType == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(""));
                }
                getModConfig().Service.SetValue(std::string(value));
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(Service->get_gameObject(), getModConfig().Service.GetHoverHint());
        Service->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        ListType_BeatSaver = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, "List", getModConfig().ListType_BeatSaver.GetValue(), { "Key", "Search", "User"  },
            [this](std::string_view value) {
                if (LastListType == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(""));
                }
                LastListType = value;
                getModConfig().ListType_BeatSaver.SetValue(std::string(value));
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(ListType_BeatSaver->get_gameObject(), getModConfig().ListType_BeatSaver.GetHoverHint());
        ListType_BeatSaver->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        ListType_BeastSaber = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, "List", getModConfig().ListType_BeastSaber.GetValue(), { "Curator Recommended", "Bookmarks" },
            [this](std::string_view value) {
                if (value == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery = getModConfig().BookmarkUsername.GetValue();
                    searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(getModConfig().BookmarkUsername.GetValue()));
                }
                else if (getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(""));
                }
                LastListType = value;
                getModConfig().ListType_BeastSaber.SetValue(std::string(value));
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(ListType_BeastSaber->get_gameObject(), getModConfig().ListType_BeastSaber.GetHoverHint());
        ListType_BeastSaber->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        ListType_ScoreSaber = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, "List", getModConfig().ListType_ScoreSaber.GetValue(), { "Trending", "Latest Ranked", "Top Played", "Top Ranked" },
            [this](std::string_view value) {
                if (LastListType == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(il2cpp_utils::newcsstr(""));
                }
                if (value == "Top Ranked" || value == "Latest Ranked") {
                    RankedToggle->get_gameObject()->SetActive(false);
                }
                else {
                    RankedToggle->get_gameObject()->SetActive(true);
                }
                LastListType = value;
                getModConfig().ListType_ScoreSaber.SetValue(std::string(value));
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(ListType_ScoreSaber->get_gameObject(), getModConfig().ListType_ScoreSaber.GetHoverHint());
        ListType_ScoreSaber->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string AutoMapperConfigSet = "Excluded";
        if (getModConfig().AutoMapper.GetValue() == "true") AutoMapperConfigSet = "Not Required";
        else if (getModConfig().AutoMapper.GetValue() == "false") AutoMapperConfigSet = "Required";

        Automapper = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().AutoMapper.GetName(), AutoMapperConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string_view value) {
                std::string setting = "";
                if (value == "Not Required") setting = "true";
                else if (value == "Required") setting = "false";
                getModConfig().AutoMapper.SetValue(setting);
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(Automapper->get_gameObject(), getModConfig().AutoMapper.GetHoverHint());
        Automapper->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        SortOrder = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().SortOrder.GetName(), getModConfig().SortOrder.GetValue(), { "Latest", "Relevance", "Rating" },
            [](std::string_view value) {
                getModConfig().SortOrder.SetValue(std::string(value));
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(SortOrder->get_gameObject(), getModConfig().SortOrder.GetHoverHint());
        SortOrder->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string NEConfigSet = "Not Required";
        if (getModConfig().NE.GetValue() == "true") NEConfigSet = "Required";
        else if (getModConfig().NE.GetValue() == "false") NEConfigSet = "Excluded";

        NEdropdown = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().NE.GetName(), NEConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string_view value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().NE.SetValue(setting);
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(NEdropdown->get_gameObject(), getModConfig().NE.GetHoverHint());
        NEdropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string MEConfigSet = "Not Required";
        if (getModConfig().ME.GetValue() == "true") MEConfigSet = "Required";
        else if (getModConfig().ME.GetValue() == "false") MEConfigSet = "Excluded";

        MEdropdown = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().ME.GetName(), MEConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string_view value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().ME.SetValue(setting);
                DownloadSongsSearchViewController::Search();
            }
        ); 
        QuestUI::BeatSaberUI::AddHoverHint(MEdropdown->get_gameObject(), getModConfig().ME.GetHoverHint());
        MEdropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string ChromaConfigSet = "Not Required";
        if (getModConfig().Chroma.GetValue() == "true") ChromaConfigSet = "Required";
        else if (getModConfig().Chroma.GetValue() == "false") ChromaConfigSet = "Excluded";

        Chroma = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, getModConfig().Chroma.GetName(), ChromaConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string_view value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().Chroma.SetValue(setting);
                DownloadSongsSearchViewController::Search();
            }
        ); 
        QuestUI::BeatSaberUI::AddHoverHint(Chroma->get_gameObject(), getModConfig().Chroma.GetHoverHint());
        Chroma->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string RankedConfigSet = "Not Required";
        if (getModConfig().Ranked.GetValue() == "true") RankedConfigSet = "Required";
        else if (getModConfig().Ranked.GetValue() == "false") RankedConfigSet = "Excluded";


        Ranked = QuestUI::BeatSaberUI::CreateDropdown(settingsLayoutTransform, "Ranked", RankedConfigSet, { "Not Required", "Required", "Excluded" },
            [](std::string_view value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().Ranked.SetValue(setting);
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(Ranked->get_gameObject(), getModConfig().Ranked.GetHoverHint());
        Ranked->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        RankedToggle = QuestUI::BeatSaberUI::CreateToggle(settingsLayoutTransform, "Ranked", getModConfig().Ranked_Toggle.GetValue(),
            [](bool value) {
                getModConfig().Ranked_Toggle.SetValue(value);
                DownloadSongsSearchViewController::Search();
            }
        );
        QuestUI::BeatSaberUI::AddHoverHint(RankedToggle->get_gameObject(), getModConfig().Ranked_Toggle.GetHoverHint());
        RankedToggle->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);


        std::string checkValue = getModConfig().Service.GetValue();
        if (checkValue != "BeatSaver") {
            if (checkValue == "ScoreSaber") {
                if (getModConfig().ListType_ScoreSaber.GetValue() == "Top Ranked" || getModConfig().ListType_ScoreSaber.GetValue() == "Latest Ranked") {
                    RankedToggle->get_gameObject()->SetActive(false);
                }
                Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            }
            else {
                RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            }
            ListType_BeatSaver->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            Automapper->get_gameObject()->SetActive(false);
            SortOrder->get_gameObject()->SetActive(false);
            NEdropdown->get_gameObject()->SetActive(false);
            MEdropdown->get_gameObject()->SetActive(false);
            Chroma->get_gameObject()->SetActive(false);
            Ranked->get_gameObject()->SetActive(false);
        }
        else {
            ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
        }
    }
}