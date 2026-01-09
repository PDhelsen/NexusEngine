#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabCreate)

	HierarchyActionPrefabCreate::HierarchyActionPrefabCreate()
		: TreeAction("Prefab - Create", 6, false, false)
	{
	}

	HierarchyActionPrefabCreate::~HierarchyActionPrefabCreate()
	{
	}

	void HierarchyActionPrefabCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			for (auto& Item : Items)
			{
				NxFr::String Path = NxFr::Path::IsDirectory(Input) ? NxFr::Path::Combine(Input, Item->GetName()) : NxFr::String(Input);
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetGameObject();
				System->CreatePrefab(Instance, Path);
			}
		});
	}
}
