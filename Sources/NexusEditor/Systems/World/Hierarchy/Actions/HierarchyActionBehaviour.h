#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class HierarchyActionBehaviour : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionBehaviour)

		NEXUS_EDITOR_API HierarchyActionBehaviour();
		NEXUS_EDITOR_API virtual ~HierarchyActionBehaviour();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}
