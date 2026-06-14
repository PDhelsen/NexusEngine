#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class AssetsBrowser;
	class AssetsBrowserItem;

	class NX_EDITOR_API AssetsBrowserPanel : public NxEn::TreePanel
	{
	public:
		NX_OBJECT(AssetsBrowserPanel)

		void Refresh() override;

		void Select(NxFr::GUID Id);
		void Select(NxFr::StringView Path);

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		NxEn::TreeItem* FetchRootItem() override;
		void OnCreateItem(AssetsBrowserItem* Item);
		void OnDestroyItem(AssetsBrowserItem* Item);
		void OnSelectItem(AssetsBrowserItem* Item, bool State);

	private:
		AssetsBrowser* Browser;
	};
}

