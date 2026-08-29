#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyManager::HierarchyManager()
		: Worlds(nullptr), Edit(nullptr),
		Items(), Panels(), Contexts()
	{
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		HierarchyPanel* Panel = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<HierarchyPanel>();
		HierarchyEditContext* Context = new HierarchyEditContext(this, Panel, HierarchyEditContext::ContextId);
		Panel->Initialize(this, Context);
		Edit->RegisterContext(Context);

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

		Edit::Context* Context = Edit->UnregisterContext(HierarchyEditContext::ContextId);
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

		NxFr::Array<NxFr::GUID> WorldsIds = Worlds->GetWorlds();
		for (auto WorldId : WorldsIds)
		{
			NxEn::World * World = Worlds->GetWorld(WorldId);
			NxFr::Handle<NxEn::GameObject> Root = World->GetRoot();

			AppendItem(Root);
		}

		NxFr::Handle<NxEn::GameObject> Root = Worlds->GetWorld(NxEn::WorldSystem::MainWorldId)->GetRoot();
		NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<HierarchyPanel>()->SetRoot(Root->GetId());
	}

	void HierarchyManager::ClearItems()
	{
		for (auto Panel : Panels)
		{
			Panel->Clear();
		}
		for (auto Context : Contexts)
		{
			Edit->Clear(Context->GetId());
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

		for (auto Panel : Panels)
		{
			Panel->OnCreateItem(Item->GetId());
		}
		for (auto Context : Contexts)
		{
			Context->OnCreated.Invoke(Item->GetId());
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

		for (auto Panel : Panels)
		{
			Panel->OnDestroyItem(Item->GetId());
		}
		for (auto Context : Contexts)
		{
			Context->OnDestroyed.Invoke(Item->GetId());
		}

		Items.Remove(Item->GetId());
		delete Item;
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
}
