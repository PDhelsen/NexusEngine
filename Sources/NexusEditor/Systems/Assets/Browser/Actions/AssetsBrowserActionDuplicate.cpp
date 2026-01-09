#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionDuplicate)

	AssetsBrowserActionDuplicate::AssetsBrowserActionDuplicate()
		: TreeAction("Duplicate", 2, false, false)
	{
	}

	AssetsBrowserActionDuplicate::~AssetsBrowserActionDuplicate()
	{
	}

	void AssetsBrowserActionDuplicate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		AssetsBrowserPanel* Browser = NxEn::GUISystem::GetPanel<AssetsBrowserPanel>();
		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
			Browser->Duplicate(Instance->GetPath(), Instance->GetPath());
		}
	}
}
