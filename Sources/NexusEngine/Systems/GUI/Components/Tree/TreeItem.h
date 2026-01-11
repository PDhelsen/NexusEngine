#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class TreeItem : public Object
	{
		friend class TreePanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, TreeItem)

		NEXUS_ENGINE_API TreeItem();
		NEXUS_ENGINE_API virtual ~TreeItem();

		NEXUS_ENGINE_API bool operator<(const TreeItem& Other) const;
		NEXUS_ENGINE_API bool operator<=(const TreeItem& Other) const;
		NEXUS_ENGINE_API bool operator>(const TreeItem& Other) const;
		NEXUS_ENGINE_API bool operator>=(const TreeItem& Other) const;

		NEXUS_ENGINE_API virtual NxFr::StringView GetName() const override { return ImGuiText; }
		NEXUS_ENGINE_API virtual NxFr::StringView GetDescription() const { return GetName(); }

		NEXUS_ENGINE_API virtual NxFr::StringView GetItemName() const = 0;
		NEXUS_ENGINE_API virtual NxFr::StringId GetItemType() const = 0;
		NEXUS_ENGINE_API virtual NxFr::GUID GetItemId() const = 0;

		NEXUS_ENGINE_API virtual bool IsOpened() const { return Expanded; }
		NEXUS_ENGINE_API virtual void Open(bool State) { Expanded = State; }
		NEXUS_ENGINE_API virtual bool IsSelected() const { return Selected; }
		NEXUS_ENGINE_API virtual void Select(bool State) { Selected = State; }
		NEXUS_ENGINE_API virtual bool IsLeaf() const { return GetChild() == nullptr; }

	protected:
		NEXUS_ENGINE_API virtual TreeItem* GetIterator();
		NEXUS_ENGINE_API virtual TreeItem* GetParent() const = 0;
		NEXUS_ENGINE_API virtual TreeItem* GetPrevious() const = 0;
		NEXUS_ENGINE_API virtual TreeItem* GetNext() const = 0;
		NEXUS_ENGINE_API virtual TreeItem* GetChild() const = 0;

		NEXUS_ENGINE_API virtual void CacheImGuiText() = 0;
		NEXUS_ENGINE_API virtual int8 Compare(const TreeItem& Other) const = 0;

	protected:
		NxFr::String ImGuiText;
		bool Expanded;
		bool Selected;
	};
}
