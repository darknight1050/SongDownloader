#pragma once

#include "GlobalNamespace/LoadingControl.hpp"
#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/InputFieldView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "custom-types/shared/macros.hpp"

#include "Types/BeatSaver/Page.hpp"
#include "Types/BeastSaber/Page.hpp"
#include "Types/ScoreSaber/Page.hpp"
#include "Types/ScoreSaber/Leaderboards.hpp"

#include "songloader/shared/API.hpp"

#include "BeatSaverAPI.hpp"
#include "BeastSaberAPI.hpp"
#include "ScoreSaberAPI.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"

#include <vector>

#define MAX_SPRITES 150

#define ENTRIES_PER_PAGE 20

namespace SongDownloader {
    class SearchEntry {
        BeatSaver::Beatmap map;
        BeastSaber::Song BSsong;
        ScoreSaber::Leaderboard SSsong;
        UnityEngine::GameObject* gameObject;
        TMPro::TextMeshProUGUI* line1Component;
        TMPro::TextMeshProUGUI* line2Component;
        UnityEngine::UI::Button* downloadButton;

    public:        
        enum class MapType {
            BeatSaver,
            BeastSaber,
            ScoreSaber
        };

        float downloadProgress = -1.0f;

        bool pushed = false;

        SearchEntry(UnityEngine::GameObject* _gameObject, TMPro::TextMeshProUGUI* _line1Component, TMPro::TextMeshProUGUI* _line2Component, HMUI::ImageView* _coverImageView, UnityEngine::UI::Button* _downloadButton);

        const BeatSaver::Beatmap& GetBeatmap();

        const BeastSaber::Song& GetSongBeastSaber();

        const ScoreSaber::Leaderboard& GetSongScoreSaber();

        MapType MapType;
        
        void SetBeatmap(const BeatSaver::Beatmap& _map);

        void SetBeatmap(const BeastSaber::Song& _song);

        void SetBeatmap(const ScoreSaber::Leaderboard& _song);
        
        void UpdateDownloadProgress(bool checkLoaded);

        void Disable();

        bool IsEnabled();

        HMUI::ImageView* coverImageView;
    };

}

DECLARE_CLASS_CODEGEN(SongDownloader, DownloadSongsSearchViewController, HMUI::ViewController,

    void CreateEntries(UnityEngine::Transform* parent);

    static void Search();

    static void SetPage(int page);

    static int searchIndex;

    static int searchPage;

    static std::string SearchQuery;

    SearchEntry searchEntries[ENTRIES_PER_PAGE];
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LoadingControl*, loadingControl);

    DECLARE_INSTANCE_FIELD(HMUI::InputFieldView*, SearchField);

    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, pageIncrement);

    DECLARE_INSTANCE_METHOD(void, SearchSongs, int);

    DECLARE_INSTANCE_METHOD(void, SearchUser, int);

    DECLARE_INSTANCE_METHOD(void, SearchKey, int);

    DECLARE_INSTANCE_METHOD(void, GetCuratorRecommended, int);

    DECLARE_INSTANCE_METHOD(void, GetBookmarks, int);

    DECLARE_INSTANCE_METHOD(void, GetTrending, int);

    DECLARE_INSTANCE_METHOD(void, GetLatestRanked, int);

    DECLARE_INSTANCE_METHOD(void, GetTopPlayed, int);

    DECLARE_INSTANCE_METHOD(void, GetTopRanked, int);
)

namespace SongDownloader {
    extern DownloadSongsSearchViewController* searchViewController;
}