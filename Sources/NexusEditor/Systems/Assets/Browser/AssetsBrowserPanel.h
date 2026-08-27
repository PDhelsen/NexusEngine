#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserPanel : public NxEn::TreePanel
	{
		friend class AssetsBrowser;
		friend class AssetsBrowserEditContext;

	public:
		NX_OBJECT(AssetsBrowserPanel)

		AssetsBrowserItem* GetItem(NxFr::GUID InstanceId) override;

		void Initialize(AssetsBrowser* Browser, AssetsBrowserEditContext* Context);

		void Refresh() override;

	protected:
		void OnInitialize() override;
		void OnDraw() override;

		void OnSelectItem(NxFr::GUID InstanceId, bool State) override;

	private:
		EditSystem* Edit;

		AssetsBrowser* Browser;
		AssetsBrowserEditContext* Context;
	};
}

