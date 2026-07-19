#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"
#include "NexusEditor/Core/NexusEditorApplication.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	void HierarchyActionCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			for (auto& Item : Items)
			{
				NxFr::Handle<NxEn::GameObject> Parent = static_cast<HierarchyItem*>(Item)->GetTarget();
				System->CreateGameObject(Input, Parent, Parent->GetWorld()->GetId());
			}
		});
	}

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

	void HierarchyActionDuplicate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			System->DuplicateGameObject(Instance);
		}
	}

	void HierarchyActionMove::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		if (Items.GetCount() <= 1)
		{
			return;
		}

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::GameObject> Parent = static_cast<HierarchyItem*>(Items[0])->GetTarget();
		NxEn::World* World = Parent->GetWorld();

		for (uint64 Index = 1; Index < Items.GetCount(); Index++)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Items[Index])->GetTarget();
			System->AttachGameObject(Instance, Parent);
		}
	}

	void HierarchyActionDelete::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			System->DestroyGameObject(Instance);
		}
	}

	void HierarchyActionInspect::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		HierarchyItem* Item = static_cast<HierarchyItem*>(Items[0]);

		NexusEditorApplication* Editor = NxEn::Application::GetInstance<NexusEditorApplication>();
		InspectorPanel* Inspector = Editor->GetStageManager()->GetFocusedStage()->GetInspector();

		Inspector->Show(Item->GetTarget(), true);
	}

	void HierarchyActionPrefabCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
			NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();

			for (auto& Item : Items)
			{
				NxFr::String Path = NxFr::Path::IsDirectory(Input) ? Input + Item->GetItemName() : NxFr::String(Input);
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
				NxEn::Prefab* Prefab = Assets->Create<NxEn::Prefab>(Path, NxEn::Prefab::Extension);
				Prefab->SetRoot(Instance);
			}
		});
	}

	void HierarchyActionPrefabPack::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();

		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			NxEn::Prefab* Prefab = Assets->Load<NxEn::Prefab>(Instance->GetId());
			Prefab->SetRoot(Instance);
		}
	}

	void HierarchyActionPrefabUnpack::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetTarget();
			System->UnpackPrefab(Instance);
		}
	}
}
