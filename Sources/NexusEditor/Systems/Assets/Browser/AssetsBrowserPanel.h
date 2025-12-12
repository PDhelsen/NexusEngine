#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	class AssetsBrowserPanel : public NxEn::GUI::Panel
	{
		friend class AssetsBrowserItem;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserPanel)

		NEXUS_EDITOR_API AssetsBrowserPanel();
		NEXUS_EDITOR_API ~AssetsBrowserPanel();

		NEXUS_EDITOR_API void Refresh();

		NEXUS_EDITOR_API void Find(NxFr::StringView Query);
		NEXUS_EDITOR_API void Select(NxFr::GUID Id, bool Additive = false, bool List = false);
		NEXUS_EDITOR_API void Select(NxFr::StringView Path, bool Additive = false, bool List = false);

		NEXUS_EDITOR_API void Create(NxFr::StringView Path, NxFr::StringId Type);
		NEXUS_EDITOR_API void Move(NxFr::StringView Path, NxFr::StringView Target);
		NEXUS_EDITOR_API void Delete(NxFr::StringView Path);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		void DrawHeader();
		void DrawItem(AssetsBrowserItem* Item);
		void SelectItem(AssetsBrowserItem* Item);

		void Clear();
		void Fetch();
		AssetsBrowserItem* FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent);
		AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item);

		AssetsBrowserItem* AppendItem(NxFr::StringView Path, bool AppendId);
		void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool UpdateId);
		void RemoveItem(AssetsBrowserItem* Item, bool RemoveId);
		void AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent, bool Sort);
		void DetachItem(AssetsBrowserItem* Item, bool Sort);
		void SortItem(AssetsBrowserItem* Item);
		AssetsBrowserItem* GetParent(NxFr::StringView Path);
		AssetsBrowserItem* GetIterator(AssetsBrowserItem* Item);

		void Find();
		void Select(AssetsBrowserItem* Item, bool Additive = false, bool List = false);
		void Show(AssetsBrowserItem* Item);
		bool IsVisible(AssetsBrowserItem* Item);

		NxFr::GUID PathToId(NxFr::StringView Path);
		NxFr::String FileToPath(NxFr::StringView Path);
		NxFr::String PathToFile(NxFr::StringView Path);
		NxFr::String ConvertPath(NxFr::StringView Path);

	private:
		const inline static NxFr::String Root = "Assets/";

		NxEn::GUI::Style Style;

		NxEn::AssetsSystem* Assets;
		NxEn::InputSystem* Inputs;

		NxFr::Dictionary<NxFr::GUID, AssetsBrowserItem*> Map;
		AssetsBrowserItem* Items;

		NxFr::Set<AssetsBrowserItem*> Selection;
		AssetsBrowserItem* Selected;

		NxFr::Set<AssetsBrowserItem*> Filtered;
		NxFr::String Filter;
	};
}
