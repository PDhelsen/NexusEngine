#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionLoad.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionLoad)

	AssetsBrowserActionLoad::AssetsBrowserActionLoad()
		: AssetsBrowserAction("Load", 6, true)
	{
	}

	AssetsBrowserActionLoad::~AssetsBrowserActionLoad()
	{
	}

	void AssetsBrowserActionLoad::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
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
