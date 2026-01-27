#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"

#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionCreate)

	void HierarchyActionCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			for (auto& Item : Items)
			{
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
				NxEn::World* World = Instance->GetWorld();

				World->CreateGameObject(Input, Instance);
			}
		});
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionRename)

	void HierarchyActionRename::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			for (auto& Item : Items)
			{
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();

				Instance->SetName(Input);
			}
		});
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionDuplicate)

	void HierarchyActionDuplicate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			NxEn::World* World = Instance->GetWorld();

			World->DuplicateGameObject(Instance);
		}
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionMove)

	void HierarchyActionMove::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		if (Items.GetCount() <= 1)
		{
			return;
		}

		NxFr::Handle<NxEn::GameObject> Parent = static_cast<HierarchyItem*>(Items[0])->GetTarget();
		NxEn::World* World = Parent->GetWorld();

		for (uint64 Index = 1; Index < Items.GetCount(); Index++)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Items[Index])->GetTarget();

			World->AttachGameObject(Instance, Parent);
		}
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionDelete)

	void HierarchyActionDelete::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			NxEn::World* World = Instance->GetWorld();

			World->DestroyGameObject(Instance);
		}
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabCreate)

	void HierarchyActionPrefabCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			for (auto& Item : Items)
			{
				NxFr::String Path = NxFr::Path::IsDirectory(Input) ? Input + Item->GetItemName() : NxFr::String(Input);
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();

				System->CreatePrefab(Instance, Path);
			}
		});
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabSave)

	void HierarchyActionPrefabSave::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			System->SavePrefab(Instance);
		}
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabUnpack)

	void HierarchyActionPrefabUnpack::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			System->UnpackPrefab(Instance);
		}
	}

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionInspect)

	void HierarchyActionInspect::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		HierarchyItem* Item = static_cast<HierarchyItem*>(Items[0]);

		InspectorPanel* Inspector = NxEn::GUISystem::GetPanel<InspectorPanel>();
		Inspector->Show(Item->GetTarget());
	}
}
