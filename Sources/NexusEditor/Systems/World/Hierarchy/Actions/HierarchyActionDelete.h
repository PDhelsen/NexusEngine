#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionDelete : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionDelete)

		NEXUS_EDITOR_API HierarchyActionDelete();
		NEXUS_EDITOR_API virtual ~HierarchyActionDelete();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;

	private:
		void Delete(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const;
		void Delete(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item) const;
	};
}
