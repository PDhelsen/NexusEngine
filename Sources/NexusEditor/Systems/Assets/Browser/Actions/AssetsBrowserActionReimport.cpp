#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReimport.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionReimport)

	AssetsBrowserActionReimport::AssetsBrowserActionReimport()
		: AssetsBrowserAction("Reimport", 8, true)
	{
	}

	AssetsBrowserActionReimport::~AssetsBrowserActionReimport()
	{
	}

	void AssetsBrowserActionReimport::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			AssetImporter::Run(Item->GetId());
		}
	}
}
