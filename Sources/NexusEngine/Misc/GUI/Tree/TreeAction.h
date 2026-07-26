#pragma once

#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Misc/GUI/Tree/TreeItem.h"

namespace NxEn
{
	class NX_ENGINE_API TreeAction : public Object
	{
	public:
		NX_OBJECT(TreeAction)

		TreeAction(NxFr::StringView Label, int64 Priority, bool Recursive, bool LastSelectedFirst);
		virtual ~TreeAction();

		virtual void Execute(const NxFr::Array<TreeItem*>& Items) = 0;

		NxFr::StringView GetName() const override { return Label; }
		int64 GetPriority() const { return Priority; }
		bool IsRecursive() const { return Recursive; }
		bool IsLastSelectedFirst() const { return LastSelectedFirst; }

	private:
		NxFr::String Label;
		int64 Priority;
		bool Recursive;
		bool LastSelectedFirst;
	};
}

