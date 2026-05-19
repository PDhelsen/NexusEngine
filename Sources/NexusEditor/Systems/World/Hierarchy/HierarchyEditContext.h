#pragma once

#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyEditContext : public Edit::Context
	{
	public:
		HierarchyEditContext(NxFr::StringId Id, HierarchyPanel* Hierarchy);
		~HierarchyEditContext();

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
		void DestroySelection();
		void PasteClipboard();
		void ClearClipboard();

	private:
		HierarchyPanel* Hierarchy;
		NxEn::World* World;
	};
}

