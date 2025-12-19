#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	class HierarchyPanel;

	class HierarchyAction : public NxEn::Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyAction)

		NEXUS_EDITOR_API HierarchyAction(NxFr::StringView Label, int64 Priority, bool Recursive = false, bool LastSelectedFirst = false);
		NEXUS_EDITOR_API virtual ~HierarchyAction();

		NEXUS_EDITOR_API bool operator<=(const HierarchyAction& Other);

		NEXUS_EDITOR_API virtual void Execute(const NxFr::Array<NxEn::GameObject*>& Instances) = 0;

		NEXUS_EDITOR_API NxFr::StringView GetLabel() const { return Label; }
		NEXUS_EDITOR_API int64 GetPriority() const { return Priority; }
		NEXUS_EDITOR_API bool IsRecursive() const { return Recursive; }
		NEXUS_EDITOR_API bool IsLastSelectedFirst() const { return LastSelectedFirst; }

	private:
		NxFr::String Label;
		int64 Priority;
		bool Recursive;
		bool LastSelectedFirst;
	};
}
