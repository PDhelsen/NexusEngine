#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionPrefabCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionPrefabCreate)

	HierarchyActionPrefabCreate::HierarchyActionPrefabCreate()
		: HierarchyAction("Prefab - Create", 6)
	{
	}

	HierarchyActionPrefabCreate::~HierarchyActionPrefabCreate()
	{
	}

	void HierarchyActionPrefabCreate::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
				Create(NxEn::Application::GetSystem<NxEn::WorldSystem>(), Items, Input);
		});
	}

	void HierarchyActionPrefabCreate::Create(NxEn::WorldSystem* System, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items, NxFr::StringView Input) const
	{
		for (auto& Item : Items)
		{
			Create(System, Item, Input);
		}
	}

	void HierarchyActionPrefabCreate::Create(NxEn::WorldSystem* System, NxFr::Handle<NxEn::GameObject> Item, NxFr::StringView Input) const
	{
		NxFr::String Path = NxFr::Path::IsDirectory(Input) ? NxFr::Path::Combine(Input, Item->GetName()) : NxFr::String(Input);
		System->CreatePrefab(Item, Path);
	}
}
