#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/Tree/TreePanel.h"

namespace NxEd
{
	class AssetsBrowser;
	class AssetsBrowserItem;

	class NX_EDITOR_API AssetsBrowserPanel : public NxEn::TreePanel
	{
		friend class AssetsBrowser;

	public:
		NX_OBJECT(AssetsBrowserPanel)

		void Refresh() override;

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		NxEn::TreeItem* FetchRootItem() override;
		void OnCreateItem(NxEn::TreeItem* Item) override;
		void OnDestroyItem(NxEn::TreeItem* Item) override;
		void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

	private:
		AssetsBrowser* Browser;
	};
}

