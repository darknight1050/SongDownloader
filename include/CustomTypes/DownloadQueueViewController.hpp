#pragma once

#include "custom-types/shared/macros.hpp"
#include "DownloadSongsSearchViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadQueueViewController, HMUI::ViewController,
      DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
      DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

      DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, scrollView);
public:
    static SongDownloader::DownloadQueueViewController* instance;
    static void PushDownload(SongDownloader::SearchEntry* entry);
)

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadItem, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Update);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, text);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, authorText);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, background);
public:
    SongDownloader::SearchEntry* entry;
    bool hasDone;
    float timer;
    float waitTime = 2.0f;
)

