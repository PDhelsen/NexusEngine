#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	class AssetsBrowserPanel : public NxEn::TreePanel
	{
		friend class AssetsBrowserItem;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserPanel)

		NEXUS_EDITOR_API AssetsBrowserPanel();
		NEXUS_EDITOR_API ~AssetsBrowserPanel();

		NEXUS_EDITOR_API void Select(NxFr::StringView Path, bool Additive = false, bool List = false);

		NEXUS_EDITOR_API void Create(NxFr::StringView Path, NxFr::StringId Type);
		NEXUS_EDITOR_API void Move(NxFr::StringView Path, NxFr::StringView Target);
		NEXUS_EDITOR_API void Duplicate(NxFr::StringView Path, NxFr::StringView Target);
		NEXUS_EDITOR_API void Delete(NxFr::StringView Path);

		NEXUS_EDITOR_API bool Exist(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::StringView ValidatePath(NxFr::StringView Path);
		NEXUS_EDITOR_API NxFr::String EnsureUniquePath(NxFr::String Path);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;

	private:
		AssetsBrowserItem* GetParent(NxFr::StringView Path);
		AssetsBrowserItem* GetItem(NxFr::GUID Id) override;
		AssetsBrowserItem* FetchItems() override;
		AssetsBrowserItem* FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent);
		AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item);
		AssetsBrowserItem* DuplicateItem(NxEn::TreeItem* Item, NxEn::TreeItem* Parent = nullptr) override;
		AssetsBrowserItem* AppendItem(NxFr::StringView Path);
		void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool AddId, bool RemoveId);

		NxFr::GUID PathToId(NxFr::StringView Path);
		static NxFr::String DiskToPath(NxFr::StringView Path);
		static NxFr::String PathToDisk(NxFr::StringView Path);
		static NxFr::String PathToAsset(NxFr::StringView Path);

	private:
		const inline static NxFr::String RootPath = "";
		const inline static NxFr::String RootImGui = "Assets";

		NxEn::AssetsSystem* Assets;

	};
}
