#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReimport.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionReimport)

	AssetsBrowserActionReimport::AssetsBrowserActionReimport()
		: TreeAction("Reimport", 9, true, false)
	{
	}

	AssetsBrowserActionReimport::~AssetsBrowserActionReimport()
	{
	}

	void AssetsBrowserActionReimport::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);

			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			AssetImporter::Run(Item->GetId());
		}
	}
}
