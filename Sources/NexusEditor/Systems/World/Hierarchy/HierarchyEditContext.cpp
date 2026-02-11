#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	HierarchyEditContext::HierarchyEditContext(NxFr::StringId Id, HierarchyPanel* Hierarchy)
		: Edit::Context(Id), Hierarchy(Hierarchy)
	{
	}

	HierarchyEditContext::~HierarchyEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetAll()
	{
		HierarchyItem* Item = static_cast<HierarchyItem*>(Hierarchy->Root);

		NxFr::List<NxFr::GUID> Result;

		NxFr::Handle<NxEn::GameObject> Iterator = Item->GetTarget();
		while (Iterator)
		{
			Result.Append(Iterator->GetId());
			Iterator = Iterator->GetIterator();
		}

		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Result);
	}

	uint64 HierarchyEditContext::GetCount()
	{
		HierarchyItem* Item = static_cast<HierarchyItem*>(Hierarchy->Root);
		return Item->GetTarget()->GetChildCount() + 1;
	}

	void HierarchyEditContext::Rename()
	{
		Hierarchy->RunAction<HierarchyActionRename>();
	}

	void HierarchyEditContext::Duplicate()
	{
		Hierarchy->RunAction<HierarchyActionDuplicate>();
	}

	void HierarchyEditContext::Delete()
	{
		Hierarchy->RunAction<HierarchyActionDelete>();
	}
}
