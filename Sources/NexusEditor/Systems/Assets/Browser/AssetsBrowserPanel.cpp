#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Systems/Editor/EditorSystem.h"

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

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

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
		AppendAction<AssetsBrowserActionInspect>();
		AppendAction<AssetsBrowserActionView>();
		AppendAction<AssetsBrowserActionReferences>();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Browser = &NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser();
		TreePanel::OnEnable();
		SetTitle("Assets");
	}

	NxEn::TreeItem* AssetsBrowserPanel::FetchRootItem()
	{
		return Root;
	}
}
