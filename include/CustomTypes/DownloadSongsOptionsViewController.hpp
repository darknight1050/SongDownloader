#pragma once

#include "HMUI/ViewController.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "UnityEngine/UI/Toggle.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadSongsOptionsViewController, HMUI::ViewController,

    std::string LastListType;

    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, Automapper);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, Service);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, ListType_BeatSaver);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, ListType_BeastSaber);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, ListType_ScoreSaber);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, SortOrder);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, NEdropdown);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, MEdropdown);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, Chroma);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, Ranked);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, RankedToggle);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
)