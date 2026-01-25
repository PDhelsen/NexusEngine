#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	static HierarchyPanel* Panel = NxEn::GUI::Panel::Create<HierarchyPanel>();

	const static NxEn::GUI::Menu::Item MenuItemHierarchy = NxEn::GUI::Menu::Item::Create("Object/World/Hierarchy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel HierarchyPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyPanel)

	void HierarchyPanel::Clear()
	{
		if (Root)
		{
			RemoveItem(static_cast<HierarchyItem*>(Root)->Target);
		}

		TreePanel::Clear();
	}

	void HierarchyPanel::Refresh()
	{
		TreePanel::Refresh();
		SelectGameObject(NxFr::Handle<NxEn::GameObject>());

		WorldsIds = Worlds->GetWorlds();
		WorldIndex = WorldsIds.Find(WorldId).Id();

		NxFr::Array<NxFr::StringView> WorldsLabels = NxFr::Array<NxFr::StringView>(WorldsIds.GetCount());
		for (uint64 Index = 0; Index < WorldsIds.GetCount(); ++Index)
		{
			WorldsLabels[Index] = Worlds->GetWorld(WorldsIds[Index])->GetName();
		}

		Menu.Remove("Worlds/");
		Menu.AddMenuEnum("Worlds/Worlds", &WorldIndex, WorldsLabels, [&]() { SelectWorld(WorldsIds[WorldIndex]); }, 1);
	}

	void HierarchyPanel::SelectWorld(NxFr::StringId Id)
	{
		WorldId = Id;
		Refresh();
	}

	void HierarchyPanel::SelectGameObject(NxFr::Handle<NxEn::GameObject> Target)
	{
		TreePanel::Select(HierarchyItem::Convert(Target));
	}

	void HierarchyPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetTitle("Hierarchy");

		AppendAction<HierarchyActionCreate>();
		AppendAction<HierarchyActionRename>();
		AppendAction<HierarchyActionDuplicate>();
		AppendAction<HierarchyActionMove>();
		AppendAction<HierarchyActionDelete>();
		AppendAction<HierarchyActionPrefabCreate>();
		AppendAction<HierarchyActionPrefabSave>();
		AppendAction<HierarchyActionPrefabUnpack>();
	}

	void HierarchyPanel::OnEnable()
	{
		WorldId = NxEn::WorldSystem::WorldId;
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Worlds->GetOnGameObjectEvent() += { this, & HierarchyPanel::OnHierarchyChanged };

		TreePanel::OnEnable();
	}

	void HierarchyPanel::OnDisable()
	{
		TreePanel::OnDisable();

		Worlds->GetOnGameObjectEvent() -= { this, & HierarchyPanel::OnHierarchyChanged };
		Worlds = nullptr;
	}

	NxEn::TreeItem* HierarchyPanel::FetchRootItem()
	{
		NxFr::Handle<NxEn::GameObject> Root = GetWorld()->GetRootGameObject();
		AppendItem(Root);
		return HierarchyItem::Convert(Root);
	}

	void HierarchyPanel::OnHierarchyChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId)
	{
		if (WorldId != this->WorldId)
		{
			return;
		}

		NxFr::Handle<NxEn::GameObject> GameObject = GetWorld()->GetGameObject(GameObjectId);
		if (EventId == NxEn::WorldSystem::AppendedId)
		{
			AppendItem(GameObject);
		}
		else if (EventId == NxEn::WorldSystem::RemovedId)
		{
			RemoveItem(GameObject);
		}
	}

	void HierarchyPanel::AppendItem(NxFr::Handle<NxEn::GameObject> Target)
	{
		HierarchyItem* Instance = new HierarchyItem(Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		OnCreateItem(Instance);

		NxFr::Handle<NxEn::GameObject> Iterator = Target->GetChild();
		while (Iterator)
		{
			AppendItem(Iterator);
			Iterator = Iterator->GetNext();
		}
	}

	void HierarchyPanel::RemoveItem(NxFr::Handle<NxEn::GameObject> Target)
	{
		HierarchyItem* Instance = HierarchyItem::Convert(Target);

		HierarchyItem* Iterator = Instance->GetChild();
		while (Iterator)
		{
			HierarchyItem* Next = Iterator->GetNext();
			RemoveItem(Iterator->Target);
			Iterator = Next;
		}

		OnDestroyItem(Instance);

		Instance->SetEnabled(false);
		Instance->Shutdown();
		delete Instance;
	}

	void HierarchyPanel::Find()
	{
		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}

		NxFr::Array<NxFr::Handle<NxEn::GameObject>> GameObjects = GetWorld()->Find(Filter);
		for (auto& Instance : GameObjects)
		{
			NxEn::TreeItem* Item = HierarchyItem::Convert(Instance);
			Show(Item);
			Filtered.Append(Item);
		}
	}
}
