#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/Tree/TreeItem.h"

namespace NxEn
{
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
