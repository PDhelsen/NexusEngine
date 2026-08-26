#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemBrowser = NxEn::GUI::Menu::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	AssetsBrowserItem* AssetsBrowserPanel::GetItem(NxFr::GUID InstanceId)
	{
		return Browser->GetItem(InstanceId);
	}

	void AssetsBrowserPanel::SetBrowser(AssetsBrowser* Browser)
	{
		this->Browser = Browser;
	}

	void AssetsBrowserPanel::Refresh()
	{
		Browser->Refresh();
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetNameId("Assets");
	}

	void AssetsBrowserPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Browser->SetEditContext();
		}

		TreePanel::OnDraw();
	}

	void AssetsBrowserPanel::OnSelectItem(NxFr::GUID InstanceId, bool State)
	{
		Browser->SelectItem(Browser->GetItem(InstanceId), State);
	}
}
