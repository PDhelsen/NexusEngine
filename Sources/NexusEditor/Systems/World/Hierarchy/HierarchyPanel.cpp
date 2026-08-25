#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyItem* HierarchyPanel::GetItem(NxFr::GUID Id)
	{
		return Manager->GetItem(Id);
	}

	void HierarchyPanel::SetManager(HierarchyManager* Manager)
	{
		this->Manager = Manager;
	}

	void HierarchyPanel::Find(NxFr::StringView Query)
	{
		Filter.Clear();
		Filter += Query;

		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}

		NxEn::World* World = GetItem(Root)->GetTarget()->GetWorld();
		NxFr::Array<NxFr::GUID> Ids = Manager->Worlds->FindGameObjects(Filter, World->GetId());
		for (auto& Id : Ids)
		{
			SetVisible(Id);
			Filtered.Append(Id);
		}
	}

	void HierarchyPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetNameId("Hierarchy");
	}

	void HierarchyPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			NxFr::Handle<NxEn::GameObject> Target = GetItem(Root)->GetTarget();
			Manager->SetEditContext(Target->GetWorldId());
		}

		TreePanel::OnDraw();
	}

	void HierarchyPanel::OnSelectItem(NxFr::GUID Id, bool State)
	{
		Manager->SelectItem(GetItem(Id)->GetTarget(), State);
	}
}
