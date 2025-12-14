#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionDuplicate::AssetsBrowserActionDuplicate()
		: AssetsBrowserAction("Duplicate", 2, false)
	{
	}

	AssetsBrowserActionDuplicate::~AssetsBrowserActionDuplicate()
	{
	}

	void AssetsBrowserActionDuplicate::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		Duplicate(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>(), Items);
	}

	void AssetsBrowserActionDuplicate::Duplicate(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items) const
	{
		for (auto& Item : Items)
		{
			Duplicate(Browser, Item);
		}
	}

	void AssetsBrowserActionDuplicate::Duplicate(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item) const
	{
		NxFr::String Path = Browser->ValidatePath(Item->GetPath());
		Browser->Duplicate(Item->GetPath(), Path);
	}
}
