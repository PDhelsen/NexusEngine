#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionRename.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionRename)

	HierarchyActionRename::HierarchyActionRename()
		: TreeAction("Rename", 3, false, false)
	{
	}

	HierarchyActionRename::~HierarchyActionRename()
	{
	}

	void HierarchyActionRename::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::World* World = static_cast<HierarchyItem*>(Items[0])->GetGameObject()->GetWorld();
			for (auto& Item : Items)
			{
				NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetGameObject();
				Instance->SetName(Input);
			}
		});
	}
}
