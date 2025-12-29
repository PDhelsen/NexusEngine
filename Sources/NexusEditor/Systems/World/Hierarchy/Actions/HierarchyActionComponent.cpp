#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionComponent.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionComponent)

	HierarchyActionComponent::HierarchyActionComponent()
		: HierarchyAction("Component", 11)
	{
	}

	HierarchyActionComponent::~HierarchyActionComponent()
	{
	}

	void HierarchyActionComponent::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Items[0]->GetWorld()->CreateComponent(Input, Items[0]);
		});
	}
}
