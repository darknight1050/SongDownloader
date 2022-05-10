
#include "CustomTypes/DownloadQueueViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UnityEngine/RectOffset.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Resources.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/Touchable.hpp"
#include "System/Action.hpp"

#include "GlobalNamespace/LevelBar.hpp"

#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/QuestUI.hpp"

//#include <sstream>
//#include <iosfwd>
//#include <iomanip>

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
using namespace TMPro;
using namespace GlobalNamespace;

using namespace SongDownloader;

DEFINE_TYPE(SongDownloader, DownloadQueueViewController);
DEFINE_TYPE(SongDownloader, DownloadItem);

void DownloadItem::Update() {
    if(entry && text) {
        if(entry->downloadProgress >= 100.0f && hasDone) {
            hasDone = true;
            text->SetText("Downloaded!");
            Destroy(authorText);
            if (timer > waitTime)
            {
                timer = timer - waitTime;
                Destroy(get_gameObject());
            }
        } else {
//            std::stringstream stream;
//            stream << std::fixed << std::setprecision(2) << entry->downloadProgress;
//            std::string s = stream.str();
            // C++ wont have std::pingpong until C++24, and Mathf::PingPong is stripped so here is a crude implementation
            auto t = 1.0F - std::abs(UnityEngine::Mathf::Repeat(entry->downloadProgress * 0.35f, 1.0F*2.0F) - 1.0F);
            UnityEngine::Color color = UnityEngine::Color::HSVToRGB(t, 1, 1);
            color.a = 0.35f;
            background->set_color(color);
            background->set_fillAmount(entry->downloadProgress);
//            text->SetText(s);
        }
    }
}

void DownloadQueueViewController::PushDownload(SongDownloader::SearchEntry *entry) {
    if(entry->pushed) return;

    auto imgSprite = entry->coverImageView->get_sprite();

    HorizontalLayoutGroup *horizontalLayoutGroup = BeatSaberUI::CreateHorizontalLayoutGroup(instance->scrollView->get_transform());
    auto dlItem = horizontalLayoutGroup->get_gameObject()->AddComponent<DownloadItem*>();

    auto image = BeatSaberUI::CreateImage(horizontalLayoutGroup->get_transform(), imgSprite);

    image->get_rectTransform()->set_anchoredPosition({5, 0});
    image->get_rectTransform()->set_anchorMin({0, 0});
    image->get_rectTransform()->set_anchorMax({0, 1});

    auto layoutElm = image->GetComponent<LayoutElement*>();
    if(!layoutElm) layoutElm = image->get_gameObject()->AddComponent<LayoutElement*>();
    layoutElm->GetComponent<LayoutElement*>()->set_preferredWidth(4);
    layoutElm->GetComponent<LayoutElement*>()->set_preferredHeight(4);

    auto bgImage = image->get_gameObject()->AddComponent<HMUI::ImageView*>();
    bgImage->set_enabled(true);
    bgImage->set_sprite(Sprite::CreateSprite(Texture2D::New_ctor(1, 1), {0, 0, 1, 1}, Vector2::get_one() / 2.0f, 1, 0, SpriteMeshType::_get_FullRect(), Vector4::get_one(), false));
    bgImage->set_type(UnityEngine::UI::Image::Type::_get_Filled());
    bgImage->set_fillMethod(UnityEngine::UI::Image::FillMethod::_get_Horizontal());
    bgImage->set_fillAmount(0.0f);
    bgImage->set_material(Resources::FindObjectsOfTypeAll<Material*>().First([](Material* x) { return x->get_name() == "UINoGlow"; }));

    dlItem->background = bgImage;

    auto subHorizontal = BeatSaberUI::CreateHorizontalLayoutGroup(horizontalLayoutGroup->get_transform());
    subHorizontal->set_childForceExpandWidth(true);
    subHorizontal->set_childControlWidth(false);
    subHorizontal->get_rectTransform()->set_anchoredPosition({11, 0});
    subHorizontal->GetComponent<LayoutElement*>()->set_preferredWidth(80);

    auto vert = BeatSaberUI::CreateVerticalLayoutGroup(subHorizontal->get_transform());
    vert->get_rectTransform()->set_anchoredPosition({20});
    vert->get_rectTransform()->set_anchorMin({0, 0});
    vert->get_rectTransform()->set_anchorMax({0, 0});
    vert->set_childAlignment(TextAnchor::_get_MiddleLeft());
    vert->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::_get_PreferredSize());

    dlItem->text = BeatSaberUI::CreateText(vert, string_format("<size=4>%s</size>", entry->GetBeatmap().GetName().c_str()));
    dlItem->authorText = BeatSaberUI::CreateText(vert, string_format("<size=3>%s</size>", entry->GetBeatmap().GetCurator()->c_str()));
}

void DownloadQueueViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        auto titleLayoutGroup = BeatSaberUI::CreateHorizontalLayoutGroup(get_transform());
        titleLayoutGroup->get_rectTransform()->set_anchorMin({0, 1});
        titleLayoutGroup->GetComponent<Backgroundable *>()->ApplyBackground("panel-top");
        titleLayoutGroup->set_padding(RectOffset::New_ctor(10, 10, 0, 0));

        auto text = BeatSaberUI::CreateText(titleLayoutGroup->get_transform(),"<size=8>Download Queue</size>");
        text->set_alignment(TextAlignmentOptions::Center);

        auto downloadLayoutGroup = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());

        scrollView = QuestUI::BeatSaberUI::CreateScrollView(downloadLayoutGroup->get_transform());
    }
}

void DownloadQueueViewController::DidDeactivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    this->instance = nullptr;
}