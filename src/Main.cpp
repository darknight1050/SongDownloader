#include <chrono>

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "custom-types/shared/register.hpp"

#include "questui/shared/QuestUI.hpp"

#include "CustomLogger.hpp"

#include "WebUtil.hpp"

#include "Types/BeatSaver/Beatmap.hpp"

ModInfo modInfo;

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true)); 
    return *logger; 
}

extern "C" void setup(ModInfo& info) {
    modInfo.id = "SongDownloader";
    modInfo.version = VERSION;
    info = modInfo;


    
    std::string data;
    auto ret = WebUtil::Get("https://beatsaver.com/api/maps/by-hash/8e7e553099436af31564adf1977a5ec42a61cfff", data);

    LOG_INFO("Ret code %d: %s", ret, data.c_str());

    rapidjson::Document document;
    document.Parse(data);
    BeatSaver::Beatmap map;
    map.Deserialize(document.GetObject());

    LOG_INFO("Name %s", map.GetName().c_str());
    LOG_INFO("Hash %s", map.GetHash().c_str());
    LOG_INFO("Description %s", map.GetDescription().value().c_str());
    LOG_INFO("Uploader Id %s", map.GetUploader().GetId().c_str());
    LOG_INFO("Uploader Name %s", map.GetUploader().GetUsername().c_str());
    LOG_INFO("Downloads %d", map.GetStats().GetDownloads());
}

extern "C" void load() {
    LOG_INFO("Starting SongDownloader installation...");
    il2cpp_functions::Init();
    QuestUI::Init();
    LOG_INFO("Successfully installed SongDownloader!");
}