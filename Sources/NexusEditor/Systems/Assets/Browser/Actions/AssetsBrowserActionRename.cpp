#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionRename.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionRename::AssetsBrowserActionRename()
		: AssetsBrowserAction("Rename", 3, false)
	{
	}

	AssetsBrowserActionRename::~AssetsBrowserActionRename()
	{
	}

	void AssetsBrowserActionRename::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Rename(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>(), Items, Input);
		});
	}

	void AssetsBrowserActionRename::Rename(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items, NxFr::StringView Input) const
	{
		for (auto& Item : Items)
		{
			Rename(Browser, Item, Input);
		}
	}

	void AssetsBrowserActionRename::Rename(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item, NxFr::StringView Input) const
	{
		NxFr::String Path = Item->GetPath();
		NxFr::String Parent = Item->GetDirectory();

		if (Item->IsDirectory())
		{
			Path = NxFr::Path::Combine((NxFr::StringView)Parent, Input);
		}
		else
		{
			Path = NxFr::Path::Combine((NxFr::StringView)Parent, (Input + "." + Item->GetExtension()));
		}

		Path = Browser->ValidatePath(Path);
		Browser->Move(Item->GetPath(), Path);
	}

}
