#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReferences.h"
#include "NexusEditor/Systems/Assets/ReferencesViewer/AssetsReferencesPanel.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionReferences)

	AssetsBrowserActionReferences::AssetsBrowserActionReferences()
		: AssetsBrowserAction("References", 9, true)
	{
	}

	AssetsBrowserActionReferences::~AssetsBrowserActionReferences()
	{
	}

	void AssetsBrowserActionReferences::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		AssetsBrowserItem* Item = Items[0];

		if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
		{
			return;
		}

		AssetsReferencesPanel* Panel = NxEn::GUISystem::GetPanel<AssetsReferencesPanel>();
		Panel->Show();
		Panel->Select(Item->GetId());
	}
}
