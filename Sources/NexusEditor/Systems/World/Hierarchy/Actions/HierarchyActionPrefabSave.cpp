#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabSave.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabSave)

		HierarchyActionPrefabSave::HierarchyActionPrefabSave()
		: HierarchyAction("Prefab - Save", 7)
	{
	}

	HierarchyActionPrefabSave::~HierarchyActionPrefabSave()
	{
	}

	void HierarchyActionPrefabSave::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		Save(NxEn::Application::GetSystem<NxEn::WorldSystem>(), Items);
	}

	void HierarchyActionPrefabSave::Save(NxEn::WorldSystem* System, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const
	{
		for (auto& Item : Items)
		{
			Save(System, Item);
		}
	}

	void HierarchyActionPrefabSave::Save(NxEn::WorldSystem* System, NxFr::Handle<NxEn::GameObject> Item) const
	{
		System->SavePrefab(Item);
	}
}
