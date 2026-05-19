#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class AssetsBrowserPanel : public NxEn::TreePanel
	{
		friend class AssetsBrowser;
		friend class AssetsBrowserEditContext;

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserPanel)

		NX_EDITOR_API void Refresh() override;

		NX_EDITOR_API void Select(NxFr::GUID Id);
		NX_EDITOR_API void Select(NxFr::StringView Path);

	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnShutdown() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnDisable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

		NX_EDITOR_API NxEn::TreeItem* FetchRootItem() override;
		NX_EDITOR_API void OnDestroyItem(NxEn::TreeItem* Item) override;
		NX_EDITOR_API void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

	private:
		class EditSystem* Edit;

		class AssetsBrowser* Browser;
		class AssetsBrowserEditContext* Context;
	};
}

