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
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItem)

		NEXUS_EDITOR_API NxFr::StringView GetName() const override { return ImGuiText; }
		NEXUS_EDITOR_API NxFr::StringView GetDescription() const { return GetItemName(); }

		NEXUS_EDITOR_API NxFr::StringView GetItemName() const override { return Path; }
		NEXUS_EDITOR_API NxFr::StringId GetItemType() const override { return Type; }
		NEXUS_EDITOR_API NxFr::GUID GetItemId() const override { return Id; }

		NEXUS_EDITOR_API NxFr::StringView GetTargetPath() const { return Path; }
		NEXUS_EDITOR_API NxFr::StringView GetPrettyPath() const { return NxFr::Path::GetPathWithoutExtension(Path); }
		NEXUS_EDITOR_API NxFr::StringView GetDirectory() const { return NxFr::Path::GetFolder(Path); }
		NEXUS_EDITOR_API NxFr::StringView GetTargetName() const { return NxFr::Path::GetNameAndExtension(Path); }
		NEXUS_EDITOR_API NxFr::StringView GetPrettyName() const { return NxFr::Path::GetName(Path); }
		NEXUS_EDITOR_API NxFr::StringView GetExtension() const { return NxFr::Path::GetExtension(Path); }
		NEXUS_EDITOR_API bool IsDirectory() const { return NxFr::Path::IsDirectory(Path); }

	protected:
		NEXUS_EDITOR_API AssetsBrowserItem();
		NEXUS_EDITOR_API virtual ~AssetsBrowserItem();

		NEXUS_EDITOR_API virtual void OnCreate(NxFr::StringView Target, NxFr::StringId Type) = 0;
		NEXUS_EDITOR_API virtual void OnMove(NxFr::StringView Item, NxFr::StringView Target) = 0;
		NEXUS_EDITOR_API virtual void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) = 0;
		NEXUS_EDITOR_API virtual void OnDelete(NxFr::StringView Item) = 0;

		NEXUS_EDITOR_API int8 Compare(const TreeItem& Other) const override;
		NEXUS_EDITOR_API void CacheImGuiText() override;

		NEXUS_EDITOR_API virtual NxFr::StringView GetPrefix() const = 0;

		NEXUS_EDITOR_API AssetsBrowserItem* GetParent() const override { return Parent; }
		NEXUS_EDITOR_API AssetsBrowserItem* GetPrevious() const override { return Previous; }
		NEXUS_EDITOR_API AssetsBrowserItem* GetNext() const override { return Next; }
		NEXUS_EDITOR_API AssetsBrowserItem* GetChild() const override { return Child; }

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
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NEXUS_EDITOR_API void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		NEXUS_EDITOR_API void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		NEXUS_EDITOR_API void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		NEXUS_EDITOR_API void OnDelete(NxFr::StringView Item) override;

		NEXUS_EDITOR_API NxFr::StringView GetPrefix() const override { return "D"; }
	};

	class AssetsBrowserItemContent : public AssetsBrowserItem
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NEXUS_EDITOR_API void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		NEXUS_EDITOR_API void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		NEXUS_EDITOR_API void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		NEXUS_EDITOR_API void OnDelete(NxFr::StringView Item) override;

		NEXUS_EDITOR_API NxFr::StringView GetPrefix() const override { return "C"; };
	};

	class AssetsBrowserItemAsset : public AssetsBrowserItem
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NEXUS_EDITOR_API void OnCreate(NxFr::StringView Target, NxFr::StringId Type) override;
		NEXUS_EDITOR_API void OnMove(NxFr::StringView Item, NxFr::StringView Target) override;
		NEXUS_EDITOR_API void OnDuplicate(NxFr::StringView Item, NxFr::StringView Target) override;
		NEXUS_EDITOR_API void OnDelete(NxFr::StringView Item) override;

		NEXUS_EDITOR_API NxFr::StringView GetPrefix() const override { return "A"; };
	};
}
