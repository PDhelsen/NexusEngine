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
		NEXUS_EDITOR_API void Select(NxFr::GUID Id, bool Additive = false, bool List = false);
		NEXUS_EDITOR_API void Select(NxFr::StringView Path, bool Additive = false, bool List = false);
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
		AssetsBrowserItem* AppendItem(NxFr::StringView Path, bool Replace = false);

		void Select(AssetsBrowserItem* Item, bool Additive = false, bool List = false);
		void Show(AssetsBrowserItem* Item);
		bool IsVisible(AssetsBrowserItem* Item);

		void Find();

	private:
		NxEn::GUI::Style Style;

		NxEn::AssetsSystem* Assets;
		NxEn::InputSystem* Inputs;

		NxFr::List<AssetsBrowserItem> Items;
		NxFr::Dictionary<NxFr::GUID, uint64> Map;

		NxFr::Set<AssetsBrowserItem*> Selection;
		AssetsBrowserItem* Selected;

		NxFr::Set<AssetsBrowserItem*> Filtered;
		NxFr::String Filter;
	};
}
