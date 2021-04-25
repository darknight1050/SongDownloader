#pragma once

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

#include "DownloadSongsOptionsViewController.hpp"
#include "DownloadSongsSearchViewController.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadSongsFlowCoordinator, HMUI::FlowCoordinator,

    DECLARE_INSTANCE_FIELD(SongDownloader::DownloadSongsOptionsViewController*, DownloadSongsOptionsViewController);
    DECLARE_INSTANCE_FIELD(SongDownloader::DownloadSongsSearchViewController*, DownloadSongsSearchViewController);

    DECLARE_METHOD(void, Awake);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
 
    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(BackButtonWasPressed);
    )
)