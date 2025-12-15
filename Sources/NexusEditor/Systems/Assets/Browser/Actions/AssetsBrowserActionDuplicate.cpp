#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionDuplicate)

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
		Browser->Duplicate(Item->GetPath(), Item->GetPath());
	}
}
