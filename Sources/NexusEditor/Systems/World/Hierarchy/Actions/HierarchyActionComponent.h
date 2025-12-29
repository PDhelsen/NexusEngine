#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionComponent : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionComponent)

		NEXUS_EDITOR_API HierarchyActionComponent();
		NEXUS_EDITOR_API virtual ~HierarchyActionComponent();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;
	};
}
