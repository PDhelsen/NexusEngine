#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class NX_ENGINE_API TreeItem : public Object
	{
		friend class TreePanel;

	public:
		NX_OBJECT(TreeItem)

		TreeItem();
		virtual ~TreeItem();

		bool operator<(const TreeItem& Other) const;
		bool operator<=(const TreeItem& Other) const;
		bool operator>(const TreeItem& Other) const;
		bool operator>=(const TreeItem& Other) const;

		virtual NxFr::StringView GetName() const override { return ImGuiText; }
		virtual NxFr::StringView GetDescription() const { return GetName(); }

		virtual NxFr::StringView GetItemName() const = 0;
		virtual NxFr::StringId GetItemType() const = 0;
		virtual NxFr::GUID GetItemId() const = 0;

		virtual bool IsOpened() const { return Expanded; }
		virtual void Open(bool State) { Expanded = State; }
		virtual bool IsSelected() const { return Selected; }
		virtual void Select(bool State) { Selected = State; }
		virtual bool IsLeaf() const { return GetChild() == nullptr; }

	protected:
		virtual TreeItem* GetIterator();
		virtual TreeItem* GetParent() const = 0;
		virtual TreeItem* GetPrevious() const = 0;
		virtual TreeItem* GetNext() const = 0;
		virtual TreeItem* GetChild() const = 0;

		virtual void CacheImGuiText() = 0;
		virtual int8 Compare(const TreeItem& Other) const = 0;

	protected:
		NxFr::String ImGuiText;
		bool Expanded;
		bool Selected;
	};
}
