#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionCreate)

	HierarchyActionCreate::HierarchyActionCreate()
		: TreeAction("Create", 1, false, false)
	{
	}

	HierarchyActionCreate::~HierarchyActionCreate()
	{
	}

	void HierarchyActionCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::World* World = static_cast<HierarchyItem*>(Items[0])->GetGameObject()->GetWorld();
			for (auto& Item : Items)
			{
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetGameObject();

				World->CreateGameObject(Input, Instance);
			}
		});
	}
}
