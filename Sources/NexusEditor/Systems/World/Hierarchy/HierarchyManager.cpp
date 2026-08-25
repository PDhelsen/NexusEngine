#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyManager::HierarchyManager()
		: Worlds(nullptr), Edit(nullptr),
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
		HierarchyPanel* Panel = new HierarchyPanel();
		Panel->Initialize();
		Panels.Append(Target->GetId(), Panel);

		HierarchyEditContext* Context = new HierarchyEditContext(this, Target->GetId());
		Edit->RegisterContext(Context);
		Contexts.Append(Target->GetId(), Context);

		Panel->SetManager(this);
		Panel->SetRoot(Target->GetRoot()->GetId());
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

	HierarchyItem* HierarchyManager::GetItem(NxFr::GUID GameObjectId)
	{
		HierarchyItem** Item = Items.TryGet(GameObjectId);
		return Item ? *Item : nullptr;
	}

	void HierarchyManager::FetchItems()
	{
		NxFr::Array<NxFr::GUID> WorldsIds = Worlds->GetWorlds();
		for (auto WorldId : WorldsIds)
		{
			NxEn::World* World = Worlds->GetWorld(WorldId);
			NxFr::Handle<NxEn::GameObject> Root = World->GetRoot();

			AppendItem(Root);
		}
	}

	void HierarchyManager::ClearItems()
	{
		for (auto [WorldId, Panel] : Panels)
		{
			Panel->Clear();
		}

		for (auto [WorldId, Context] : Contexts)
		{
			Edit->Unselect(Context->GetId());
		}

		for (auto [GameObjectId, Item] : Items)
		{
			delete Item;
		}

		Items.Clear();
	}

	void HierarchyManager::AppendItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		HierarchyItem* Item = new HierarchyItem();
		Item->Target = Instance;

		for (auto [WorldId, Panel] : Panels)
		{
			Panel->OnCreateItem(Item->GetId());
		}

		NxFr::Handle<NxEn::GameObject> Iterator = Instance->GetChild();
		while (Iterator)
		{
			AppendItem(Iterator);
			Iterator = Iterator->GetNext();
		}

		Items.Append(Item->GetId(), Item);
	}

	void HierarchyManager::RemoveItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		HierarchyItem* Item = GetItem(Instance->GetId());

		NxFr::Handle<NxEn::GameObject> Iterator = Instance->GetChild();
		while (Iterator)
		{
			RemoveItem(Iterator);
			Iterator = Iterator->GetNext();
		}

		for (auto [WorldId, Panel] : Panels)
		{
			Panel->OnDestroyItem(Item->GetId());
		}

		Items.Remove(Item->GetId());
		delete Item;
	}

	void HierarchyManager::SelectItem(NxFr::Handle<NxEn::GameObject> Instance, bool State)
	{
		NxFr::GUID WorldId = Instance->GetWorldId();

		Panels[WorldId]->Select(Instance->GetId(), State, true, false);
		Edit->SetSelected(Instance->GetId(), State, Contexts[WorldId]->GetId());
	}

	void HierarchyManager::OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId)
	{
		NxFr::Handle<NxEn::Object> GameObject = Worlds->GetObject(GameObjectId, WorldId);
		if (!GameObject || GameObject->GetObjectType() != NxEn::GameObject::GetClassType())
		{
			return;
		}

		if (EventId == NxEn::WorldSystem::EventCreatedId)
		{
			AppendItem(GameObject);
		}
		else if (EventId == NxEn::WorldSystem::EventDestroyedId)
		{
			RemoveItem(GameObject);
		}
	}

	void HierarchyManager::SetEditContext(NxFr::GUID WorldId)
	{
		Edit->SetContext(Contexts[WorldId]->GetId());
	}
}
