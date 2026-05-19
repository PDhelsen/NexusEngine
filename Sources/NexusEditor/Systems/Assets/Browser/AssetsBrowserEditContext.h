#pragma once

#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

namespace NxEd
{
	class AssetsBrowserEditContext : public Edit::Context
	{
	public:
		NX_EDITOR_API AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowserPanel* Assets);
		NX_EDITOR_API ~AssetsBrowserEditContext();

	protected:
		NX_EDITOR_API NxFr::Array<NxFr::GUID> GetAll() override;
		NX_EDITOR_API uint64 GetCount() override;

		NX_EDITOR_API void Rename() override;
		NX_EDITOR_API void Duplicate() override;
		NX_EDITOR_API void Delete() override;
		NX_EDITOR_API void Cut() override;
		NX_EDITOR_API void Copy() override;
		NX_EDITOR_API void Paste() override;

	private:
		NX_EDITOR_API NxFr::Set<NxFr::GUID> FilterSelection();
		NX_EDITOR_API void CopySelection();
		NX_EDITOR_API void DestroyClipboard();
		NX_EDITOR_API void PasteClipboard();
		NX_EDITOR_API void ClearClipboard();

	private:
		AssetsBrowserPanel* Assets;
		bool IsCutting;
	};
}

