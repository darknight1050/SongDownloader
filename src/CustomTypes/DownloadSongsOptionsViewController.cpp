#include "CustomTypes/DownloadSongsOptionsViewController.hpp"
#include "CustomTypes/DownloadSongsSearchViewController.hpp"

#include "CustomLogger.hpp"
#include "ModConfig.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "HMUI/Touchable.hpp"

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

        VerticalLayoutGroup* settingsLayout = BSML::Lite::CreateVerticalLayoutGroup(parent);
        RectTransform* settingsLayoutTransform = settingsLayout->GetComponent<RectTransform*>();
        static ConstString backgroundName("round-rect-panel");
        settingsLayout->get_gameObject()->AddComponent<BSML::Backgroundable*>()->ApplyBackground(backgroundName);
        settingsLayout->set_spacing(1.2f);
        settingsLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

        auto contentSizeFitter = settingsLayout->get_gameObject()->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
        contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

        static ConstString emptyString("");
        Service = BSML::Lite::CreateDropdown(settingsLayoutTransform, getModConfig().Service.GetName(), getModConfig().Service.GetValue(), { "BeatSaver", "BeastSaber", "ScoreSaber" },
            [this](StringW value) {
                if (value != "BeatSaver") {
                    if (value == "ScoreSaber") {
                        if (getModConfig().ListType_ScoreSaber.GetValue() == "Top Ranked" || getModConfig().ListType_ScoreSaber.GetValue() == "Latest Ranked") {
                            Ranked->get_gameObject()->SetActive(false);
                            //RankedToggle->get_gameObject()->SetActive(false);
                        }
                        else {
                            Ranked->get_gameObject()->SetActive(true);
                            //RankedToggle->get_gameObject()->SetActive(true);
                        }
                        //RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                        //Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                        ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                        ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                    }
                    else { // if (value == "BeastSaber")
                        //RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
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
                    //RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                    Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(true);
                    Ranked->get_gameObject()->SetActive(true);
                }
                if (value == "BeastSaber" && getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery = getModConfig().BookmarkUsername.GetValue();
                    searchViewController->SearchField->SetText(getModConfig().BookmarkUsername.GetValue());
                }
                else if (LastListType == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(emptyString);
                }
                getModConfig().Service.SetValue(static_cast<std::string>(value));
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(Service->get_gameObject(), getModConfig().Service.GetHoverHint());
        Service->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        ListType_BeatSaver = BSML::Lite::CreateDropdown(settingsLayoutTransform, "List", getModConfig().ListType_BeatSaver.GetValue(), { "Key", "Search", "User"  },
            [this](StringW value) {
                if (LastListType == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(emptyString);
                }
                LastListType = static_cast<std::string>(value);
                getModConfig().ListType_BeatSaver.SetValue(LastListType);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(ListType_BeatSaver->get_gameObject(), getModConfig().ListType_BeatSaver.GetHoverHint());
        ListType_BeatSaver->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        ListType_BeastSaber = BSML::Lite::CreateDropdown(settingsLayoutTransform, "List", getModConfig().ListType_BeastSaber.GetValue(), { "Curator Recommended", "Bookmarks" },
            [this](StringW value) {
                if (value == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery = getModConfig().BookmarkUsername.GetValue();
                    searchViewController->SearchField->SetText(getModConfig().BookmarkUsername.GetValue());
                }
                else if (getModConfig().ListType_BeastSaber.GetValue() == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText(emptyString);
                }
                LastListType = static_cast<std::string>(value);
                getModConfig().ListType_BeastSaber.SetValue(LastListType);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(ListType_BeastSaber->get_gameObject(), getModConfig().ListType_BeastSaber.GetHoverHint());
        ListType_BeastSaber->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        ListType_ScoreSaber = BSML::Lite::CreateDropdown(settingsLayoutTransform, "List", getModConfig().ListType_ScoreSaber.GetValue(), { "Trending", "Latest Ranked", "Top Played", "Top Ranked" },
            [this](StringW value) {
                if (LastListType == "Bookmarks") {
                    DownloadSongsSearchViewController::SearchQuery.clear();
                    searchViewController->SearchField->SetText("");
                }
                if (value == "Top Ranked" || value == "Latest Ranked") {
                    Ranked->get_gameObject()->SetActive(false);
                }
                else {
                    Ranked->get_gameObject()->SetActive(true);
                }
                LastListType = static_cast<std::string>(value);
                getModConfig().ListType_ScoreSaber.SetValue(LastListType);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(ListType_ScoreSaber->get_gameObject(), getModConfig().ListType_ScoreSaber.GetHoverHint());
        ListType_ScoreSaber->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string AutoMapperConfigSet = "Excluded";
        if (getModConfig().AutoMapper.GetValue() == "true") AutoMapperConfigSet = "Not Required";
        else if (getModConfig().AutoMapper.GetValue() == "false") AutoMapperConfigSet = "Required";

        Automapper = BSML::Lite::CreateDropdown(settingsLayoutTransform, getModConfig().AutoMapper.GetName(), AutoMapperConfigSet, { "Not Required", "Required", "Excluded" },
            [](StringW value) {
                std::string setting = "";
                if (value == "Not Required") setting = "true";
                else if (value == "Required") setting = "false";
                getModConfig().AutoMapper.SetValue(setting);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(Automapper->get_gameObject(), getModConfig().AutoMapper.GetHoverHint());
        Automapper->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        SortOrder = BSML::Lite::CreateDropdown(settingsLayoutTransform, getModConfig().SortOrder.GetName(), getModConfig().SortOrder.GetValue(), { "Latest", "Relevance", "Rating" },
            [](StringW value) {
                getModConfig().SortOrder.SetValue(std::string(value));
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(SortOrder->get_gameObject(), getModConfig().SortOrder.GetHoverHint());
        SortOrder->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string NEConfigSet = "Not Required";
        if (getModConfig().NE.GetValue() == "true") NEConfigSet = "Required";
        else if (getModConfig().NE.GetValue() == "false") NEConfigSet = "Excluded";

        NEdropdown = BSML::Lite::CreateDropdown(settingsLayoutTransform, getModConfig().NE.GetName(), NEConfigSet, { "Not Required", "Required", "Excluded" },
            [](StringW value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().NE.SetValue(setting);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(NEdropdown->get_gameObject(), getModConfig().NE.GetHoverHint());
        NEdropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string MEConfigSet = "Not Required";
        if (getModConfig().ME.GetValue() == "true") MEConfigSet = "Required";
        else if (getModConfig().ME.GetValue() == "false") MEConfigSet = "Excluded";

        MEdropdown = BSML::Lite::CreateDropdown(settingsLayoutTransform, getModConfig().ME.GetName(), MEConfigSet, { "Not Required", "Required", "Excluded" },
            [](StringW value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().ME.SetValue(setting);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(MEdropdown->get_gameObject(), getModConfig().ME.GetHoverHint());
        MEdropdown->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string ChromaConfigSet = "Not Required";
        if (getModConfig().Chroma.GetValue() == "true") ChromaConfigSet = "Required";
        else if (getModConfig().Chroma.GetValue() == "false") ChromaConfigSet = "Excluded";

        Chroma = BSML::Lite::CreateDropdown(settingsLayoutTransform, getModConfig().Chroma.GetName(), ChromaConfigSet, { "Not Required", "Required", "Excluded" },
            [](StringW value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().Chroma.SetValue(setting);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(Chroma->get_gameObject(), getModConfig().Chroma.GetHoverHint());
        Chroma->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        std::string RankedConfigSet = "Not Required";
        if (getModConfig().Ranked.GetValue() == "true") RankedConfigSet = "Required";
        else if (getModConfig().Ranked.GetValue() == "false") RankedConfigSet = "Excluded";


        Ranked = BSML::Lite::CreateDropdown(settingsLayoutTransform, "Ranked", RankedConfigSet, { "Not Required", "Required", "Excluded" },
            [](StringW value) {
                std::string setting = "";
                if (value == "Required") setting = "true";
                else if (value == "Excluded") setting = "false";
                getModConfig().Ranked.SetValue(setting);
                DownloadSongsSearchViewController::SetPage(0);
                DownloadSongsSearchViewController::Search();
            }
        );
        BSML::Lite::AddHoverHint(Ranked->get_gameObject(), getModConfig().Ranked.GetHoverHint());
        Ranked->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);

        //RankedToggle = BSML::Lite::CreateToggle(settingsLayoutTransform, "Ranked", getModConfig().Ranked_Toggle.GetValue(),
        //    [](bool value) {
        //        getModConfig().Ranked_Toggle.SetValue(value);
        //        DownloadSongsSearchViewController::Search();
        //    }
        //);
        //BSML::Lite::AddHoverHint(RankedToggle->get_gameObject(), getModConfig().Ranked_Toggle.GetHoverHint());
        //RankedToggle->get_transform()->GetParent()->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);


        std::string checkValue = getModConfig().Service.GetValue();
        if (checkValue != "BeatSaver") {
            if (checkValue == "ScoreSaber") {
                if (getModConfig().ListType_ScoreSaber.GetValue() == "Top Ranked" || getModConfig().ListType_ScoreSaber.GetValue() == "Latest Ranked") {
                    Ranked->get_gameObject()->SetActive(false);
                    //RankedToggle->get_gameObject()->SetActive(false);
                }
                //Ranked->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            }
            else {
                //RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
                ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            }
            ListType_BeatSaver->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            Automapper->get_gameObject()->SetActive(false);
            SortOrder->get_gameObject()->SetActive(false);
            NEdropdown->get_gameObject()->SetActive(false);
            MEdropdown->get_gameObject()->SetActive(false);
            Chroma->get_gameObject()->SetActive(false);
            //Ranked->get_gameObject()->SetActive(false);
        }
        else {
            ListType_BeastSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            ListType_ScoreSaber->get_transform()->GetParent()->get_gameObject()->SetActive(false);
            //RankedToggle->get_transform()->GetParent()->get_gameObject()->SetActive(false);
        }
    }
}
