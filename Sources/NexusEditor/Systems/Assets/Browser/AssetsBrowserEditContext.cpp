#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"

namespace NxEd
{
	AssetsBrowserEditContext::AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowser* Browser)
		: Edit::Context(Id), Browser(Browser)
	{
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetAll()
	{
		return NxFr::ContainersUtils::ToArrayKeys(Browser->Items);
	}

	uint64 AssetsBrowserEditContext::GetCount()
	{
		return Browser->Items.GetCount();
	}
}
