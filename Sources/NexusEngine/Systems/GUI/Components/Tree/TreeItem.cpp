#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEn
{
	NX_OBJECT_IMPLEMENTATION(TreeItem)

	TreeItem::TreeItem()
		: ImGuiText(), Expanded(false), Selected(false)
	{
	}

	TreeItem::~TreeItem()
	{
	}

	bool TreeItem::operator<(const TreeItem& Other) const
	{
		return Compare(Other) < 0;
	}

	bool TreeItem::operator<=(const TreeItem& Other) const
	{
		return Compare(Other) <= 0;
	}

	bool TreeItem::operator>(const TreeItem& Other) const
	{
		return Compare(Other) > 0;
	}

	bool TreeItem::operator>=(const TreeItem& Other) const
	{
		return Compare(Other) >= 0;
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
