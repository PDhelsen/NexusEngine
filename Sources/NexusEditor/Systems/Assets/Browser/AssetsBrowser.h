#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetsBrowserItem;

	namespace AssetsBrowser
	{
		NEXUS_EDITOR_API extern const NxFr::String Root;
		NEXUS_EDITOR_API extern const NxFr::String RootLabel;

		NEXUS_EDITOR_API void Create(NxFr::StringView Path, NxFr::StringId Type);
		NEXUS_EDITOR_API void Move(NxFr::StringView Path, NxFr::StringView Target);
		NEXUS_EDITOR_API void Duplicate(NxFr::StringView Path, NxFr::StringView Target);
		NEXUS_EDITOR_API void Delete(NxFr::StringView Path);

		NEXUS_EDITOR_API bool Exist(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::StringView ValidatePath(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::String EnsureUniquePath(NxFr::String Path);

		NEXUS_EDITOR_API AssetsBrowserItem* GetItem(NxFr::GUID Id);
		NEXUS_EDITOR_API AssetsBrowserItem* GetParent(NxFr::StringView Path);
	}
}
