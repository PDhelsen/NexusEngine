#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserActionDuplicate::AssetsBrowserActionDuplicate(NxFr::StringView Label, int64 Priority)
		: AssetsBrowserAction(Label, Priority)
	{
	}

	AssetsBrowserActionDuplicate::~AssetsBrowserActionDuplicate()
	{
	}

	void AssetsBrowserActionDuplicate::Execute(AssetsBrowserItem* Item)
	{
		AssetsBrowserPanel* Browser = NxEn::GUISystem::GetPanel<AssetsBrowserPanel>();

		NxFr::String Path = Item->GetPath();
		NxFr::StringView Name = Item->GetPrettyName();
		NxFr::StringView Extension = Item->GetExtension();
		bool Directory = Item->IsDirectory();

		uint64 Count = 1;
		NxFr::String Target;
		while (Browser->Exist(Path))
		{
			Target = Name + " " + NxFr::StringUtility::ToString(Count);
			if (Directory)
			{
				NxFr::Path::ChangeDirectoryName(Path, Target);
			}
			else
			{
				Target += "." + Extension;
				NxFr::Path::ChangeFileName(Path, Target);
			}

			Count++;
		}

		Browser->Duplicate(Item->GetPath(), Path);
	}
}
