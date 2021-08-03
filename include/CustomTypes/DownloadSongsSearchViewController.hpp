#pragma once

#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ImageView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "custom-types/shared/macros.hpp"

//#include "Types/BeatSaver/Page.hpp"
#include "Types/BeatSaver/Page.hpp"

#include <vector>

#define MAX_SPRITES 150

#define ENTRIES_PER_PAGE 20

namespace SongDownloader {
    class SearchEntry {
        BeatSaver::Beatmap map;
        UnityEngine::GameObject* gameObject;
        TMPro::TextMeshProUGUI* line1Component;
        TMPro::TextMeshProUGUI* line2Component;
        HMUI::ImageView* coverImageView;
        UnityEngine::UI::Button* downloadButton;

    public:

        static int spriteCount;

        float downloadProgress = -1.0f;

        SearchEntry(UnityEngine::GameObject* _gameObject, TMPro::TextMeshProUGUI* _line1Component, TMPro::TextMeshProUGUI* _line2Component, HMUI::ImageView* _coverImageView, UnityEngine::UI::Button* _downloadButton);

        const BeatSaver::Beatmap& GetBeatmap();
        
        void SetBeatmap(const BeatSaver::Beatmap& _map);
        
        void UpdateDownloadProgress(bool checkLoaded);

        void Disable();

        bool IsEnabled();

    }; 

}

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadSongsSearchViewController, HMUI::ViewController,

    void CreateEntries(UnityEngine::Transform* parent);

    static int searchIndex;

    SearchEntry searchEntries[ENTRIES_PER_PAGE];
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);

)