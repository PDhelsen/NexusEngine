#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"

namespace NxEd
{
	HierarchyEditContext::HierarchyEditContext(NxFr::StringId Id, HierarchyManager* Hierarchy)
		: Edit::Context(Id), Hierarchy(Hierarchy)
	{
	}

	HierarchyEditContext::~HierarchyEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetAll()
	{
		return NxFr::ContainersUtils::ToArrayKeys(Hierarchy->Items);
	}

	uint64 HierarchyEditContext::GetCount()
	{
		return Hierarchy->Items.GetCount();
	}
}
