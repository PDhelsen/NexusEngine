#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionCreate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserActionCreate)

	AssetsBrowserActionCreate::AssetsBrowserActionCreate()
		: TreeAction("Create", 1, false, false)
	{
	}

	AssetsBrowserActionCreate::~AssetsBrowserActionCreate()
	{
	}

	void AssetsBrowserActionCreate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);

			for (auto& Item : Items)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);

				NxFr::StringView Directory = Instance->IsDirectory() ? Instance->GetPath() : Instance->GetDirectory();
				NxFr::String Path = NxFr::Path::Combine(Directory, Name);
				AssetsBrowser::Create(Path, Type);
			}
		});
	}
}
