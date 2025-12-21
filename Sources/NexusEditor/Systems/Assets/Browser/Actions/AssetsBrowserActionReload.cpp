#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReload.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionReload)

	AssetsBrowserActionReload::AssetsBrowserActionReload()
		: AssetsBrowserAction("Reload", 7, true)
	{
	}

	AssetsBrowserActionReload::~AssetsBrowserActionReload()
	{
	}

	void AssetsBrowserActionReload::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
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
