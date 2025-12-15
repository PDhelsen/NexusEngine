#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionMove)

	AssetsBrowserActionMove::AssetsBrowserActionMove()
		: AssetsBrowserAction("Move", 4, false)
	{
	}

	AssetsBrowserActionMove::~AssetsBrowserActionMove()
	{
	}

	void AssetsBrowserActionMove::Execute(const NxFr::Array<AssetsBrowserItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			if (Items.GetCount() == 1)
			{
				Move(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>(), Items[0], Input);
			}
			else
			{
				Move(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>(), Items, Input);
			}
		});
	}

	void AssetsBrowserActionMove::Move(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items, NxFr::StringView Input) const
	{
		for (auto& Item : Items)
		{
			Browser->Move(Item->GetPath(), NxFr::Path::Combine(Input, Item->GetName()));
		}
	}

	void AssetsBrowserActionMove::Move(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item, NxFr::StringView Input) const
	{
		Browser->Move(Item->GetPath(), Input);
	}

}
