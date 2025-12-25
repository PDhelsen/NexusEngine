#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabUnpack.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabUnpack)

	HierarchyActionPrefabUnpack::HierarchyActionPrefabUnpack()
		: HierarchyAction("Prefab - Unpack", 8)
	{
	}

	HierarchyActionPrefabUnpack::~HierarchyActionPrefabUnpack()
	{
	}

	void HierarchyActionPrefabUnpack::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		Unpack(NxEn::Application::GetSystem<NxEn::WorldSystem>(), Items);
	}

	void HierarchyActionPrefabUnpack::Unpack(NxEn::WorldSystem* System, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const
	{
		for (auto& Item : Items)
		{
			Unpack(System, Item);
		}
	}

	void HierarchyActionPrefabUnpack::Unpack(NxEn::WorldSystem* System, NxFr::Handle<NxEn::GameObject> Item) const
	{
		System->UnpackPrefab(Item);
	}
}
