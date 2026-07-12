#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyManager::HierarchyManager()
		: Worlds(nullptr),
		Items(), Root(nullptr), Panel(nullptr)
	{
		Panel = NxEn::GUISystem::GetPanel<HierarchyPanel>();
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		Panel->Manager = this;
		Context.Manager = this;

		Worlds->GetOnWorldObjectChange() += { this, &HierarchyManager::OnHierarchyChanged };
		Edit->RegisterContext(Context.GetId(), &Context);

		Refresh();
	}

	HierarchyManager::~HierarchyManager()
	{
		ClearItems();

		Edit->UnregisterContext(Context.GetId());
		Worlds->GetOnWorldObjectChange() -= { this, &HierarchyManager::OnHierarchyChanged };
	}

	void HierarchyManager::Refresh()
	{
		ClearItems();
		Root = FetchItems();

		Root->Open(true);

		Panel->Root = Root;
	}

	void HierarchyManager::Select(NxFr::GUID Id)
	{
		Panel->Select(GetItem(Id));
	}

	void HierarchyManager::Select(NxFr::Handle<NxEn::GameObject> Instance)
	{
		Panel->Select(GetItem(Instance));
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

	HierarchyItem* HierarchyManager::FetchItems()
	{
		NxEn::World* World = Worlds->GetWorld();
		NxFr::Handle<NxEn::GameObject> Instance = World->GetRoot();

		AppendItem(Instance);
		return GetItem(Instance);
	}

	void HierarchyManager::ClearItems()
	{
		for (auto [Id, Item] : Items)
		{
			delete Item;
		}

		Items.Clear();
		Panel->Clear();
		Edit->Unselect(Context.GetId());
	}

	void HierarchyManager::AppendItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		HierarchyItem* Item = new HierarchyItem(this, Instance);
		Items.Append(Instance->GetId(), Item);

		Panel->OnCreateItem(Item);

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

		Edit->Unselect(Instance->GetId(), Context.GetId());
		Panel->OnDestroyItem(Item);
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

		if (SelectionContextId != Panel->GetId())
		{
			Panel->SelectItem(GetItem(Instance->GetId()), State, true, false);
		}
		if (SelectionContextId != Context.GetId())
		{
			if (State)
			{
				Edit->Select(Instance->GetId(), Context.GetId());
			}
			else
			{
				Edit->Unselect(Instance->GetId(), Context.GetId());
			}
		}

		SelectionContextId = 0;
	}
}
