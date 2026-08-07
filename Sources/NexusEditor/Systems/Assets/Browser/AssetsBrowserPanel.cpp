#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemBrowser = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	void AssetsBrowserPanel::Refresh()
	{
		Browser->Refresh();
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetNameId("Assets");

		AppendAction<AssetsBrowserActionCreate>();
		AppendAction<AssetsBrowserActionRename>();
		AppendAction<AssetsBrowserActionDuplicate>();
		AppendAction<AssetsBrowserActionMove>();
		AppendAction<AssetsBrowserActionDelete>();
		AppendAction<AssetsBrowserActionImport>();
		AppendAction<AssetsBrowserActionLoad>();
		AppendAction<AssetsBrowserActionInstantiate>();
		AppendAction<AssetsBrowserActionView>();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		TreePanel::OnEnable();

		Root = FetchRootItem();
	}

	void AssetsBrowserPanel::OnDisable()
	{
		Clear();

		TreePanel::OnDisable();
	}

	void AssetsBrowserPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit::Context::SetCurrent(&Browser->Context);
		}

		TreePanel::OnDraw();
	}

	NxEn::TreeItem* AssetsBrowserPanel::FetchRootItem()
	{
		return Browser->Root;
	}

	void AssetsBrowserPanel::OnCreateItem(NxEn::TreeItem* Item)
	{
		TreePanel::Select(Item);
	}

	void AssetsBrowserPanel::OnDestroyItem(NxEn::TreeItem* Item)
	{
		TreePanel::OnDestroyItem(Item);
	}

	void AssetsBrowserPanel::OnSelectItem(NxEn::TreeItem* Item, bool State)
	{
		AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
		Browser->SelectItem(Instance, State, GetId());
	}
}
