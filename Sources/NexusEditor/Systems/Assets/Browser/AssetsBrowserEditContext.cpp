#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserEditContext::AssetsBrowserEditContext(AssetsBrowser* Browser)
		: Edit::Context(ContextId), Browser(Browser)
	{
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetAll()
	{
		return NxFr::ContainerUtility::ToArrayKeys(Browser->Items);
	}

	uint64 AssetsBrowserEditContext::GetCount()
	{
		return Browser->Items.GetCount();
	}

	void AssetsBrowserEditContext::Create()
	{
		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);

			NxFr::Array<NxFr::GUID> InstanceIds = GetSelection(true);
			for (auto InstanceId : InstanceIds)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				if (Instance->GetObjectType() != AssetsBrowserItemDirectory::GetClassType())
				{
					NX_LOG(Error, System, "Can only create AssetsBrowserItem in directory");
					continue;
				}

				NxFr::String Path = NxFr::Path::Combine(Instance->GetTargetPath(), Name);
				Browser->Create(Type, Path);
			}
		});
	}

	void AssetsBrowserEditContext::Rename()
	{
		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Array<NxFr::GUID> InstanceIds = GetSelection(true);
			for (auto InstanceId : InstanceIds)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeName(Instance->GetTargetPath(), Input));
			}
		});
	}

	void AssetsBrowserEditContext::Duplicate()
	{
		NxFr::Array<NxFr::GUID> InstanceIds = GetSelection(true);
		for (auto InstanceId : InstanceIds)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			Browser->Duplicate(Instance->GetTargetPath(), Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Delete()
	{
		NxFr::Array<NxFr::GUID> InstanceIds = GetSelection(true);
		for (auto InstanceId : InstanceIds)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			Browser->Delete(Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Paste()
	{
		AssetsBrowserItem* Target = Browser->GetItem(Selected);
		if (Target->GetObjectType() != AssetsBrowserItemDirectory::GetClassType())
		{
			NX_LOG(Error, System, "Can only paste AssetsBrowserItem in directory");
			return;
		}

		for (auto InstanceId : Clipboard)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			Browser->Duplicate(Instance->GetTargetPath(), NxFr::Path::ChangeFolder(Instance->GetTargetPath(), Target->GetTargetPath()));
		}

		if (IsCutting)
		{
			for (auto InstanceId : Clipboard)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				Browser->Delete(Instance->GetTargetPath());
			}
		}

		Context::Paste();
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetSelection(bool Filtered) const
	{
		if (!Filtered)
		{
			return Context::GetSelection(Filtered);
		}

		NxFr::Set<NxFr::GUID> Result = Selection.GetCapacity();

		for (auto InstanceId : Selection)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			AssetsBrowserItem* Parent = Browser->GetItem(Instance->GetParent());
			bool Selected = false;

			while (Parent)
			{
				if (Selection.TryGet(Parent->GetId()))
				{
					Selected = true;
					break;
				}
				Parent = Browser->GetItem(Parent->GetParent());
			}

			if (!Selected)
			{
				Result.Append(InstanceId);
			}
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	void AssetsBrowserEditContext::OnSelectionChanged(NxFr::GUID InstanceId, bool State) const
	{
		Browser->Select(InstanceId, State);
	}
}
