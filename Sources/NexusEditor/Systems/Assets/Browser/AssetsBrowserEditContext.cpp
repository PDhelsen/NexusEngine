#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
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

			NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
			for (auto Id : Ids)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(Id);
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
			NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
			for (auto Id : Ids)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(Id);
				Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeName(Instance->GetTargetPath(), Input));
			}
		});
	}

	void AssetsBrowserEditContext::Duplicate()
	{
		NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
		for (auto Id : Ids)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
			Browser->Duplicate(Instance->GetTargetPath(), Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Delete()
	{
		NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
		for (auto Id : Ids)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
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

		for (auto Id : Clipboard)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
			Browser->Duplicate(Instance->GetTargetPath(), NxFr::Path::ChangeFolder(Instance->GetTargetPath(), Target->GetTargetPath()));
		}

		if (IsCutting)
		{
			for (auto Id : Clipboard)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(Id);
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

		for (auto Id : Selection)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
			AssetsBrowserItem* Parent = Instance->GetParent();
			bool Selected = false;

			while (Parent)
			{
				if (Selection.TryGet(Parent->GetId()))
				{
					Selected = true;
					break;
				}
				Parent = Parent->GetParent();
			}

			if (!Selected)
			{
				Result.Append(Id);
			}
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	void AssetsBrowserEditContext::OnSelectionChanged(NxFr::GUID InstanceId, bool State) const
	{
		AssetsBrowserItem* Instance = Browser->GetItem(Id);
		Browser->SelectItem(Instance, State, GetId());
	}

	AssetsBrowserEditContext::AssetsBrowserEditContext(AssetsBrowser* Browser)
		: Edit::Context(ContextId), Browser(Browser), IsCutting(false)
	{
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
	}
}
