#pragma once

#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class HierarchyEditContext : public Edit::Context
	{
	public:
		NEXUS_EDITOR_API HierarchyEditContext(NxFr::StringId Id, HierarchyPanel* Hierarchy);
		NEXUS_EDITOR_API ~HierarchyEditContext();

	protected:
		NEXUS_EDITOR_API NxFr::Array<NxFr::GUID> GetAll() override;
		NEXUS_EDITOR_API uint64 GetCount() override;

		NEXUS_EDITOR_API void Rename() override;
		NEXUS_EDITOR_API void Duplicate() override;
		NEXUS_EDITOR_API void Delete() override;
		NEXUS_EDITOR_API void Cut() override;
		NEXUS_EDITOR_API void Copy() override;
		NEXUS_EDITOR_API void Paste() override;

	private:
		NEXUS_EDITOR_API NxFr::Set<NxFr::GUID> FilterSelection();
		NEXUS_EDITOR_API void CopySelection();
		NEXUS_EDITOR_API void DestroySelection();
		NEXUS_EDITOR_API void PasteClipboard();
		NEXUS_EDITOR_API void ClearClipboard();

	private:
		HierarchyPanel* Hierarchy;
		NxEn::World* World;
	};
}

