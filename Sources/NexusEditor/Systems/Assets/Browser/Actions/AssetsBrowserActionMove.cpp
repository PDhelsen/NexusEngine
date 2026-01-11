#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
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
			if (Items.GetCount() == 1)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[0]);
				AssetsBrowser::Move(Instance->GetPath(), Input);
			}
			else
			{
				for (auto& Item : Items)
				{
					AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
					AssetsBrowser::Move(Instance->GetPath(), NxFr::Path::Combine(Input, Instance->GetName()));
				}
			}
		});
	}
}
