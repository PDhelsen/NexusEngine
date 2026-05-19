#pragma once

#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserEditContext : public Edit::Context
	{
	public:
		AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowserPanel* Assets);
		~AssetsBrowserEditContext();

	protected:
		NxFr::Array<NxFr::GUID> GetAll() override;
		uint64 GetCount() override;

		void Rename() override;
		void Duplicate() override;
		void Delete() override;
		void Cut() override;
		void Copy() override;
		void Paste() override;

	private:
		NxFr::Set<NxFr::GUID> FilterSelection();
		void CopySelection();
		void DestroyClipboard();
		void PasteClipboard();
		void ClearClipboard();

	private:
		AssetsBrowserPanel* Assets;
		bool IsCutting;
	};
}

