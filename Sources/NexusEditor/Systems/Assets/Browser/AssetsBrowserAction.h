#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	class AssetsBrowserPanel;

	class AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserAction(NxFr::StringView Label, int64 Priority, bool Recursive);
		NEXUS_EDITOR_API virtual ~AssetsBrowserAction();

		NEXUS_EDITOR_API bool operator<=(const AssetsBrowserAction& Other);

		NEXUS_EDITOR_API virtual void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) = 0;

		NEXUS_EDITOR_API NxFr::StringView GetLabel() const { return Label; }
		NEXUS_EDITOR_API int64 GetPriority() const { return Priority; }
		NEXUS_EDITOR_API bool IsRecursive() const { return Recursive; }

	private:
		NxFr::String Label;
		int64 Priority;
		bool Recursive;
	};
}
