#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserItem : public NxEn::TreeItem
	{
		friend class AssetsBrowser;
		friend class AssetsBrowserEditContext;

	public:
		NX_OBJECT_DECLARATION(AssetsBrowserItem)

		NxFr::StringView GetName() const override { return ImGuiText; }
		NxFr::StringView GetDescription() const { return GetItemName(); }

		NxFr::StringView GetItemName() const override { return Path; }
		NxFr::StringId GetItemType() const override { return Type; }
		NxFr::GUID GetItemId() const override { return Id; }

		NxFr::StringView GetTargetPath() const { return Path; }
		NxFr::StringView GetPrettyPath() const { return NxFr::Path::GetPathWithoutExtension(Path); }
		NxFr::StringView GetDirectory() const { return NxFr::Path::GetFolder(Path); }
		NxFr::StringView GetTargetName() const { return NxFr::Path::GetNameAndExtension(Path); }
		NxFr::StringView GetPrettyName() const { return NxFr::Path::GetName(Path); }
		NxFr::StringView GetExtension() const { return NxFr::Path::GetExtension(Path); }
		bool IsDirectory() const { return NxFr::Path::IsDirectory(Path); }

	protected:
		AssetsBrowserItem();
		virtual ~AssetsBrowserItem();

		virtual void OnCreate(NxFr::StringView Target, NxFr::StringId Type) = 0;
		virtual void OnMove(NxFr::StringView Item, NxFr::StringView Target) = 0;
		virtual void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) = 0;
		virtual void OnDelete(NxFr::StringView Item) = 0;

		int8 Compare(const TreeItem& Other) const override;
		void CacheImGuiText() override;

		virtual NxFr::StringView GetPrefix() const = 0;

		AssetsBrowserItem* GetParent() const override { return Parent; }
		AssetsBrowserItem* GetPrevious() const override { return Previous; }
		AssetsBrowserItem* GetNext() const override { return Next; }
		AssetsBrowserItem* GetChild() const override { return Child; }

	private:
		NxFr::GUID Id;
		NxFr::String Path;
		NxFr::StringId Type;

		AssetsBrowserItem* Parent;
		AssetsBrowserItem* Previous;
		AssetsBrowserItem* Next;
		AssetsBrowserItem* Child;
	};

	class AssetsBrowserItemDirectory : public AssetsBrowserItem
	{
	public:
		NX_OBJECT_DECLARATION(AssetsBrowserItemDirectory)

	protected:
		void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		void OnDelete(NxFr::StringView Item) override;

		NxFr::StringView GetPrefix() const override { return "D"; }
	};

	class AssetsBrowserItemContent : public AssetsBrowserItem
	{
	public:
		NX_OBJECT_DECLARATION(AssetsBrowserItemDirectory)

	protected:
		void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		void OnDelete(NxFr::StringView Item) override;

		NxFr::StringView GetPrefix() const override { return "C"; };
	};

	class AssetsBrowserItemAsset : public AssetsBrowserItem
	{
	public:
		NX_OBJECT_DECLARATION(AssetsBrowserItemDirectory)

	protected:
		void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		void OnDelete(NxFr::StringView Item) override;

		NxFr::StringView GetPrefix() const override { return "A"; };
	};
}
