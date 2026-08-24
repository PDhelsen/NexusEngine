#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	enum class AssetsBrowserFilter : uint16
	{
		Unfiltered = 0,

		NoDirectory = 1 << 1,
		NoAssets = 1 << 2,
		NoContent = 1 << 3,
		DirectoryOnly = NoAssets | NoContent,
		AssetsOnly = NoDirectory | NoContent,
		ContentOnly = NoDirectory | NoAssets,

		MultiSelection = 1 << 4,
		Recursive = 1 << 5,
		TopMost = 1 << 6,
		IgnoreSelected = 1 << 7,

		Sorted = 1 << 8,

		Default = MultiSelection
	};
}

NX_FLAG(NxEd::AssetsBrowserFilter, uint16)

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserEditContext : public Edit::Context
	{
		friend class AssetsBrowser;

	public:
		inline static const NxFr::StringId ContextId = "AssetsBrowserContext"_Sid;

		AssetsBrowserEditContext(AssetsBrowser* Browser);
		~AssetsBrowserEditContext();

		NxFr::Array<NxFr::GUID> FilterSelection(AssetsBrowserFilter Mode = AssetsBrowserFilter::Default, NxFr::GUID* Active = nullptr) const;

	protected:
		NxFr::Array<NxFr::GUID> GetAll() override;
		uint64 GetCount() override;

		void Create() override;
		void Rename() override;
		void Move() override;
		void Duplicate() override;
		void Delete() override;
		void Cut() override;
		void Copy() override;
		void Paste() override;

		void OnSelectionChanged(NxFr::GUID InstanceId, bool State) const override;

	private:
		AssetsBrowser* Browser;
	};
}

