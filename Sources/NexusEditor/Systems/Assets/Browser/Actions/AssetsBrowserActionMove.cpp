#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionMove)

	AssetsBrowserActionMove::AssetsBrowserActionMove()
		: TreeAction("Move", 4, false, false)
	{
	}

	AssetsBrowserActionMove::~AssetsBrowserActionMove()
	{
	}

	void AssetsBrowserActionMove::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			AssetsBrowserPanel* Browser = NxEn::GUISystem::GetPanel<AssetsBrowserPanel>();
			if (Items.GetCount() == 1)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[0]);
				Browser->Move(Instance->GetPath(), Input);
			}
			else
			{
				for (auto& Item : Items)
				{
					AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
					Browser->Move(Instance->GetPath(), NxFr::Path::Combine(Input, Instance->GetName()));
				}
			}
		});
	}
}
