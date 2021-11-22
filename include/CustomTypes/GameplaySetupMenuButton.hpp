#pragma once

#include "UnityEngine/MonoBehaviour.hpp"

#include "DownloadSongsFlowCoordinator.hpp"
#include "HMUI/FlowCoordinator.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(SongDownloader, GameplaySetupMenuButton, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(HMUI::FlowCoordinator*, currentFlowCoordinator);
    DECLARE_INSTANCE_FIELD(SongDownloader::DownloadSongsFlowCoordinator*, downloadSongsFlowCoordinator);

    //DECLARE_INSTANCE_METHOD(void, Awake);

    DECLARE_INSTANCE_METHOD(void, DidActivate, bool first);

    //DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    //
    //DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
)