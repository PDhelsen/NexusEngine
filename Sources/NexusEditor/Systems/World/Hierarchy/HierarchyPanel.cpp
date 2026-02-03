#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyPanel)

	void HierarchyPanel::Refresh()
	{
		NxEn::TreeItem* Target = Root;
		TreePanel::Refresh();
		Root = Target;
	}

	void HierarchyPanel::Show(NxFr::Handle<NxEn::GameObject> Target)
	{
		Clear();
		Root = Manager->Convert(Target);
		TreePanel::Show();
	}

	void HierarchyPanel::SelectGameObject(NxFr::Handle<NxEn::GameObject> Target)
	{
		TreePanel::Select(Manager->Convert(Target));
	}

	HierarchyPanel::HierarchyPanel(HierarchyManager* Manager)
		: Manager(Manager)
	{
	}

	HierarchyPanel::~HierarchyPanel()
	{
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
		AppendAction<HierarchyActionPrefabCreate>();
		AppendAction<HierarchyActionPrefabSave>();
		AppendAction<HierarchyActionPrefabUnpack>();
		AppendAction<HierarchyActionInspect>();
	}

	NxEn::TreeItem* HierarchyPanel::FetchRootItem()
	{
		return nullptr;
	}

	void HierarchyPanel::FindItem()
	{
		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}

		NxEn::World* World = static_cast<HierarchyItem*>(Root)->GetTarget()->GetWorld();
		NxFr::Array<NxFr::Handle<NxEn::GameObject>> GameObjects = World->Find(Filter);
		for (auto& Instance : GameObjects)
		{
			NxEn::TreeItem* Item = Manager->Convert(Instance);
			ShowItem(Item);
			Filtered.Append(Item);
		}
	}
}
