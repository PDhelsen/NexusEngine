#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDelete.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionDelete)

	AssetsBrowserActionDelete::AssetsBrowserActionDelete()
		: TreeAction("Delete", 5, false, false)
	{
	}

	AssetsBrowserActionDelete::~AssetsBrowserActionDelete()
	{
	}

	void AssetsBrowserActionDelete::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxFr::Array<NxFr::String> Paths(Items.GetCount());
		for (uint64 Index = 0; Index < Paths.GetCount(); ++Index)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[Index]);
			Paths[Index] = Instance->GetPath();
		}

		AssetsBrowserPanel* Browser = NxEn::GUISystem::GetPanel<AssetsBrowserPanel>();
		for (auto& Item : Paths)
		{
			if (Browser->Exist(Item))
			{
				Browser->Delete(Item);
			}
		}
	}
}
