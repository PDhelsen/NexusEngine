#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionCreate : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionCreate)

		NEXUS_EDITOR_API HierarchyActionCreate();
		NEXUS_EDITOR_API virtual ~HierarchyActionCreate();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::GameObject*>& Items) override;

	private:
		void Create(NxEn::World* World, const NxFr::Array<NxEn::GameObject*>& Items, NxFr::StringView Name) const;
		void Create(NxEn::World* World, NxEn::GameObject* Item, NxFr::StringView Name) const;
	};
}
