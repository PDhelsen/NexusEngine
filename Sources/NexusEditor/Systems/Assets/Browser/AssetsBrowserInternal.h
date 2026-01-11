#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	namespace AssetsBrowser
	{
		NEXUS_EDITOR_API extern const NxFr::String RootPath;

		NEXUS_EDITOR_API AssetsBrowserItem* FetchItems();
		NEXUS_EDITOR_API AssetsBrowserItem* FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent);
		NEXUS_EDITOR_API AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item);

		NEXUS_EDITOR_API AssetsBrowserItem* AppendItem(NxFr::StringView Path);
		NEXUS_EDITOR_API void RemoveItem(AssetsBrowserItem* Item);
		NEXUS_EDITOR_API void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool AddId, bool RemoveId);

		NEXUS_EDITOR_API AssetsBrowserItem* DuplicateItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent = nullptr);
		NEXUS_EDITOR_API void AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent);
		NEXUS_EDITOR_API void DetachItem(AssetsBrowserItem* Item);
		NEXUS_EDITOR_API void SortItem(AssetsBrowserItem* Item, bool Recursive = false);

		NEXUS_EDITOR_API NxFr::StringView IdToPath(NxFr::GUID Id);
		NEXUS_EDITOR_API NxFr::GUID PathToId(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::String DiskToPath(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::String PathToDisk(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::String PathToAsset(NxFr::StringView Path);
	}
}
