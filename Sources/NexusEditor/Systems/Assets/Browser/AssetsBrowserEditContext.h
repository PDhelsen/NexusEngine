#pragma once

#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	class AssetsBrowserEditContext : public Edit::Context
	{
	public:
		NEXUS_EDITCONTEXT_DECLARATION(NEXUS_EDITOR_API, Assets)

		NEXUS_EDITOR_API AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowser* Browser);
		NEXUS_EDITOR_API ~AssetsBrowserEditContext();

	protected:
		NEXUS_EDITOR_API NxFr::Array<NxFr::GUID> GetAll() override;
		NEXUS_EDITOR_API uint64 GetCount() override;

	private:
		AssetsBrowser* Browser;
	};
}

