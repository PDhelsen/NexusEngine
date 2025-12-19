#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionRename.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionRename)

	HierarchyActionRename::HierarchyActionRename()
		: HierarchyAction("Rename", 3)
	{
	}

	HierarchyActionRename::~HierarchyActionRename()
	{
	}

	void HierarchyActionRename::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Rename(Items[0]->GetWorld(), Items, Input);
		});
	}

	void HierarchyActionRename::Rename(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items, NxFr::StringView Input) const
	{
		for (auto& Item : Items)
		{
			Rename(World, Item, Input);
		}
	}

	void HierarchyActionRename::Rename(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item, NxFr::StringView Input) const
	{
		Item->SetName(Input);
	}
}
