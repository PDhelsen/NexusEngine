#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabSave.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabSave)

		HierarchyActionPrefabSave::HierarchyActionPrefabSave()
		: TreeAction("Prefab - Save", 7, false, false)
	{
	}

	HierarchyActionPrefabSave::~HierarchyActionPrefabSave()
	{
	}

	void HierarchyActionPrefabSave::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetGameObject();
			System->SavePrefab(Instance);
		}
	}
}
