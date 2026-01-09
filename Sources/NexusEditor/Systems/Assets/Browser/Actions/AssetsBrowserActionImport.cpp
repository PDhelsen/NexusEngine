#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionImport.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionImport)

	AssetsBrowserActionImport::AssetsBrowserActionImport()
		: TreeAction("Import", 8, true, false)
	{
	}

	AssetsBrowserActionImport::~AssetsBrowserActionImport()
	{
	}

	void AssetsBrowserActionImport::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		bool Dirty = false;

		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
			if (Instance->GetObjectType() != AssetsBrowserItemFile::GetClassType())
			{
				continue;
			}

			AssetImporter::Run(Instance->GetPath(), NxFr::StringId());
			Dirty = true;
		}

		if (Dirty)
		{
			NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Refresh();
		}
	}
}
