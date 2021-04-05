#pragma once
#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadSongsViewController, HMUI::ViewController,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    DECLARE_METHOD(void, Update);

    REGISTER_FUNCTION(DownloadSongsViewController,

        REGISTER_METHOD(DidActivate);
        
        REGISTER_METHOD(Update);

    )
)