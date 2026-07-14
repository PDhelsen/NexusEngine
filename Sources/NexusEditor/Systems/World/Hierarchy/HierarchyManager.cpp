#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyManager::HierarchyManager()
		: Worlds(nullptr),
		Items(), Panels(), Contexts()
	{
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		Worlds->GetOnWorldObjectChange() += { this, &HierarchyManager::OnHierarchyChanged };

		FetchItems();
	}

	HierarchyManager::~HierarchyManager()
	{
		ClearItems();

		Worlds->GetOnWorldObjectChange() -= { this, &HierarchyManager::OnHierarchyChanged };
	}

	HierarchyPanel* HierarchyManager::CreatePanel(NxEn::World* Target)
	{
		HierarchyPanel* Panel = new HierarchyPanel(this, GetItem(Target->GetRoot()));
		Panel->Initialize();
		Panels.Append(Target->GetId(), Panel);

		HierarchyEditContext* Context = new HierarchyEditContext(this, Target->GetId());
		Edit->RegisterContext(Context->GetId(), Context);
		Contexts.Append(Target->GetId(), Context);

		return Panel;
	}

	void HierarchyManager::DestroyPanel(NxEn::World* Target)
	{
		HierarchyEditContext* Context = Contexts[Target->GetId()];
		Contexts.Remove(Target->GetId());
		Edit->UnregisterContext(Context->GetId());
		delete Context;

		HierarchyPanel* Panel = Panels[Target->GetId()];
		Panels.Remove(Target->GetId());
		Panel->Shutdown();
		delete Panel;
	}

	void HierarchyManager::OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId)
	{
		NxFr::Handle<NxEn::GameObject> GameObject = Worlds->GetObject(GameObjectId, WorldId);
		if (EventId == NxEn::WorldSystem::EventCreatedId)
		{
			AppendItem(GameObject);
		}
		else if (EventId == NxEn::WorldSystem::EventDestroyedId)
		{
			RemoveItem(GameObject);
		}
	}

	void HierarchyManager::FetchItems()
	{
		NxFr::Array<NxFr::GUID> WorldsIds = Worlds->GetWorlds();
		for (auto WorldId : WorldsIds)
		{
			NxEn::World* World = Worlds->GetWorld(WorldId);
			NxFr::Handle<NxEn::GameObject> Instances = World->GetRoot();

			AppendItem(Instances);
		}
	}

	void HierarchyManager::ClearItems()
	{
		for (auto [Id, Context] : Contexts)
		{
			Edit->Unselect(Context->GetId());
			delete Context;
		}

		for (auto [Id, Panel] : Panels)
		{
			delete Panel;
		}

		for (auto [Id, Item] : Items)
		{
			delete Item;
		}

		Contexts.Clear();
		Panels.Clear();
		Items.Clear();
	}

	void HierarchyManager::AppendItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		HierarchyItem* Item = new HierarchyItem(this, Instance);
		Items.Append(Instance->GetId(), Item);

		for (auto [Id, Panel] : Panels)
		{
			Panel->OnCreateItem(Item);
		}

		NxFr::Handle<NxEn::GameObject> Iterator = Instance->GetChild();
		while (Iterator)
		{
			AppendItem(Iterator);
			Iterator = Iterator->GetNext();
		}
	}

	void HierarchyManager::RemoveItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		HierarchyItem* Item = GetItem(Instance);

		HierarchyItem* Iterator = Item->GetChild();
		while (Iterator)
		{
			HierarchyItem* Next = Iterator->GetNext();
			RemoveItem(Iterator->Target);
			Iterator = Next;
		}

		for (auto [Id, Panel] : Panels)
		{
			Panel->OnDestroyItem(Item);
		}

		for (auto [Id, Context] : Contexts)
		{
			Edit->Unselect(Instance->GetId(), Context->GetId());
		}

		Items.Remove(Item->GetItemId());
		delete Item;
	}

	HierarchyItem* HierarchyManager::GetItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		return Instance ? GetItem(Instance->GetId()) : nullptr;
	}

	HierarchyItem* HierarchyManager::GetItem(NxFr::GUID Id)
	{
		HierarchyItem** Item = Items.TryGet(Id);
		return Item ? *Item : nullptr;
	}

	void HierarchyManager::SelectItem(NxFr::Handle<NxEn::GameObject> Instance, bool State, NxFr::StringId SelectionId)
	{
		if (SelectionContextId.GetId() != 0)
		{
			return;
		}

		SelectionContextId = SelectionId;
		NxFr::GUID WorldId = Instance->GetWorldId();

		if (SelectionContextId != Panels[WorldId]->GetId())
		{
			Panels[WorldId]->SelectItem(GetItem(Instance->GetId()), State, true, false);
		}
		if (SelectionContextId != Contexts[WorldId]->GetId())
		{
			if (State)
			{
				Edit->Select(Instance->GetId(), Contexts[WorldId]->GetId());
			}
			else
			{
				Edit->Unselect(Instance->GetId(), Contexts[WorldId]->GetId());
			}
		}

		SelectionContextId = 0;
	}
}
