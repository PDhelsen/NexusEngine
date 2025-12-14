#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionMove::AssetsBrowserActionMove(NxFr::StringView Label, int64 Priority)
		: AssetsBrowserAction(Label, Priority)
	{
	}

	AssetsBrowserActionMove::~AssetsBrowserActionMove()
	{
	}

	void AssetsBrowserActionMove::Execute(AssetsBrowserItem* Item)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Move(Item->GetPath(), Input);
		});
	}
}
