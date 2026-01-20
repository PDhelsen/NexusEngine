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

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;

		NEXUS_EDITOR_API NxEn::TreeItem* FetchRootItem() override;
	};
}

