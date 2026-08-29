#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	static HierarchyPanel* Panel = NxEn::GUI::Panel::Create<HierarchyPanel>(false);

	static const NxEn::GUI::Menu::Item* MenuItemHierarchy = NxEn::GUI::Menu::Create("Object/World/Hierarchy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel HierarchyPanel");
	}));

	HierarchyItem* HierarchyPanel::GetItem(NxFr::GUID InstanceId)
	{
		return Manager->GetItem(InstanceId);
	}

	void HierarchyPanel::Initialize(HierarchyManager* Manager, HierarchyEditContext* Context)
	{
		this->Manager = Manager;
		this->Context = Context;
		this->Actions = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<HierarchyPanel>()->Actions;

		TreePanel::Initialize();
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

		NxFr::Array<NxFr::GUID> InstanceIds = Manager->Worlds->FindGameObjects(Filter, NxEn::WorldSystem::MainWorldId);
		for (auto& InstanceId : InstanceIds)
		{
			SetVisible(InstanceId);
			Filtered.Append(InstanceId);
		}
	}

	void HierarchyPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetNameId("Hierarchy");

		Edit = NxEn::Application::GetSystem<EditSystem>();
		Manager->Panels.Append(this);
	}

	void HierarchyPanel::OnShutdown()
	{
		Manager->Panels.Remove(this);
		TreePanel::OnShutdown();
	}

	void HierarchyPanel::OnDraw()
	{
		if (NxEn::GUI::Utils::IsPanelActive())
		{
			Edit->SetContext(Context->GetId());
		}

		TreePanel::OnDraw();
	}

	void HierarchyPanel::OnSelectItem(NxFr::GUID InstanceId, bool State)
	{
		Edit->SetSelected(InstanceId, State, Context->GetId());
	}
}
