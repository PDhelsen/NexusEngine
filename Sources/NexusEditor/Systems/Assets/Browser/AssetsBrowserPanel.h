#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class AssetsBrowserPanel : public NxEn::TreePanel
	{
		friend class AssetsBrowser;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserPanel)

		NEXUS_EDITOR_API void Refresh() override;

		NEXUS_EDITOR_API void Select(NxFr::GUID Id);
		NEXUS_EDITOR_API void Select(NxFr::StringView Path);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

		NEXUS_EDITOR_API NxEn::TreeItem* FetchRootItem() override;
		NEXUS_EDITOR_API void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

	private:
		class EditSystem* Edit;

		class AssetsBrowser* Browser;
		class AssetsBrowserEditContext* Context;
	};
}

