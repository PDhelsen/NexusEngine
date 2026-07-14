#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class HierarchyManager;
	class HierarchyItem;

	class NX_EDITOR_API HierarchyEditContext : public Edit::Context
	{
		friend class HierarchyManager;

	protected:
		NxFr::Array<NxFr::GUID> GetAll() override;
		uint64 GetCount() override;

		void Rename() override;
		void Duplicate() override;
		void Delete() override;
		void Cut() override;
		void Copy() override;
		void Paste() override;

		NxFr::Set<NxFr::GUID> FilterSelection();
		void OnSelectItem(NxFr::GUID Id, bool State);

	private:
		HierarchyEditContext(HierarchyManager* Manager, NxFr::StringId Id);
		~HierarchyEditContext();

		HierarchyManager* Manager;
		bool IsCutting;
	};
}

