#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	AssetsBrowserEditContext::AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowserPanel* Assets)
		: Edit::Context(Id), Assets(Assets)
	{
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetAll()
	{
		return NxFr::ContainersUtils::ToArrayKeys(Assets->Browser->Items);
	}

	uint64 AssetsBrowserEditContext::GetCount()
	{
		return Assets->Browser->Items.GetCount();
	}

	void AssetsBrowserEditContext::Rename()
	{
		Assets->RunAction<AssetsBrowserActionRename>();
	}

	void AssetsBrowserEditContext::Duplicate()
	{
		Assets->RunAction<AssetsBrowserActionDuplicate>();
	}

	void AssetsBrowserEditContext::Delete()
	{
		Assets->RunAction<AssetsBrowserActionDelete>();
	}
}
