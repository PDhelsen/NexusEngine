#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabUnpack.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabUnpack)

	HierarchyActionPrefabUnpack::HierarchyActionPrefabUnpack()
		: TreeAction("Prefab - Unpack", 8, false, false)
	{
	}

	HierarchyActionPrefabUnpack::~HierarchyActionPrefabUnpack()
	{
	}

	void HierarchyActionPrefabUnpack::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetGameObject();
			System->UnpackPrefab(Instance);
		}
	}
}
