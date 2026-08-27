#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>(false);

	static const NxEn::GUI::Menu::Item* MenuItemBrowser = NxEn::GUI::Menu::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	AssetsBrowserItem* AssetsBrowserPanel::GetItem(NxFr::GUID InstanceId)
	{
		return Browser->GetItem(InstanceId);
	}

	void AssetsBrowserPanel::Initialize(AssetsBrowser* Browser, AssetsBrowserEditContext* Context)
	{
		this->Browser = Browser;
		this->Context = Context;

		TreePanel::Initialize();
	}

	void AssetsBrowserPanel::Refresh()
	{
		Browser->Refresh();
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetNameId("Assets");

		Edit = NxEn::Application::GetSystem<EditSystem>();
	}

	void AssetsBrowserPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit->SetContext(Context->GetId());
		}

		TreePanel::OnDraw();
	}

	void AssetsBrowserPanel::OnSelectItem(NxFr::GUID InstanceId, bool State)
	{
		Edit->SetSelected(InstanceId, State, Context->GetId());
	}
}
