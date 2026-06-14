#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class AssetsBrowser;
	class AssetsBrowserItem;

	class NX_EDITOR_API AssetsBrowserEditContext : public Edit::Context
	{
	public:
		AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowser* Browser);
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

		AssetsBrowser* Browser;
		bool IsCutting;
	};
}

