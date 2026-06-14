#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemBrowser = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	static NxEn::Command* CmdAssetBrowserSelect = NxEn::Command::Create("Assets.Browser.Select"_Sid, "Select path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Select(Item);
	}));

	void AssetsBrowserPanel::Refresh()
	{
		Browser->Refresh();
		TreePanel::Refresh();
	}

	void AssetsBrowserPanel::Select(NxFr::GUID Id)
	{
		TreePanel::Select(Browser->GetItem(Id));
	}

	void AssetsBrowserPanel::Select(NxFr::StringView Path)
	{
		Select(Browser->ItemPathToId(Path));
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetTitle("Assets");

		AppendAction<AssetsBrowserActionCreate>();
		AppendAction<AssetsBrowserActionRename>();
		AppendAction<AssetsBrowserActionDuplicate>();
		AppendAction<AssetsBrowserActionMove>();
		AppendAction<AssetsBrowserActionDelete>();
		AppendAction<AssetsBrowserActionImport>();
		AppendAction<AssetsBrowserActionReimport>();
		AppendAction<AssetsBrowserActionLoad>();
		AppendAction<AssetsBrowserActionReload>();
		AppendAction<AssetsBrowserActionInstantiate>();
		AppendAction<AssetsBrowserActionView>();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();
		Browser->OnItemCreated += { this, &AssetsBrowserPanel::OnCreateItem };
		Browser->OnItemDestroyed += { this, &AssetsBrowserPanel::OnDestroyItem };
		Browser->OnItemSelected += { this, &AssetsBrowserPanel::OnSelectItem };

		TreePanel::OnEnable();
	}

	void AssetsBrowserPanel::OnDisable()
	{
		Browser->OnItemCreated -= { this, &AssetsBrowserPanel::OnCreateItem };
		Browser->OnItemDestroyed -= { this, &AssetsBrowserPanel::OnDestroyItem };
		Browser->OnItemSelected -= { this, &AssetsBrowserPanel::OnSelectItem };

		TreePanel::OnDisable();
	}

	void AssetsBrowserPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit::Context::SetCurrent(Browser->Context);
		}

		TreePanel::OnDraw();
	}

	NxEn::TreeItem* AssetsBrowserPanel::FetchRootItem()
	{
		return Browser->Root;
	}

	void AssetsBrowserPanel::OnCreateItem(AssetsBrowserItem* Item)
	{
		TreePanel::OnCreateItem(Item);
	}

	void AssetsBrowserPanel::OnDestroyItem(AssetsBrowserItem* Item)
	{
		TreePanel::OnDestroyItem(Item);
	}

	void AssetsBrowserPanel::OnSelectItem(AssetsBrowserItem* Item, bool State)
	{
		TreePanel::OnSelectItem(Item, State);
	}
}
