#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	static HierarchyPanel* Panel = NxEn::GUI::Panel::Create<HierarchyPanel>();

	const static NxEn::GUI::Menu::Item MenuItemHierarchy = NxEn::GUI::Menu::Item::Create("Object/World/Hierarchy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel HierarchyPanel");
	}));

	void HierarchyPanel::Refresh()
	{
		Manager->Refresh();
	}

	void HierarchyPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetTitle("Hierarchy");

		AppendAction<HierarchyActionCreate>();
		AppendAction<HierarchyActionRename>();
		AppendAction<HierarchyActionDuplicate>();
		AppendAction<HierarchyActionMove>();
		AppendAction<HierarchyActionDelete>();
		//AppendAction<HierarchyActionPrefabCreate>();
		//AppendAction<HierarchyActionPrefabSave>();
		//AppendAction<HierarchyActionPrefabUnpack>();
		AppendAction<HierarchyActionInspect>();
	}

	void HierarchyPanel::OnEnable()
	{
		TreePanel::OnEnable();

		Root = FetchRootItem();
	}

	void HierarchyPanel::OnDisable()
	{
		Clear();

		TreePanel::OnDisable();
	}

	void HierarchyPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit::Context::SetCurrent(&Manager->Context);
		}

		TreePanel::OnDraw();
	}

	NxEn::TreeItem* HierarchyPanel::FetchRootItem()
	{
		return Manager->Root;
	}

	void HierarchyPanel::OnSelectItem(NxEn::TreeItem* Item, bool State)
	{
		Manager->SelectItem(static_cast<HierarchyItem*>(Item)->GetTarget(), State, GetId());
	}

	void HierarchyPanel::FindItem()
	{
		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}

		NxEn::World* World = static_cast<HierarchyItem*>(Root)->GetTarget()->GetWorld();
		NxFr::Array<NxFr::Handle<NxEn::GameObject>> GameObjects = Manager->Worlds->FindGameObjects(Filter, World->GetId());
		for (auto& Instance : GameObjects)
		{
			NxEn::TreeItem* Item = Manager->GetItem(Instance);
			ShowItem(Item);
			Filtered.Append(Item);
		}
	}
}
