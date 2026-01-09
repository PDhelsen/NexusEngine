#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionBehaviour.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionBehaviour)

	HierarchyActionBehaviour::HierarchyActionBehaviour()
		: TreeAction("Behaviour", 10, false, false)
	{
	}

	HierarchyActionBehaviour::~HierarchyActionBehaviour()
	{
	}

	void HierarchyActionBehaviour::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Handle<NxEn::GameObject> Item = static_cast<HierarchyItem*>(Items[0])->GetGameObject();
			NxEn::World* World = Item->GetWorld();

			World->CreateBehaviour(Input, Item);
		});
	}
}
