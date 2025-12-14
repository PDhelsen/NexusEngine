#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDelete.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionDelete::AssetsBrowserActionDelete(NxFr::StringView Label, int64 Priority)
		: AssetsBrowserAction(Label, Priority)
	{
	}

	AssetsBrowserActionDelete::~AssetsBrowserActionDelete()
	{
	}

	void AssetsBrowserActionDelete::Execute(AssetsBrowserItem* Item)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Delete(Item->GetPath());
	}
}
