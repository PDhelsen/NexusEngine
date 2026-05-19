#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class TreeItem : public Object
	{
		friend class TreePanel;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, TreeItem)

		NX_ENGINE_API TreeItem();
		NX_ENGINE_API virtual ~TreeItem();

		NX_ENGINE_API bool operator<(const TreeItem& Other) const;
		NX_ENGINE_API bool operator<=(const TreeItem& Other) const;
		NX_ENGINE_API bool operator>(const TreeItem& Other) const;
		NX_ENGINE_API bool operator>=(const TreeItem& Other) const;

		NX_ENGINE_API virtual NxFr::StringView GetName() const override { return ImGuiText; }
		NX_ENGINE_API virtual NxFr::StringView GetDescription() const { return GetName(); }

		NX_ENGINE_API virtual NxFr::StringView GetItemName() const = 0;
		NX_ENGINE_API virtual NxFr::StringId GetItemType() const = 0;
		NX_ENGINE_API virtual NxFr::GUID GetItemId() const = 0;

		NX_ENGINE_API virtual bool IsOpened() const { return Expanded; }
		NX_ENGINE_API virtual void Open(bool State) { Expanded = State; }
		NX_ENGINE_API virtual bool IsSelected() const { return Selected; }
		NX_ENGINE_API virtual void Select(bool State) { Selected = State; }
		NX_ENGINE_API virtual bool IsLeaf() const { return GetChild() == nullptr; }

	protected:
		NX_ENGINE_API virtual TreeItem* GetIterator();
		NX_ENGINE_API virtual TreeItem* GetParent() const = 0;
		NX_ENGINE_API virtual TreeItem* GetPrevious() const = 0;
		NX_ENGINE_API virtual TreeItem* GetNext() const = 0;
		NX_ENGINE_API virtual TreeItem* GetChild() const = 0;

		NX_ENGINE_API virtual void CacheImGuiText() = 0;
		NX_ENGINE_API virtual int8 Compare(const TreeItem& Other) const = 0;

	protected:
		NxFr::String ImGuiText;
		bool Expanded;
		bool Selected;
	};
}
