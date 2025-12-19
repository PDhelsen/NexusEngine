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

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;

	private:
		void Duplicate(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const;
		void Duplicate(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item) const;
	};
}
