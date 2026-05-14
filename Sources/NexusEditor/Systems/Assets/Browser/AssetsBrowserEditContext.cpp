#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	AssetsBrowserEditContext::AssetsBrowserEditContext(NxFr::StringId Id, AssetsBrowserPanel* Assets)
		: Edit::Context(Id), Assets(Assets)
	{
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetAll()
	{
		return NxFr::ContainerUtility::ToArrayKeys(Assets->Browser->Items);
	}

	uint64 AssetsBrowserEditContext::GetCount()
	{
		return Assets->Browser->Items.GetCount();
	}

	void AssetsBrowserEditContext::Rename()
	{
		Assets->RunAction<AssetsBrowserActionRename>();
	}

	void AssetsBrowserEditContext::Duplicate()
	{
		Assets->RunAction<AssetsBrowserActionDuplicate>();
	}

	void AssetsBrowserEditContext::Delete()
	{
		Assets->RunAction<AssetsBrowserActionDelete>();
	}

	void AssetsBrowserEditContext::Cut()
	{
		ClearClipboard();
		CopySelection();
		IsCutting = true;
	}

	void AssetsBrowserEditContext::Copy()
	{
		ClearClipboard();
		CopySelection();
	}

	void AssetsBrowserEditContext::Paste()
	{
		PasteClipboard();
		if (IsCutting)
		{
			DestroyClipboard();
			ClearClipboard();
		}
	}

	NxFr::Set<NxFr::GUID> AssetsBrowserEditContext::FilterSelection()
	{
		NxFr::Set<NxFr::GUID> Result = Selection.GetCapacity();

		for (auto Id : Selection)
		{
			AssetsBrowserItem* Target = static_cast<AssetsBrowserItem*>(Assets->Browser->Items[Id]);
			AssetsBrowserItem* Parent = Target->GetParent();
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

	void AssetsBrowserEditContext::CopySelection()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		Clipboard.AppendRange(Instances);
	}

	void AssetsBrowserEditContext::DestroyClipboard()
	{
		for (auto Id : Clipboard)
		{
			AssetsBrowserItem* Target = static_cast<AssetsBrowserItem*>(Assets->Browser->Items[Id]);
			Assets->Browser->Delete(Target->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::PasteClipboard()
	{
		AssetsBrowserItem* Parent = static_cast<AssetsBrowserItem*>(Assets->Browser->Items[Selected]);
		NxFr::String Root = Parent->IsDirectory() ? Parent->GetTargetPath() : Parent->GetDirectory();

		for (auto Id : Clipboard)
		{
			AssetsBrowserItem* Target = static_cast<AssetsBrowserItem*>(Assets->Browser->Items[Id]);
			Assets->Browser->Duplicate(Target->GetTargetPath(), NxFr::Path::Combine(Root, Target->GetTargetName()));
		}
	}

	void AssetsBrowserEditContext::ClearClipboard()
	{
		Clipboard.Clear();
		IsCutting = false;
	}
}
