#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	enum class HierarchyFilter : uint16
	{
		Unfiltered = 0,

		MultiSelection = 1 << 1,
		Recursive = 1 << 2,
		TopMost = 1 << 3,
		IgnoreSelected = 1 << 4,

		Sorted = 1 << 5,

		Default = MultiSelection
	};
}

NX_FLAG(NxEd::HierarchyFilter, uint16)

namespace NxEd
{
	class NX_EDITOR_API HierarchyEditContext : public Edit::Context
	{
		friend class HierarchyManager;
		friend class HierarchyPanel;

	public:
		inline static const NxFr::StringId ContextId = "HierarchyContext"_Sid;

		HierarchyEditContext(HierarchyManager* Manager, HierarchyPanel* Panel, NxFr::StringId Id);
		~HierarchyEditContext();

		NxFr::Array<NxFr::GUID> FilterSelection(HierarchyFilter Mode = HierarchyFilter::Default, NxFr::GUID* Active = nullptr) const;

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

	private:
		HierarchyManager* Manager;
		HierarchyPanel* Panel;
	};
}

