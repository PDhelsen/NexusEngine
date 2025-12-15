#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDelete.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionDelete)

	AssetsBrowserActionDelete::AssetsBrowserActionDelete()
		: AssetsBrowserAction("Delete", 5, false)
	{
	}

	AssetsBrowserActionDelete::~AssetsBrowserActionDelete()
	{
	}

	void AssetsBrowserActionDelete::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		NxFr::Array<NxFr::String> Paths(Items.GetCount());
		for (uint64 Index = 0; Index < Paths.GetCount(); ++Index)
		{
			Paths[Index] = Items[Index]->GetPath();
		}

		Delete(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>(), Paths);
	}

	void AssetsBrowserActionDelete::Delete(AssetsBrowserPanel* Browser, const NxFr::Array<NxFr::String>& Items) const
	{
		for (auto& Item : Items)
		{
			Delete(Browser, Item);
		}
	}
	void AssetsBrowserActionDelete::Delete(AssetsBrowserPanel* Browser, NxFr::String Item) const
	{
		if (Browser->Exist(Item))
		{
			Browser->Delete(Item);
		}
	}
}
