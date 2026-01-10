#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDuplicate.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionRename.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionMove.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDelete.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabSave.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabUnpack.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionInspect.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionBehaviour.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionComponent.h"

namespace NxEd
{
	static HierarchyPanel* Panel = NxEn::GUI::Panel::Create<HierarchyPanel>();

	const static NxEn::GUI::Menu::Item MenuItemHierarchy = NxEn::GUI::Menu::Item::Create("Object/World/Hierarchy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel HierarchyPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyPanel)

	void HierarchyPanel::Refresh()
	{
		RefreshWorld();
		RefreshGameObjects();
	}

	void HierarchyPanel::RefreshWorld()
	{
		WorldsIds = Worlds->GetWorlds();
		WorldsLabels = NxFr::Array<NxFr::StringView>(WorldsIds.GetCount());

		for (uint64 Index = 0; Index < WorldsIds.GetCount(); ++Index)
		{
			WorldsLabels[Index] = Worlds->GetWorld(WorldsIds[Index])->GetName();
		}

		Menu.Remove("Worlds/");
		Menu.AddMenuItem("Worlds/Refresh", { this, &HierarchyPanel::RefreshWorld });
		Menu.AddMenuEnum("Worlds/Worlds", &WorldIndex, WorldsLabels, [&]() { SelectWorld(WorldsIds[WorldIndex]); }, 1);

		SelectWorld(NxEn::WorldSystem::WorldId);
	}

	void HierarchyPanel::RefreshGameObjects()
	{
		Clear();
		Root = FetchItems();

		SelectGameObject(NxFr::Handle<NxEn::GameObject>());
	}

	void HierarchyPanel::SelectWorld(NxFr::StringId Id)
	{
		WorldIndex = WorldsIds.Find(Id).Id();
		Refresh();
	}

	void HierarchyPanel::SelectGameObject(NxFr::Handle<NxEn::GameObject> Target)
	{
		if (!GetWorld()->Belong(Target))
		{
			return;
		}

		TreePanel::Select(Target->GetId(), false, false);
	}

	void HierarchyPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetTitle("Hierarchy");

		AppendAction<HierarchyActionCreate>();
		AppendAction<HierarchyActionDuplicate>();
		AppendAction<HierarchyActionRename>();
		AppendAction<HierarchyActionMove>();
		AppendAction<HierarchyActionDelete>();
		AppendAction<HierarchyActionPrefabCreate>();
		AppendAction<HierarchyActionPrefabSave>();
		AppendAction<HierarchyActionPrefabUnpack>();
		AppendAction<HierarchyActionInspect>();
		AppendAction<HierarchyActionBehaviour>();
		AppendAction<HierarchyActionComponent>();
	}

	void HierarchyPanel::OnEnable()
	{
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Worlds->GetOnGameObjectEvent() += { this, &HierarchyPanel::OnGameObjectChanged };

		TreePanel::OnEnable();
	}

	void HierarchyPanel::OnDisable()
	{
		Worlds->GetOnGameObjectEvent() -= { this, &HierarchyPanel::OnGameObjectChanged };

		TreePanel::OnDisable();
	}

	HierarchyItem* HierarchyPanel::GetItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		return Instance ? static_cast<HierarchyItem*>(TreePanel::GetItem(Instance->GetId())) : nullptr;
	}

	HierarchyItem* HierarchyPanel::FetchItems()
	{
		NxFr::Array<NxFr::Handle<NxEn::GameObject>> Instances = GetWorld()->GetGameObjects();
		NxFr::Handle<NxEn::GameObject> Root = GetWorld()->GetRootGameObject();

		for (auto& Instance : Instances)
		{
			AppendItem(Instance);
		}

		return GetItem(Root);
	}

	void HierarchyPanel::AppendItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		TreePanel::AppendItem(new HierarchyItem(Instance, { this, &HierarchyPanel::GetItem }));
	}

	void HierarchyPanel::RemoveItem(NxFr::Handle<NxEn::GameObject> Instance)
	{
		HierarchyItem* Item = GetItem(Instance);
		if (!Item || !Items.ContainsKey(Item->GetId()))
		{
			return;
		}

		Item->SetEnabled(false);
		Item->Shutdown();

		while (Item->GetChild())
		{
			HierarchyItem* Child = static_cast<HierarchyItem*>(Item->GetChild());
			RemoveItem(Child->GetGameObject());
		}

		if (Item == Selected)
		{
			Selected = nullptr;
		}
		if (Selection.Contains(Item))
		{
			Selection.Remove(Item);
		}
		if (Filtered.Contains(Item))
		{
			Filtered.Remove(Item);
		}

		Items.Remove(Item->GetId());
		delete Item;
	}

	void HierarchyPanel::OnGameObjectChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId)
	{
		NxEn::World* World = GetWorld();
		if (!World || World->GetId() != WorldId)
		{
			return;
		}

		NxFr::Handle<NxEn::GameObject> GameObject = World->GetGameObject(GameObjectId);
		if (EventId == NxEn::WorldSystem::AppendedId)
		{
			AppendItem(GameObject);
		}
		else if (EventId == NxEn::WorldSystem::RemovedId)
		{
			RemoveItem(GameObject);
		}

		NxFr::Handle<NxEn::GameObject> Child = GameObject->GetChild();
		while (Child)
		{
			OnGameObjectChanged(EventId, WorldId, Child->GetGameObjectId());
			Child = Child->GetNext();
		}
	}

	void HierarchyPanel::Find()
	{
		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}
		
		NxFr::Array<NxFr::Handle<NxEn::GameObject>> GameObjects = GetWorld()->Find(Filter);
		for (auto Instance : GameObjects)
		{
			NxEn::TreeItem* Item = GetItem(Instance);
			Show(Item);
			Filtered.Append(Item);
		}
	}
}
