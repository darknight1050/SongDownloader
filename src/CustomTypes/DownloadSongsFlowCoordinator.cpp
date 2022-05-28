#include "CustomTypes/DownloadSongsFlowCoordinator.hpp"

#include "CustomLogger.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

using namespace QuestUI;
using namespace SongDownloader;

DEFINE_TYPE(SongDownloader, DownloadSongsFlowCoordinator);

void DownloadSongsFlowCoordinator::Awake(){
    if(!DownloadSongsOptionsViewController)
        DownloadSongsOptionsViewController = BeatSaberUI::CreateViewController<SongDownloader::DownloadSongsOptionsViewController*>();
    if(!DownloadSongsSearchViewController)
        DownloadSongsSearchViewController = BeatSaberUI::CreateViewController<SongDownloader::DownloadSongsSearchViewController*>();
    if(!DownloadQueueViewController)
        DownloadQueueViewController = BeatSaberUI::CreateViewController<SongDownloader::DownloadQueueViewController*>();
}

void DownloadSongsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation){
        SetTitle(il2cpp_utils::newcsstr("Download Songs"), HMUI::ViewController::AnimationType::In);
        showBackButton = true;
        ProvideInitialViewControllers(DownloadSongsSearchViewController, DownloadSongsOptionsViewController, nullptr, DownloadQueueViewController, nullptr);
    }
}

void DownloadSongsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController){
    this->parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
}