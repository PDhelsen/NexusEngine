#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEn
{
	class TreeAction : public Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, TreeAction)

		NEXUS_ENGINE_API TreeAction(NxFr::StringView Label, int64 Priority, bool Recursive, bool LastSelectedFirst);
		NEXUS_ENGINE_API virtual ~TreeAction();

		NEXUS_ENGINE_API virtual void Execute(const NxFr::Array<TreeItem*>& Items) = 0;

		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Label; }
		NEXUS_ENGINE_API int64 GetPriority() const { return Priority; }
		NEXUS_ENGINE_API bool IsRecursive() const { return Recursive; }
		NEXUS_ENGINE_API bool IsLastSelectedFirst() const { return LastSelectedFirst; }

	private:
		NxFr::String Label;
		int64 Priority;
		bool Recursive;
		bool LastSelectedFirst;
	};
}

