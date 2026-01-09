#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReload.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionReload)

	AssetsBrowserActionReload::AssetsBrowserActionReload()
		: TreeAction("Reload", 7, true, false)
	{
	}

	AssetsBrowserActionReload::~AssetsBrowserActionReload()
	{
	}

	void AssetsBrowserActionReload::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			System->Reload(Item->GetId());
		}
	}
}
