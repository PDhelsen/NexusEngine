#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReferences.h"
#include "NexusEditor/Systems/Assets/ReferencesViewer/AssetsReferencesPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionReferences)

	AssetsBrowserActionReferences::AssetsBrowserActionReferences()
		: TreeAction("References", 10, false, false)
	{
	}

	AssetsBrowserActionReferences::~AssetsBrowserActionReferences()
	{
	}

	void AssetsBrowserActionReferences::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::TreeItem* Item = Items[0];

		if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
		{
			return;
		}

		AssetsReferencesPanel* Panel = NxEn::GUISystem::GetPanel<AssetsReferencesPanel>();
		Panel->Show();
		Panel->Select(Item->GetId());
	}
}
