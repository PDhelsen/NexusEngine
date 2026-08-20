#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class AssetsBrowser;
	class AssetsBrowserItem;

	class NX_EDITOR_API AssetsBrowserEditContext : public Edit::Context
	{
		friend class AssetsBrowser;

	public:
		inline static const NxFr::StringId ContextId = "AssetsBrowserContext"_Sid;

	protected:
		NxFr::Array<NxFr::GUID> GetAll() override;
		uint64 GetCount() override;

		void Create() override;
		void Rename() override;
		void Duplicate() override;
		void Delete() override;
		void Paste() override;

		NxFr::Array<NxFr::GUID> GetSelection(bool Filtered) const override;
		void OnSelectionChanged(NxFr::GUID InstanceId, bool State) const override;

	private:
		AssetsBrowserEditContext(AssetsBrowser* Browser);
		~AssetsBrowserEditContext();

		AssetsBrowser* Browser;
		bool IsCutting;
	};
}

