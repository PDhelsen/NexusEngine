#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionBehaviour.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionBehaviour)

	HierarchyActionBehaviour::HierarchyActionBehaviour()
		: HierarchyAction("Behaviour", 10)
	{
	}

	HierarchyActionBehaviour::~HierarchyActionBehaviour()
	{
	}

	void HierarchyActionBehaviour::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Items[0]->GetWorld()->CreateBehaviour(Input, Items[0]);
		});
	}
}
