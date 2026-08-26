#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyManager::HierarchyManager()
		: Worlds(nullptr), Edit(nullptr),
		Items(), Panel(nullptr), Context(nullptr)
	{
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		Context = new HierarchyEditContext(this);
		Edit->RegisterContext(Context);
		Panel = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<HierarchyPanel>();
		Panel->SetManager(this);

		Panel->AppendAction(NxEn::TreeAction{ .Name = "Create", .Action = [&]() { Edit->Create(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Rename", .Action = [&]() { Edit->Rename(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Duplicate", .Action = [&]() { Edit->Duplicate(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Move", .Action = [&]() { Edit->Move(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Delete", .Action = [&]() { Edit->Delete(); }, .Priority = -1 });

		Worlds->GetOnWorldObjectChange() += { this, &HierarchyManager::OnHierarchyChanged };

		FetchItems();
	}

	HierarchyManager::~HierarchyManager()
	{
		ClearItems();

		Worlds->GetOnWorldObjectChange() -= { this, &HierarchyManager::OnHierarchyChanged };

		Edit->UnregisterContext(Context->GetId());
		delete Context;
	}

	HierarchyItem* HierarchyManager::GetItem(NxFr::GUID GameObjectId)
	{
		HierarchyItem** Item = Items.TryGet(GameObjectId);
		return Item ? *Item : nullptr;
	}

	void HierarchyManager::FetchItems()
	{
		ClearItems();

		NxEn::World* World = Worlds->GetWorld(NxEn::WorldSystem::MainWorldId);
		NxFr::Handle<NxEn::GameObject> Root = World->GetRoot();

		AppendItem(Root);
		Panel->SetRoot(Root->GetId());
	}

	void HierarchyManager::ClearItems()
	{
		Panel->Clear();
		Edit->Unselect(Context->GetId());

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

		Panel->OnCreateItem(Item->GetId());

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

		Panel->OnDestroyItem(Item->GetId());

		Items.Remove(Item->GetId());
		delete Item;
	}

	void HierarchyManager::SelectItem(HierarchyItem* Item, bool State)
	{
		Panel->Select(Item->GetId(), State, true, false);
		Edit->SetSelected(Item->GetId(), State, Context->GetId());
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

	void HierarchyManager::SetEditContext()
	{
		Edit->SetContext(Context->GetId());
	}
}
