#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionDuplicate : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionDuplicate)

		NEXUS_EDITOR_API HierarchyActionDuplicate();
		NEXUS_EDITOR_API virtual ~HierarchyActionDuplicate();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::GameObject*>& Items) override;

	private:
		void Duplicate(NxEn::World* World, const NxFr::Array<NxEn::GameObject*>& Items) const;
		void Duplicate(NxEn::World* World, NxEn::GameObject* Item) const;
	};
}
