#include <chrono>

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "custom-types/shared/register.hpp"

#include "questui/shared/QuestUI.hpp"

#include "songloader/shared/API.hpp"

#include "CustomLogger.hpp"

#include "DownloadSongsViewController.hpp"

#include "WebUtil.hpp"

#include "BeatSaverAPI.hpp"

ModInfo modInfo;

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true)); 
    return *logger; 
}

extern "C" void setup(ModInfo& info) {
    modInfo.id = "SongDownloader";
    modInfo.version = VERSION;
    info = modInfo;
}

std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*> LoadedSongs;

extern "C" void load() {
    LOG_INFO("Starting SongDownloader installation...");
    il2cpp_functions::Init();
    QuestUI::Init();
    custom_types::Register::RegisterTypes<SongDownloader::DownloadSongsViewController>();
    QuestUI::Register::RegisterModSettingsViewController<SongDownloader::DownloadSongsViewController*>(modInfo);
    RuntimeSongLoader::API::AddSongsLoadedEvent(
        [] (const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& loadedSongs) {
            LoadedSongs = loadedSongs;
        }
    );
    LOG_INFO("Successfully installed SongDownloader!");
}