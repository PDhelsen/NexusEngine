#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class AssetsBrowserItem : public NxEn::TreeItem
	{
		friend class AssetsBrowser;
		friend class AssetsBrowserEditContext;

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserItem)

		NX_EDITOR_API NxFr::StringView GetName() const override { return ImGuiText; }
		NX_EDITOR_API NxFr::StringView GetDescription() const { return GetItemName(); }

		NX_EDITOR_API NxFr::StringView GetItemName() const override { return Path; }
		NX_EDITOR_API NxFr::StringId GetItemType() const override { return Type; }
		NX_EDITOR_API NxFr::GUID GetItemId() const override { return Id; }

		NX_EDITOR_API NxFr::StringView GetTargetPath() const { return Path; }
		NX_EDITOR_API NxFr::StringView GetPrettyPath() const { return NxFr::Path::GetPathWithoutExtension(Path); }
		NX_EDITOR_API NxFr::StringView GetDirectory() const { return NxFr::Path::GetFolder(Path); }
		NX_EDITOR_API NxFr::StringView GetTargetName() const { return NxFr::Path::GetNameAndExtension(Path); }
		NX_EDITOR_API NxFr::StringView GetPrettyName() const { return NxFr::Path::GetName(Path); }
		NX_EDITOR_API NxFr::StringView GetExtension() const { return NxFr::Path::GetExtension(Path); }
		NX_EDITOR_API bool IsDirectory() const { return NxFr::Path::IsDirectory(Path); }

	protected:
		NX_EDITOR_API AssetsBrowserItem();
		NX_EDITOR_API virtual ~AssetsBrowserItem();

		NX_EDITOR_API virtual void OnCreate(NxFr::StringView Target, NxFr::StringId Type) = 0;
		NX_EDITOR_API virtual void OnMove(NxFr::StringView Item, NxFr::StringView Target) = 0;
		NX_EDITOR_API virtual void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) = 0;
		NX_EDITOR_API virtual void OnDelete(NxFr::StringView Item) = 0;

		NX_EDITOR_API int8 Compare(const TreeItem& Other) const override;
		NX_EDITOR_API void CacheImGuiText() override;

		NX_EDITOR_API virtual NxFr::StringView GetPrefix() const = 0;

		NX_EDITOR_API AssetsBrowserItem* GetParent() const override { return Parent; }
		NX_EDITOR_API AssetsBrowserItem* GetPrevious() const override { return Previous; }
		NX_EDITOR_API AssetsBrowserItem* GetNext() const override { return Next; }
		NX_EDITOR_API AssetsBrowserItem* GetChild() const override { return Child; }

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
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NX_EDITOR_API void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		NX_EDITOR_API void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		NX_EDITOR_API void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		NX_EDITOR_API void OnDelete(NxFr::StringView Item) override;

		NX_EDITOR_API NxFr::StringView GetPrefix() const override { return "D"; }
	};

	class AssetsBrowserItemContent : public AssetsBrowserItem
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NX_EDITOR_API void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		NX_EDITOR_API void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		NX_EDITOR_API void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		NX_EDITOR_API void OnDelete(NxFr::StringView Item) override;

		NX_EDITOR_API NxFr::StringView GetPrefix() const override { return "C"; };
	};

	class AssetsBrowserItemAsset : public AssetsBrowserItem
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NX_EDITOR_API void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		NX_EDITOR_API void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		NX_EDITOR_API void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		NX_EDITOR_API void OnDelete(NxFr::StringView Item) override;

		NX_EDITOR_API NxFr::StringView GetPrefix() const override { return "A"; };
	};
}
