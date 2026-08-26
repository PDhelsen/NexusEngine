#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowser
	{
		friend class AssetsBrowserPanel;
		friend class AssetsBrowserEditContext;

	public:
		NX_NOCOPY_NOMOVE(AssetsBrowser)
		AssetsBrowser();
		~AssetsBrowser();

		AssetsBrowserItem* GetItem(NxFr::GUID Id);
		void Refresh();

		void Create(NxFr::StringId Type, NxFr::StringView TargetPath);
		void Move(NxFr::StringView ItemPath, NxFr::StringView TargetPath);
		void Duplicate(NxFr::StringView ItemPath, NxFr::StringView TargetPath);
		void Delete(NxFr::StringView ItemPath);

		bool Exist(NxFr::StringView ItemPath);
		NxFr::StringView Validate(NxFr::StringView ItemPath);
		NxFr::String MakeUniquePath(NxFr::StringView ItemPath);

	private:
		void FetchItems();
		AssetsBrowserItem* FetchItems(NxFr::StringView FsPath, AssetsBrowserItem* Parent);
		AssetsBrowserItem* PurgeItems(AssetsBrowserItem* Item);
		void ClearItems();

		AssetsBrowserItem* AppendItem(NxFr::StringView ItemPath);
		void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView ItemPath, bool UpdateId = false);
		void RemoveItem(AssetsBrowserItem* Item);
		void AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent);
		void DetachItem(AssetsBrowserItem* Item);
		void SelectItem(AssetsBrowserItem* Item, bool State);

		void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath, AssetsBrowserItem* Parent);
		void OnMove(AssetsBrowserItem* Item, NxFr::StringView TargetPath, AssetsBrowserItem* Parent);
		void OnDuplicate(AssetsBrowserItem* Item, NxFr::StringView TargetPath, AssetsBrowserItem* Parent);
		void OnDelete(AssetsBrowserItem* Item);

		void SetEditContext();

		NxFr::GUID PathToParent(NxFr::StringView Path);
		NxFr::GUID ItemPathToId(NxFr::StringView ItemPath);
		NxFr::String ItemPathToFsPath(NxFr::StringView ItemPath);
		NxFr::String FsPathToItemPath(NxFr::StringView FsPath);
		NxFr::String ItemPathToAssetPath(NxFr::StringView ItemPath);
		NxFr::String ItemPathToCallbackPath(NxFr::StringView ItemPath, AssetsBrowserItem* Item);

	private:
		NxEn::AssetsSystem* Assets;
		EditSystem* Edit;

		NxFr::Dictionary<NxFr::GUID, AssetsBrowserItem*> Items;
		AssetsBrowserPanel* Panel;
		AssetsBrowserEditContext* Context;
	};
}
