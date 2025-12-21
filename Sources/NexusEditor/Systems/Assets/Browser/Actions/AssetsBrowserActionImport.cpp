#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionImport.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionImport)

	AssetsBrowserActionImport::AssetsBrowserActionImport()
		: AssetsBrowserAction("Import", 8, true)
	{
	}

	AssetsBrowserActionImport::~AssetsBrowserActionImport()
	{
	}

	void AssetsBrowserActionImport::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		bool Dirty = false;

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemFile::GetClassType())
			{
				continue;
			}

			AssetImporter::Run(Item->GetPath(), NxFr::StringId());
			Dirty = true;
		}

		if (Dirty)
		{
			NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Refresh();
		}
	}
}
