#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	class AssetsBrowserPanel : public NxEn::GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserPanel)

		NEXUS_EDITOR_API AssetsBrowserPanel();
		NEXUS_EDITOR_API ~AssetsBrowserPanel();

		NEXUS_EDITOR_API void Refresh();
		NEXUS_EDITOR_API void Select(NxFr::StringView Path, bool Additive = false, bool List = false);
		NEXUS_EDITOR_API void Select(NxFr::GUID Id, bool Additive = false, bool List = false);
		NEXUS_EDITOR_API void Find(NxFr::StringView Query);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		void DrawHeader();
		void DrawItem(AssetsBrowserItem* Item);
		void SelectItem(AssetsBrowserItem* Item);

		void FetchFolder();
		AssetsBrowserItem* FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent);
		AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item);
		AssetsBrowserItem* AppendItem(NxFr::GUID Id, NxFr::StringView Path);
		void RemoveItem(AssetsBrowserItem* Item);

		void Select(AssetsBrowserItem* Item, bool Additive = false, bool List = false);
		void Show(AssetsBrowserItem* Item);
		bool IsVisible(AssetsBrowserItem* Item);

		void Find();

		NxFr::GUID PathToId(NxFr::StringView Path);
		NxFr::String FileToPath(NxFr::StringView Path);
		NxFr::String PathToFile(NxFr::StringView Path);

	private:
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
