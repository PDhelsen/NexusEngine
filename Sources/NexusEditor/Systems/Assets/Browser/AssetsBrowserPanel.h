#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserPanel : public NxEn::Rework::TreePanel
	{
		friend class AssetsBrowser;

	public:
		NX_OBJECT(AssetsBrowserPanel)

		AssetsBrowserItem* GetItem(NxFr::GUID Id) override;

		void SetBrowser(AssetsBrowser* Browser);

		void Refresh() override;

	protected:
		void OnInitialize() override;
		void OnDraw() override;

		void OnSelectItem(NxFr::GUID Id, bool State) override;

	private:
		AssetsBrowser* Browser;
	};
}

