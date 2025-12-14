#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionCreate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionCreate::AssetsBrowserActionCreate(NxFr::StringView Label, int64 Priority)
		: AssetsBrowserAction(Label, Priority)
	{
	}

	AssetsBrowserActionCreate::~AssetsBrowserActionCreate()
	{
	}

	void AssetsBrowserActionCreate::Execute(AssetsBrowserItem* Item)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);
			NxFr::StringView Directory = Item->IsDirectory() ? Item->GetPath() : Item->GetDirectory();
			NxFr::String Path = Directory.IsEmpty() ? NxFr::String(Name) : NxFr::Path::Combine(Directory, Name);
			NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Create(Path, NxFr::StringId(Type));
		});
	}
}
