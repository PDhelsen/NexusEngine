#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionImport.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	AssetsBrowserActionImport::AssetsBrowserActionImport()
		: AssetsBrowserAction("Import", 7, true)
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

			AssetImporter::Run(AssetImporter::TryGetImporterId(Item->GetPath()), Item->GetPath());
			Dirty = true;
		}

		if (Dirty)
		{
			NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Refresh();
		}
	}
}
