#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionComponent.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionComponent)

	HierarchyActionComponent::HierarchyActionComponent()
		: TreeAction("Component", 11, false, false)
	{
	}

	HierarchyActionComponent::~HierarchyActionComponent()
	{
	}

	void HierarchyActionComponent::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Handle<NxEn::GameObject> Item = static_cast<HierarchyItem*>(Items[0])->GetGameObject();
			NxEn::World* World = Item->GetWorld();

			World->CreateComponent(Input, Item);
		});
	}
}
