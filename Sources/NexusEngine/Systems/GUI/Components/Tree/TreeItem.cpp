#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(TreeItem)

	TreeItem::TreeItem()
		: ImGuiText(), Expanded(false), Selected(false)
	{
		SetTickable(true);
	}

	TreeItem::~TreeItem()
	{
	}

	TreeItem* TreeItem::GetIterator()
	{
		if (GetChild())
		{
			return GetChild();
		}

		if (GetNext())
		{
			return GetNext();
		}

		TreeItem* Parent = GetParent();
		while (Parent && !Parent->GetNext())
		{
			Parent = Parent->GetParent();
		}

		if (Parent)
		{
			return Parent->GetNext();
		}

		return nullptr;
	}
}
