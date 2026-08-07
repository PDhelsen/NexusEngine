#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	void HierarchyPanel::Refresh()
	{
		
	}

	void HierarchyPanel::Select(NxFr::GUID Id)
	{
		TreePanel::Select(Manager->GetItem(Id));
	}

	void HierarchyPanel::Select(NxFr::Handle<NxEn::GameObject> Target)
	{
		TreePanel::Select(Manager->GetItem(Target));
	}

	void HierarchyPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetNameId("Hierarchy");

		AppendAction<HierarchyActionCreate>();
		AppendAction<HierarchyActionRename>();
		AppendAction<HierarchyActionDuplicate>();
		AppendAction<HierarchyActionMove>();
		AppendAction<HierarchyActionDelete>();
		AppendAction<HierarchyActionInspect>();
		AppendAction<HierarchyActionPrefabCreate>();
		AppendAction<HierarchyActionPrefabPack>();
		AppendAction<HierarchyActionPrefabUnpack>();
	}

	void HierarchyPanel::OnEnable()
	{
		TreePanel::OnEnable();
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
			NxFr::Handle<NxEn::GameObject> Target = static_cast<HierarchyItem*>(Root)->GetTarget();
			Edit::Context::SetCurrent(Manager->Contexts[Target->GetWorldId()]);
		}

		TreePanel::OnDraw();
	}

	NxEn::TreeItem* HierarchyPanel::FetchRootItem()
	{
		return Root;
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
		NxFr::Array<NxFr::GUID> Ids = Manager->Worlds->FindGameObjects(Filter, World->GetId());
		for (auto& Id : Ids)
		{
			NxEn::TreeItem* Item = Manager->GetItem(Id);
			ShowItem(Item);
			Filtered.Append(Item);
		}
	}

	HierarchyPanel::HierarchyPanel(HierarchyManager* Manager, HierarchyItem* Root)
		: Manager(Manager)
	{
		this->Root = Root;
	}

	HierarchyPanel::~HierarchyPanel()
	{
	}
}
