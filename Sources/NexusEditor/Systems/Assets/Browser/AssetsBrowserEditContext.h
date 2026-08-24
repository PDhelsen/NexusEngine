#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	enum class AssetsBrowserFilter : uint8
	{
		Unfiltered = 0,

		TopMost = 1 << 1,
		Recursive = 1 << 2,
		MultiSelection = 1 << 3,
		NoDirectory = 1 << 4,
		Sorted = 1 << 5,
	};
}

NX_FLAG(NxEd::AssetsBrowserFilter, uint8)

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserEditContext : public Edit::Context
	{
		friend class AssetsBrowser;

	public:
		inline static const NxFr::StringId ContextId = "AssetsBrowserContext"_Sid;

		AssetsBrowserEditContext(AssetsBrowser* Browser);
		~AssetsBrowserEditContext();

		NxFr::Array<NxFr::GUID> FilterSelection(AssetsBrowserFilter Mode = AssetsBrowserFilter::Unfiltered, NxFr::GUID* Active = nullptr) const;

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

		void Import() const;
		void Load() const;
		void Instantiate() const;
		void View() const;

	private:
		AssetsBrowser* Browser;
	};
}

