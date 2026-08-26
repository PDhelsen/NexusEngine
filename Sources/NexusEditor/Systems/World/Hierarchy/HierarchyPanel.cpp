#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	static HierarchyPanel* Panel = NxEn::GUI::Panel::Create<HierarchyPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemHierarchy = NxEn::GUI::Menu::Create("Object/World/Hierarchy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel HierarchyPanel");
	}));

	HierarchyItem* HierarchyPanel::GetItem(NxFr::GUID Id)
	{
		if (!Manager)
		{
			return nullptr;
		}

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
		if (Filter.IsEmpty() || !Manager)
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
		if (Manager && NxEn::GUI::Utils::IsPanelActive())
		{
			Manager->SetEditContext();
		}

		TreePanel::OnDraw();
	}

	void HierarchyPanel::OnSelectItem(NxFr::GUID Id, bool State)
	{
		if (!Manager)
		{
			return;
		}

		Manager->SelectItem(GetItem(Id)->GetTarget(), State);
	}
}
