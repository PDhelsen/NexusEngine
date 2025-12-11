#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetsBrowserItem : public NxEn::Object
	{
		friend class AssetsBrowserPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItem)

		NEXUS_EDITOR_API NxFr::GUID GetId() const override { return Id; };
		NEXUS_EDITOR_API NxFr::StringView GetPath() const { return Path; };

		NEXUS_EDITOR_API NxFr::StringView GetPrettyPath() const { return PrettyPath; };
		NEXUS_EDITOR_API NxFr::StringView GetDirectory() const { return Directory; };
		NEXUS_EDITOR_API NxFr::StringView GetName() const { return Name; };
		NEXUS_EDITOR_API NxFr::StringView GetExtension() const { return Extension; };

		NEXUS_EDITOR_API bool IsOpen() const { return Expanded; }
		NEXUS_EDITOR_API bool IsSelected() const { return Selected; }

	protected:
		NEXUS_EDITOR_API AssetsBrowserItem(NxFr::GUID ItemId, NxFr::StringView FilePath);
		NEXUS_EDITOR_API virtual ~AssetsBrowserItem();

		NEXUS_EDITOR_API void Update(NxFr::GUID ItemId, NxFr::StringView FilePath);

		NEXUS_EDITOR_API void GenerateInfo();
		NEXUS_EDITOR_API virtual void GenerateImGui() = 0;

		NEXUS_EDITOR_API bool IsRoot() const { return Parent == nullptr; }
		NEXUS_EDITOR_API bool IsLeaf() const { return Child == nullptr; }

		NEXUS_EDITOR_API AssetsBrowserItem* GetIterator();

	protected:
		AssetsBrowserItem* Parent;
		AssetsBrowserItem* Previous;
		AssetsBrowserItem* Next;
		AssetsBrowserItem* Child;

		NxFr::GUID Id;
		NxFr::String Path;
		NxFr::String ImGuiText;

		NxFr::StringView PrettyPath;
		NxFr::StringView Directory;
		NxFr::StringView Name;
		NxFr::StringView Extension;

		bool Expanded;
		bool Selected;
	};

	class AssetsBrowserItemDirectory : public AssetsBrowserItem
	{
		friend class AssetsBrowserPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemDirectory)

	protected:
		NEXUS_EDITOR_API AssetsBrowserItemDirectory(NxFr::GUID ItemId, NxFr::StringView FilePath);
		NEXUS_EDITOR_API virtual ~AssetsBrowserItemDirectory();

		NEXUS_EDITOR_API void GenerateImGui() override;
	};

	class AssetsBrowserItemFile : public AssetsBrowserItem
	{
		friend class AssetsBrowserPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemFile)

	protected:
		NEXUS_EDITOR_API AssetsBrowserItemFile(NxFr::GUID ItemId, NxFr::StringView FilePath);
		NEXUS_EDITOR_API virtual ~AssetsBrowserItemFile();

		NEXUS_EDITOR_API void GenerateImGui() override;
	};

	class AssetsBrowserItemAsset : public AssetsBrowserItem
	{
		friend class AssetsBrowserPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItemAsset)

	protected:
		NEXUS_EDITOR_API AssetsBrowserItemAsset(NxFr::GUID ItemId, NxFr::StringView FilePath);
		NEXUS_EDITOR_API virtual ~AssetsBrowserItemAsset();

		NEXUS_EDITOR_API void GenerateImGui() override;
	};
}
