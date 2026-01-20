#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Systems/Editor/EditorSystem.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemReferences = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	void AssetsBrowserPanel::Refresh()
	{
		NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser().Refresh();
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
		AppendAction<AssetsBrowserActionReferences>();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		TreePanel::OnEnable();
		SetTitle("Assets");
	}

	NxEn::TreeItem* AssetsBrowserPanel::FetchRootItem()
	{
		return Root;
	}
}
