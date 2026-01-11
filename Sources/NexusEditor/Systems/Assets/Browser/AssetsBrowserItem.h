#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserInternal.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class AssetsBrowserItem : public NxEn::TreeItem
	{
		friend AssetsBrowserItem* AssetsBrowser::FetchItems();
		friend void AssetsBrowser::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool AddId, bool RemoveId);

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItem)

		NEXUS_EDITOR_API AssetsBrowserItem();
		NEXUS_EDITOR_API virtual ~AssetsBrowserItem();

		NEXUS_EDITOR_API virtual void Create(NxFr::StringView Target, NxFr::StringId Type) = 0;
		NEXUS_EDITOR_API virtual void Move(NxFr::StringView Target) = 0;
		NEXUS_EDITOR_API virtual void Duplicate(NxFr::StringView Target) = 0;
		NEXUS_EDITOR_API virtual void Delete() = 0;

		NEXUS_EDITOR_API AssetsBrowserItem* GetParent() const override;
		NEXUS_EDITOR_API void SetParent(TreeItem* Instance) override;
		NEXUS_EDITOR_API AssetsBrowserItem* GetPrevious() const override;
		NEXUS_EDITOR_API void SetPrevious(TreeItem* Instance) override;
		NEXUS_EDITOR_API AssetsBrowserItem* GetNext() const override;
		NEXUS_EDITOR_API void SetNext(TreeItem* Instance) override;
		NEXUS_EDITOR_API AssetsBrowserItem* GetChild() const override;
		NEXUS_EDITOR_API void SetChild(TreeItem* Instance) override;

		NEXUS_EDITOR_API NxFr::StringView GetLabel() const override { return ImGuiText; }
		NEXUS_EDITOR_API NxFr::StringView GetDescription() const override { return Path; }
		NEXUS_EDITOR_API NxFr::StringId GetType() const override { return Type; };

		NEXUS_EDITOR_API NxFr::GUID GetId() const override { return Id; };
		NEXUS_EDITOR_API NxFr::StringView GetName() const override { return IsDirectory() ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path, true); };
		NEXUS_EDITOR_API NxFr::StringView GetPath() const { return Path; };

		NEXUS_EDITOR_API NxFr::StringView GetPrettyPath() const { return NxFr::Path::GetPathWithoutExtension(Path); };
		NEXUS_EDITOR_API NxFr::StringView GetDirectory() const { return NxFr::Path::GetParent(Path); };
		NEXUS_EDITOR_API NxFr::StringView GetPrettyName() const { return IsDirectory() ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path); };
		NEXUS_EDITOR_API NxFr::StringView GetExtension() const { return NxFr::Path::GetExtension(Path); };
		NEXUS_EDITOR_API bool IsDirectory() const { return NxFr::Path::IsDirectory(Path); }

	protected:
		NEXUS_EDITOR_API virtual NxFr::StringView GetPrefix() const = 0;

	private:
		NEXUS_EDITOR_API void GenerateImGuiText() override;
		NEXUS_EDITOR_API int8 Compare(const TreeItem& Other) const override;

		NEXUS_EDITOR_API void ForceImGuiText(NxFr::StringView Text);
		NEXUS_EDITOR_API void Update(NxFr::GUID Id, NxFr::StringView Path, NxFr::StringId Type);


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

		NEXUS_EDITOR_API AssetsBrowserItemDirectory();
		NEXUS_EDITOR_API virtual ~AssetsBrowserItemDirectory();

		NEXUS_EDITOR_API void Create(NxFr::StringView Target, NxFr::StringId Type) override;
		NEXUS_EDITOR_API void Move(NxFr::StringView Target) override;
		NEXUS_EDITOR_API void Duplicate(NxFr::StringView Target) override;
		NEXUS_EDITOR_API void Delete() override;

	protected:
		NEXUS_EDITOR_API NxFr::StringView GetPrefix() const override { return "D"; }
	};

	class AssetsBrowserItemFile : public AssetsBrowserItem
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemFile)

		NEXUS_EDITOR_API AssetsBrowserItemFile();
		NEXUS_EDITOR_API virtual ~AssetsBrowserItemFile();

		NEXUS_EDITOR_API void Create(NxFr::StringView Target, NxFr::StringId Type) override;
		NEXUS_EDITOR_API void Move(NxFr::StringView Target) override;
		NEXUS_EDITOR_API void Duplicate(NxFr::StringView Target) override;
		NEXUS_EDITOR_API void Delete() override;

	protected:
		NEXUS_EDITOR_API NxFr::StringView GetPrefix() const override { return "F"; }
	};

	class AssetsBrowserItemAsset : public AssetsBrowserItem
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemAsset)

		NEXUS_EDITOR_API AssetsBrowserItemAsset();
		NEXUS_EDITOR_API virtual ~AssetsBrowserItemAsset();

		NEXUS_EDITOR_API void Create(NxFr::StringView Target, NxFr::StringId Type) override;
		NEXUS_EDITOR_API void Move(NxFr::StringView Target) override;
		NEXUS_EDITOR_API void Duplicate(NxFr::StringView Target) override;
		NEXUS_EDITOR_API void Delete() override;

	protected:
		NEXUS_EDITOR_API NxFr::StringView GetPrefix() const override { return "A"; }
	};
}
