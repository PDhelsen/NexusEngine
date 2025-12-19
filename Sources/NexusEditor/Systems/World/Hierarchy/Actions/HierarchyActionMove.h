#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionMove : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionMove)

		NEXUS_EDITOR_API HierarchyActionMove();
		NEXUS_EDITOR_API virtual ~HierarchyActionMove();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::GameObject*>& Items) override;

	private:
		void Move(NxEn::World* World, const NxFr::Array<NxEn::GameObject*>& Items, NxEn::GameObject* Parent) const;
		void Move(NxEn::World* World, NxEn::GameObject* Item, NxEn::GameObject* Parent) const;
	};
}
