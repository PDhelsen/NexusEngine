#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyManager::HierarchyManager()
		: Worlds(NxEn::Application::GetSystem<NxEn::WorldSystem>()), Items(), Panels()
	{
		Worlds->GetOnGameObjectEvent() += { this, &HierarchyManager::OnHierarchyChanged };

		FetchItems();
	}

	HierarchyManager::~HierarchyManager()
	{
		ClearItems();

		Worlds->GetOnGameObjectEvent() -= { this, &HierarchyManager::OnHierarchyChanged };
	}

	HierarchyPanel* HierarchyManager::CreatePanel()
	{
		HierarchyPanel* Panel = new HierarchyPanel(this);
		Panel->Initialize();

		Panels.Append(Panel->GetId(), Panel);
		return Panel;
	}

	void HierarchyManager::DestroyPanel(HierarchyPanel* Instance)
	{
		Panels.Remove(Instance->GetId());

		Instance->Shutdown();
		delete Instance;
	}

	HierarchyItem* HierarchyManager::Convert(NxFr::Handle<NxEn::GameObject> Target)
	{
		if (!Target)
		{
			return nullptr;
		}

		HierarchyItem** Instance = Items.TryGet(Target->GetId());
		return Instance ? *Instance : nullptr;
	}

	void HierarchyManager::OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId)
	{
		NxFr::Handle<NxEn::GameObject> GameObject = Worlds->GetWorld(WorldId)->GetGameObject(GameObjectId);
		if (EventId == NxEn::WorldSystem::AppendedId)
		{
			AppendItem(GameObject);
		}
		else if (EventId == NxEn::WorldSystem::RemovedId)
		{
			RemoveItem(GameObject);
		}
	}

	void HierarchyManager::FetchItems()
	{
		NxFr::Array<NxFr::GUID> WorldsIds = Worlds->GetWorlds();
		for (auto WorldId : WorldsIds)
		{
			NxEn::World * World = Worlds->GetWorld(WorldId);
			NxFr::Handle<NxEn::GameObject> Instances = World->GetRootGameObject();

			AppendItem(Instances);
		}
	}

	void HierarchyManager::ClearItems()
	{
		for (auto [Id, Instance] : Items)
		{
			delete Instance;
		}

		Items.Clear();
	}

	void HierarchyManager::AppendItem(NxFr::Handle<NxEn::GameObject> Target)
	{
		HierarchyItem* Instance = new HierarchyItem(this, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		Items.Append(Instance->GetItemId(), Instance);

		for (auto& [Id, Panel] : Panels)
		{
			Panel->OnCreateItem(Instance);
		}

		NxFr::Handle<NxEn::GameObject> Iterator = Target->GetChild();
		while (Iterator)
		{
			AppendItem(Iterator);
			Iterator = Iterator->GetNext();
		}
	}

	void HierarchyManager::RemoveItem(NxFr::Handle<NxEn::GameObject> Target)
	{
		HierarchyItem* Instance = Convert(Target);

		HierarchyItem* Iterator = Instance->GetChild();
		while (Iterator)
		{
			HierarchyItem* Next = Iterator->GetNext();
			RemoveItem(Iterator->Target);
			Iterator = Next;
		}

		for (auto& [Id, Panel] : Panels)
		{
			Panel->OnDestroyItem(Instance);
		}

		Items.Remove(Instance->GetItemId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		delete Instance;
	}
}
