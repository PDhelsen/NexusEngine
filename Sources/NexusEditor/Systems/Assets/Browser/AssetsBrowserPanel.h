#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserPanel : public NxEn::TreePanel
	{
		friend class AssetsBrowser;
		friend class AssetsBrowserEditContext;

	public:
		NX_OBJECT(AssetsBrowserPanel)

		void Refresh() override;

		void Select(NxFr::GUID Id);
		void Select(NxFr::StringView Path);

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

		NxEn::TreeItem* FetchRootItem() override;
		void OnDestroyItem(NxEn::TreeItem* Item) override;
		void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

	private:
		class EditSystem* Edit;

		class AssetsBrowser* Browser;
		class AssetsBrowserEditContext* Context;
	};
}

