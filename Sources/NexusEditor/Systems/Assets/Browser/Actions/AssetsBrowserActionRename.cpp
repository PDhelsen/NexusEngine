#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionRename.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionRename)

	AssetsBrowserActionRename::AssetsBrowserActionRename()
		: TreeAction("Rename", 3, false, false)
	{
	}

	AssetsBrowserActionRename::~AssetsBrowserActionRename()
	{
	}

	void AssetsBrowserActionRename::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			for (auto& Item : Items)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);

				NxFr::String Path = Instance->GetPath();
				NxFr::String Parent = Instance->GetDirectory();

				if (Instance->IsDirectory())
				{
					Path = NxFr::Path::Combine((NxFr::StringView)Parent, Input);
				}
				else
				{
					Path = NxFr::Path::Combine((NxFr::StringView)Parent, (Input + "." + Instance->GetExtension()));
				}

				AssetsBrowser::Move(Instance->GetPath(), Path);
			}
		});
	}
}
