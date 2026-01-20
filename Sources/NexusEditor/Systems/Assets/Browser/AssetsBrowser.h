#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetsBrowserItem;
	class AssetsBrowserPanel;

	class AssetsBrowser
	{
	public:
		NEXUS_EDITOR_API AssetsBrowser();
		NEXUS_EDITOR_API ~AssetsBrowser();

		NEXUS_EDITOR_API void Clear();
		NEXUS_EDITOR_API void Refresh();

		NEXUS_EDITOR_API void Create(NxFr::StringView ItemPath, NxFr::StringId Type);
		NEXUS_EDITOR_API void Move(NxFr::StringView ItemPath, NxFr::StringView Target);
		NEXUS_EDITOR_API void Duplicate(NxFr::StringView ItemPath, NxFr::StringView Target);
		NEXUS_EDITOR_API void Delete(NxFr::StringView ItemPath);

		NEXUS_EDITOR_API bool Exist(NxFr::StringView ItemPath);
		NEXUS_EDITOR_API NxFr::StringView Validate(NxFr::StringView ItemPath);
		NEXUS_EDITOR_API NxFr::String MakeUniquePath(NxFr::StringView ItemPath);

	private:
		AssetsBrowserItem* FetchItems(NxFr::StringView FsPath, AssetsBrowserItem* Parent);
		AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item);
		void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView ItemPath, bool Add, bool Remove);

		AssetsBrowserItem* AppendItem(NxFr::StringView ItemPath, AssetsBrowserItem* Parent);
		void RemoveItem(AssetsBrowserItem* Item);
		void AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent);
		void DetachItem(AssetsBrowserItem* Item);

		AssetsBrowserItem* GetItem(NxFr::GUID Id);
		AssetsBrowserItem* GetParent(NxFr::StringView Path);

		void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath);
		void OnMove(AssetsBrowserItem* Item, NxFr::StringView ItemPath, NxFr::StringView TargetPath);
		void OnDuplicate(AssetsBrowserItem* Item, AssetsBrowserItem* Parent, NxFr::StringView ItemPath, NxFr::StringView TargetPath);
		void OnDelete(AssetsBrowserItem* Item, NxFr::StringView ItemPath);

		NxFr::GUID ItemPathToId(NxFr::StringView ItemPath);
		NxFr::String IdToItemPath(NxFr::GUID Id);
		NxFr::String ItemPathToAssetPath(NxFr::StringView ItemPath);
		NxFr::String ItemPathToCallbackPath(NxFr::StringView ItemPath, AssetsBrowserItem* Item);
		NxFr::String ItemPathToFsPath(NxFr::StringView ItemPath);
		NxFr::String FsPathToItemPath(NxFr::StringView FsPath);

	private:
		NxEn::AssetsSystem* Assets;

		NxFr::Dictionary<NxFr::GUID, AssetsBrowserItem*> Items;
		AssetsBrowserItem* Root;
		AssetsBrowserPanel* Panel;
	};
}
