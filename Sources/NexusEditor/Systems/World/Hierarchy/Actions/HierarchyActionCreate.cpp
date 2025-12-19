#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionCreate)

	HierarchyActionCreate::HierarchyActionCreate()
		: HierarchyAction("Create", 1)
	{
	}

	HierarchyActionCreate::~HierarchyActionCreate()
	{
	}

	void HierarchyActionCreate::Execute(const NxFr::Array<NxEn::GameObject*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Create(Items[0]->GetWorld(), Items, Input);
		});
	}

	void HierarchyActionCreate::Create(NxEn::World* World, const NxFr::Array<NxEn::GameObject*>& Items, NxFr::StringView Name) const
	{
		for (auto& Item : Items)
		{
			Create(World, Item, Name);
		}
	}

	void HierarchyActionCreate::Create(NxEn::World* World, NxEn::GameObject* Item, NxFr::StringView Name) const
	{
		World->CreateGameObject(Name, Item);
	}
}
