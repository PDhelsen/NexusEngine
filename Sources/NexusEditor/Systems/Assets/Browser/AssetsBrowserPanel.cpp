#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserInternal.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionCreate.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionRename.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDelete.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionLoad.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReload.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionImport.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReimport.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReferences.h"

#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	const static NxEn::Command CmdAssetPing = NxEn::Command::Create("Assets.Ping"_Sid, "Ping path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Select(Path);
	}));


	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	AssetsBrowserPanel::AssetsBrowserPanel()
		: TreePanel(), Assets(nullptr)
	{
	}

	AssetsBrowserPanel::~AssetsBrowserPanel()
	{
	}

	void AssetsBrowserPanel::Select(NxFr::StringView Path)
	{
		Path = AssetsBrowser::ValidatePath(Path);
		NxFr::GUID Id = AssetsBrowser::PathToId(Path);
		TreePanel::Select(GetItem(Id));
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetTitle("Assets Browser");

		AppendAction<AssetsBrowserActionCreate>();
		AppendAction<AssetsBrowserActionDuplicate>();
		AppendAction<AssetsBrowserActionRename>();
		AppendAction<AssetsBrowserActionMove>();
		AppendAction<AssetsBrowserActionDelete>();
		AppendAction<AssetsBrowserActionLoad>();
		AppendAction<AssetsBrowserActionReload>();
		AppendAction<AssetsBrowserActionImport>();
		AppendAction<AssetsBrowserActionReimport>();
		AppendAction<AssetsBrowserActionReferences>();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		TreePanel::OnEnable();
	}

	NxEn::TreeItem* AssetsBrowserPanel::FetchItems()
	{
		return AssetsBrowser::FetchItems();
	}

	NxEn::TreeItem* AssetsBrowserPanel::GetItem(NxFr::GUID Id)
	{
		return AssetsBrowser::GetItem(Id);
	}
}
