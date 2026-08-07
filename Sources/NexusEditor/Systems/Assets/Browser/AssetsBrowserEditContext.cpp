#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

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
		NxEn::InputTextPopup* Popup = NxEn::Application::GetSystem<NxEn::GUISystem>()->TryReuseElement<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Set<NxFr::GUID> Instances = FilterSelection();
			for (auto Id : Instances)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(Id);
				Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeName(Instance->GetTargetPath(), Input));
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
