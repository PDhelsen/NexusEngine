#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionLoad.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionLoad)

	AssetsBrowserActionLoad::AssetsBrowserActionLoad()
		: TreeAction("Load", 6, true, false)
	{
	}

	AssetsBrowserActionLoad::~AssetsBrowserActionLoad()
	{
	}

	void AssetsBrowserActionLoad::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			System->Load(Item->GetId());
		}
	}
}
