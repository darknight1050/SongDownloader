#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "custom-types/shared/register.hpp"

#include "bsml/shared/BSML.hpp"

#include "CustomLogger.hpp"
#include "ModConfig.hpp"

#include "CustomTypes/DownloadSongsFlowCoordinator.hpp"

modloader::ModInfo modInfo{MOD_ID, VERSION, VERSION_LONG};

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

extern "C" void setup(CModInfo* info) {
    info->id = "SongDownloader";
    info->version = VERSION;
    info->version_long = VERSION_LONG;

    getModConfig().Init(modInfo);
}

extern "C" void late_load() {
    LOG_INFO("Starting SongDownloader installation...");
    il2cpp_functions::Init();
    BSML::Init();
    custom_types::Register::AutoRegister();
    BSML::Register::RegisterMainMenu<SongDownloader::DownloadSongsFlowCoordinator*>("SongDownloader", "More Songs");
    LOG_INFO("Successfully installed SongDownloader!");
}
