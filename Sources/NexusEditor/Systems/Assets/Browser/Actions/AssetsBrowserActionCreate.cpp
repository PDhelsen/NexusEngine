#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionCreate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionCreate::AssetsBrowserActionCreate()
		: AssetsBrowserAction("Create", 1, false)
	{
	}

	AssetsBrowserActionCreate::~AssetsBrowserActionCreate()
	{
	}

	void AssetsBrowserActionCreate::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);
			Create(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>(), Items, Name, NxFr::StringId(Type));
		});
	}

	void AssetsBrowserActionCreate::Create(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items, NxFr::StringView Name, NxFr::StringId Type) const
	{
		for (auto& Item : Items)
		{
			Create(Browser, Item, Name, Type);
		}
	}

	void AssetsBrowserActionCreate::Create(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item, NxFr::StringView Name, NxFr::StringId Type) const
	{
		NxFr::StringView Directory = Item->IsDirectory() ? Item->GetPath() : Item->GetDirectory();
		NxFr::String Path = NxFr::Path::Combine(Directory, Name);
		Path = Browser->ValidatePath(Path);

		Browser->Create(Path, Type);
	}
}
