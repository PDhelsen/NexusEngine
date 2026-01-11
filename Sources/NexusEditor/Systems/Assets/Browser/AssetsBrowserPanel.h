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

		NEXUS_EDITOR_API void Select(NxFr::StringView Path);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;

	private:
		NEXUS_ENGINE_API NxEn::TreeItem* FetchItems() override;
		NEXUS_ENGINE_API NxEn::TreeItem* GetItem(NxFr::GUID Id);

	private:
		NxEn::AssetsSystem* Assets;
	};
}
