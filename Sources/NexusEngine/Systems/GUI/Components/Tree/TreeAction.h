#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEn
{
	class TreeAction : public Object
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, TreeAction)

		NX_ENGINE_API TreeAction(NxFr::StringView Label, int64 Priority, bool Recursive, bool LastSelectedFirst);
		NX_ENGINE_API virtual ~TreeAction();

		NX_ENGINE_API virtual void Execute(const NxFr::Array<TreeItem*>& Items) = 0;

		NX_ENGINE_API NxFr::StringView GetName() const override { return Label; }
		NX_ENGINE_API int64 GetPriority() const { return Priority; }
		NX_ENGINE_API bool IsRecursive() const { return Recursive; }
		NX_ENGINE_API bool IsLastSelectedFirst() const { return LastSelectedFirst; }

	private:
		NxFr::String Label;
		int64 Priority;
		bool Recursive;
		bool LastSelectedFirst;
	};
}

