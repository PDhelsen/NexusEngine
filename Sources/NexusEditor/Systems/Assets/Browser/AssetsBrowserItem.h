#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetsBrowserItem : public NxEn::Object
	{
		friend class AssetsBrowserPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItem)

		NEXUS_EDITOR_API AssetsBrowserItem();
		NEXUS_EDITOR_API virtual ~AssetsBrowserItem();

		NEXUS_EDITOR_API virtual void Create(NxFr::StringView Target, NxFr::StringId Type) = 0;
		NEXUS_EDITOR_API virtual void Move(NxFr::StringView Target) = 0;
		NEXUS_EDITOR_API virtual void Duplicate(NxFr::StringView Target) = 0;
		NEXUS_EDITOR_API virtual void Delete() = 0;

		NEXUS_EDITOR_API AssetsBrowserItem* GetParent() const { return Parent; }
		NEXUS_EDITOR_API AssetsBrowserItem* GetPrevious() const { return Previous; }
		NEXUS_EDITOR_API AssetsBrowserItem* GetNext() const { return Next; }
		NEXUS_EDITOR_API AssetsBrowserItem* GetChild() const { return Child; }
		NEXUS_EDITOR_API bool IsRoot() const { return Parent == nullptr; }
		NEXUS_EDITOR_API bool IsLeaf() const { return Child == nullptr; }

		NEXUS_EDITOR_API NxFr::GUID GetId() const override { return Id; };
		NEXUS_EDITOR_API NxFr::StringView GetPath() const { return Path; };

		NEXUS_EDITOR_API NxFr::StringView GetPrettyPath() const { return NxFr::Path::GetPathWithoutExtension(Path); };
		NEXUS_EDITOR_API NxFr::StringView GetDirectory() const { return NxFr::Path::GetParent(Path); };
		NEXUS_EDITOR_API NxFr::StringView GetPrettyName() const { return IsDirectory() ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path); };
		NEXUS_EDITOR_API NxFr::StringView GetName() const { return IsDirectory() ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path, true); };
		NEXUS_EDITOR_API NxFr::StringView GetExtension() const { return NxFr::Path::GetExtension(Path); };
		NEXUS_EDITOR_API bool IsDirectory() const { return NxFr::Path::IsDirectory(Path); }

		NEXUS_EDITOR_API bool IsOpen() const { return Expanded; }
		NEXUS_EDITOR_API bool IsSelected() const { return Selected; }

	protected:
		NEXUS_EDITOR_API virtual NxFr::StringView GetPrefix() const = 0;

		NEXUS_EDITOR_API void Update(NxFr::StringView Target, bool AddId, bool RemoveId);
		NEXUS_EDITOR_API NxFr::String PathToDisk(NxFr::StringView Path) const;
		NEXUS_EDITOR_API NxFr::String PathToAsset(NxFr::StringView Path) const;

	protected:
		AssetsBrowserItem* Parent;
		AssetsBrowserItem* Previous;
		AssetsBrowserItem* Next;
		AssetsBrowserItem* Child;

		NxFr::GUID Id;
		NxFr::String Path;
		NxFr::String ImGuiText;

		bool Expanded;
		bool Selected;
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
