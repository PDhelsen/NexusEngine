#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowser
	{
		friend class AssetsBrowserPanel;
		friend class AssetsBrowserEditContext;

	public:
		inline static const NxFr::String RootFolderName = "Assets";
		inline static const NxFr::StringId ContextId = "AssetsBrowser"_Sid;

		AssetsBrowser();
		~AssetsBrowser();

		void Clear();
		void Refresh();

		void Create(NxFr::StringView ItemPath, NxFr::StringId Type);
		void Move(NxFr::StringView ItemPath, NxFr::StringView TargetPath);
		void Duplicate(NxFr::StringView ItemPath, NxFr::StringView TargetPath);
		void Delete(NxFr::StringView ItemPath);

		bool Exist(NxFr::StringView ItemPath);
		NxFr::StringView Validate(NxFr::StringView ItemPath);
		NxFr::String MakeUniquePath(NxFr::StringView ItemPath);

	private:
		AssetsBrowserItem* FetchItems(NxFr::StringView FsPath, AssetsBrowserItem* Parent);
		AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item);

		AssetsBrowserItem* AppendItem(NxFr::StringView ItemPath, AssetsBrowserItem* Parent);
		void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView ItemPath, bool Add, bool Remove);
		void RemoveItem(AssetsBrowserItem* Item);
		void AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent);
		void DetachItem(AssetsBrowserItem* Item);

		void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath);
		void OnMove(AssetsBrowserItem* Item, NxFr::StringView TargetPath);
		void OnDuplicate(AssetsBrowserItem* Item, NxFr::StringView TargetPath);
		void OnDelete(AssetsBrowserItem* Item);

		AssetsBrowserItem* GetItem(NxFr::GUID Id);
		AssetsBrowserItem* GetParent(NxFr::StringView Path);

		NxFr::GUID ItemPathToId(NxFr::StringView ItemPath);
		NxFr::String ItemPathToFsPath(NxFr::StringView ItemPath);
		NxFr::String FsPathToItemPath(NxFr::StringView FsPath);
		NxFr::String ItemPathToAssetPath(NxFr::StringView ItemPath);
		NxFr::String ItemPathToCallbackPath(NxFr::StringView ItemPath, AssetsBrowserItem* Item);

	private:
		NxFr::Event<AssetsBrowserItem*> OnItemCreated;
		NxFr::Event<AssetsBrowserItem*> OnItemDestroyed;
		NxFr::Event<AssetsBrowserItem*, bool> OnItemSelected;

		NxEn::AssetsSystem* Assets;
		EditSystem* Edit;

		NxFr::Dictionary<NxFr::GUID, AssetsBrowserItem*> Items;
		AssetsBrowserItem* Root;
		AssetsBrowserEditContext Context;
	};
}
