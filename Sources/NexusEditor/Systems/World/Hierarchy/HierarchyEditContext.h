#pragma once

#include "NexusEditor/Systems/Edit/EditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class HierarchyEditContext : public Edit::Context
	{
	public:
		NEXUS_EDITCONTEXT_DECLARATION(NEXUS_EDITOR_API, World)

		NEXUS_EDITOR_API HierarchyEditContext(NxFr::StringId Id, HierarchyPanel* Hierarchy);
		NEXUS_EDITOR_API ~HierarchyEditContext();

	protected:
		NEXUS_EDITOR_API NxFr::Array<NxFr::GUID> GetAll() override;
		NEXUS_EDITOR_API uint64 GetCount() override;

	private:
		HierarchyPanel* Hierarchy;
	};
}

