#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"

#include "NexusEditor/Core/NexusEditorApplication.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemBrowser = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	const static NxEn::Command CmdAssetBrowserSelect = NxEn::Command::Create("Assets.Browser.Select"_Sid, "Select path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Select(Item);
	}));

	void AssetsBrowserPanel::Refresh()
	{
		Browser->Refresh();
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
		AppendAction<AssetsBrowserActionReferences>();
	}

	void AssetsBrowserPanel::OnShutdown()
	{
		TreePanel::OnShutdown();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

		Context = new AssetsBrowserEditContext(GetImGuiId(), this);
		Context->GetOnSelectionChanged() += [this](NxFr::GUID Id, bool State)
		{
			NxEn::TreeItem* Item = Browser->GetItem(Id);
			SelectItem(Item, State, true, false);
		};
		Edit = NxEn::Application::GetSystem<EditSystem>();
		Edit->RegisterContext(GetImGuiId(), Context);

		TreePanel::OnEnable();
	}

	void AssetsBrowserPanel::OnDisable()
	{
		TreePanel::OnDisable();

		delete Edit->UnregisterContext(GetImGuiId());
		Context = nullptr;
		Edit = nullptr;
	}

	void AssetsBrowserPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit::Context::SetCurrent(Context);
		}

		TreePanel::OnDraw();
	}

	NxEn::TreeItem* AssetsBrowserPanel::FetchRootItem()
	{
		return Root;
	}

	void AssetsBrowserPanel::OnDestroyItem(NxEn::TreeItem* Item)
	{
		if (Edit && Context)
		{
			Edit->Unselect(Item->GetItemId(), Context->GetId());
		}

		TreePanel::OnDestroyItem(Item);
	}

	void AssetsBrowserPanel::OnSelectItem(NxEn::TreeItem* Item, bool State)
	{
		TreePanel::OnSelectItem(Item, State);

		if (State)
		{
			Edit->Select(Item->GetItemId(), Context->GetId());
		}
		else
		{
			Edit->Unselect(Item->GetItemId(), Context->GetId());
		}
	}
}
