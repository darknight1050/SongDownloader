#include "CustomTypes/DownloadSongsFlowCoordinator.hpp"

#include "CustomLogger.hpp"

#include "bsml/shared/Helpers/creation.hpp"
#include "HMUI/NoTransitionsButton.hpp"
using namespace BSML::Helpers;
using namespace SongDownloader;

DEFINE_TYPE(SongDownloader, DownloadSongsFlowCoordinator);

void DownloadSongsFlowCoordinator::Awake(){
    fcInstance = this;

    if(!DownloadSongsOptionsViewController)
        DownloadSongsOptionsViewController = CreateViewController<SongDownloader::DownloadSongsOptionsViewController*>();
    if(!DownloadSongsSearchViewController)
        DownloadSongsSearchViewController = CreateViewController<SongDownloader::DownloadSongsSearchViewController*>();
    if(!DownloadSongsPlaylistViewController)
        DownloadSongsPlaylistViewController = CreateViewController<SongDownloader::DownloadSongsPlaylistViewController*>();
}

void DownloadSongsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation){
        SetTitle("Download Songs", HMUI::ViewController::AnimationType::In);
        showBackButton = true;
        ProvideInitialViewControllers(DownloadSongsSearchViewController, DownloadSongsOptionsViewController, DownloadSongsPlaylistViewController, nullptr, nullptr);
    }
}

void DownloadSongsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController){
    this->_parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
}

void DownloadSongsFlowCoordinator::Close(bool immediately) {
    this->_parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, immediately);
}

void DownloadSongsFlowCoordinator::GoToSongSelect() {
    auto songSelectButton = UnityEngine::GameObject::Find("SoloButton");
    if (!songSelectButton) {
        songSelectButton = UnityEngine::GameObject::Find("Wrapper/BeatmapWithModifiers/BeatmapSelection/EditButton");
    }
    if (!songSelectButton) {
        return;
    }
    songSelectButton->GetComponent<HMUI::NoTransitionsButton *>()->Press();
}