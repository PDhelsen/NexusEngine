#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"

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

	void HierarchyPanel::OnShutdown()
	{
		TreePanel::OnShutdown();
	}

	void HierarchyPanel::OnEnable()
	{
		Context = new HierarchyEditContext(GetImGuiId(), this);
		Context->GetOnSelectionChanged() += [this](NxFr::GUID Id, bool State)
			{
				NxEn::TreeItem* Item = Manager->Items[Id];
				SelectItem(Item, State, true, false);
			};
		Edit = NxEn::Application::GetSystem<EditSystem>();
		Edit->RegisterContext(GetImGuiId(), Context);

		TreePanel::OnEnable();
	}

	void HierarchyPanel::OnDisable()
	{
		TreePanel::OnDisable();

		delete Edit->UnregisterContext(GetImGuiId());
	}

	void HierarchyPanel::OnGui(float TimeStep)
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit::Context::SetCurrent(Context);
		}

		TreePanel::OnGui(TimeStep);
	}

	NxEn::TreeItem* HierarchyPanel::FetchRootItem()
	{
		return nullptr;
	}

	void HierarchyPanel::OnSelectItem(NxEn::TreeItem* Item, bool State)
	{
		if (State)
		{
			Edit->Select(Item->GetItemId(), Context->GetId());
		}
		else
		{
			Edit->Unselect(Item->GetItemId(), Context->GetId());
		}
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
