#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserEditContext::AssetsBrowserEditContext()
		: Edit::Context(ContextId), Browser(nullptr), IsCutting(false)
	{
		OnSelectionChanged += { this, &AssetsBrowserEditContext::OnSelectItem };
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
		OnSelectionChanged -= { this, &AssetsBrowserEditContext::OnSelectItem };
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetAll()
	{
		return NxFr::ContainerUtility::ToArrayKeys(Browser->Items);
	}

	uint64 AssetsBrowserEditContext::GetCount()
	{
		return Browser->Items.GetCount();
	}

	void AssetsBrowserEditContext::Rename()
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Set<NxFr::GUID> Instances = FilterSelection();
			for (auto Id : Instances)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(Id);
				NxFr::String Path = Instance->GetTargetPath();
				NxFr::String Parent = Instance->GetDirectory();

				if (Instance->IsDirectory())
				{
					Path = NxFr::Path::Combine((NxFr::StringView)Parent, Input);
				}
				else
				{
					Path = NxFr::Path::Combine((NxFr::StringView)Parent, (Input + NxFr::Path::SeparatorExtension + Instance->GetExtension()));
				}

				Browser->Move(Instance->GetTargetPath(), Path);
			}
		});
	}

	void AssetsBrowserEditContext::Duplicate()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		for (auto Id : Instances)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
			Browser->Duplicate(Instance->GetTargetPath(), Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Delete()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		for (auto Id : Instances)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
			Browser->Delete(Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Cut()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		Clipboard.Clear();
		Clipboard.AppendRange(Instances);
		IsCutting = true;
	}

	void AssetsBrowserEditContext::Copy()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		Clipboard.Clear();
		Clipboard.AppendRange(Instances);
		IsCutting = false;
	}

	void AssetsBrowserEditContext::Paste()
	{
		AssetsBrowserItem* Parent = Browser->GetItem(Selected);
		NxFr::String Root = Parent->IsDirectory() ? Parent->GetTargetPath() : Parent->GetDirectory();

		for (auto Id : Clipboard)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(Id);
			Browser->Duplicate(Instance->GetTargetPath(), NxFr::Path::Combine(Root, Instance->GetTargetName()));
		}

		if (IsCutting)
		{
			for (auto Id : Clipboard)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(Id);
				Browser->Delete(Instance->GetTargetPath());
			}

			Clipboard.Clear();
			IsCutting = false;
		}
	}

	NxFr::Set<NxFr::GUID> AssetsBrowserEditContext::FilterSelection()
	{
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

		return Result;
	}

	void AssetsBrowserEditContext::OnSelectItem(NxFr::GUID Id, bool State)
	{
		AssetsBrowserItem* Instance = Browser->GetItem(Id);
		Browser->SelectItem(Instance, State, GetId());
	}
}
