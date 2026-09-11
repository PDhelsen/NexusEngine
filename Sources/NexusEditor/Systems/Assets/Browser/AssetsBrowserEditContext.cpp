#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	AssetsBrowserEditContext::AssetsBrowserEditContext(AssetsBrowser* Browser, AssetsBrowserPanel* Panel)
		: Edit::Context(ContextId), Browser(Browser), Panel(Panel)
	{
		OnSelection += [this](NxFr::GUID InstanceId, bool State) { this->Panel->Select(InstanceId, State, true); };
	}

	AssetsBrowserEditContext::~AssetsBrowserEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::FilterSelection(AssetsBrowserFilter Mode, NxFr::GUID* Active) const
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		const bool Unfiltered = Mode == AssetsBrowserFilter::Unfiltered;
		const bool NoDirectory = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::NoDirectory);
		const bool NoAssets = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::NoAssets);
		const bool NoContent = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::NoContent);
		const bool MultiSelection = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::MultiSelection);
		const bool Recursive = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::Recursive);
		const bool TopMost = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::TopMost);
		const bool IgnoreSelected = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::IgnoreSelected);
		const bool Sorted = NxFr::Enum::CheckFlag(Mode, AssetsBrowserFilter::Sorted);

		NX_ASSERT(!(TopMost && Recursive), Default, "Can't filter TopMost and Recursive at the same time");

		if (Active)
		{
			*Active = Selected;
		}

		if (Unfiltered)
		{
			return GetSelection();
		}

		NxFr::Set<NxFr::GUID> Filtered = Selection.GetCapacity();
		NxFr::Set<NxFr::GUID> Roots = MultiSelection ? Selection : NxFr::Set<NxFr::GUID>({ Selected });

		if (TopMost)
		{
			for (auto InstanceId : Roots)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				if (!Instance)
				{
					continue;
				}

				AssetsBrowserItem* Parent = Browser->GetItem(Instance->GetParent());
				bool ParentSelected = false;

				while (Parent)
				{
					if (IsSelected(Parent->GetId()) && !(IgnoreSelected && Parent->GetId() == Selected))
					{
						ParentSelected = true;
						break;
					}

					Parent = Browser->GetItem(Parent->GetParent());
				}

				if (!ParentSelected)
				{
					Filtered.Append(InstanceId);
				}
			}
		}
		else if (Recursive)
		{
			NxFr::Delegate<void(NxFr::GUID)> Traverse = [&](NxFr::GUID InstanceId)
			{
				Filtered.Append(InstanceId);

				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				if (!Instance)
				{
					return;
				}

				for (AssetsBrowserItem* Child = Browser->GetItem(Instance->GetChild());
					Child;
					Child = Browser->GetItem(Child->GetNext()))
				{
					Traverse(Child->GetId());
				}
			};

			for (auto InstanceId : Roots)
			{
				Traverse(InstanceId);
			}
		}
		else
		{
			Filtered = Roots;
		}

		if (IgnoreSelected)
		{
			Filtered.Remove(Selected);
		}

		if (NoDirectory || NoAssets || NoContent)
		{
			NxFr::Set<NxFr::GUID> ToRemove;
			for (auto InstanceId : Filtered)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				if (!Instance)
				{
					continue;
				}

				if (NoDirectory && Instance->GetObjectType() == AssetsBrowserItemDirectory::GetClassType())
				{
					ToRemove.Append(InstanceId);
				}
				else if (NoAssets && Instance->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
				{
					ToRemove.Append(InstanceId);
				}
				else if (NoContent && Instance->GetObjectType() == AssetsBrowserItemContent::GetClassType())
				{
					ToRemove.Append(InstanceId);
				}
			}

			Filtered.TryRemoveRange(ToRemove);
		}

		NxFr::Array Result = NxFr::ContainerUtility::ToArray<NxFr::GUID>(Filtered);

		if (Sorted)
		{
			NxFr::ContainerUtility::Sort<NxFr::GUID>(Result, [&](const NxFr::GUID& A, const NxFr::GUID& B)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(A);
				if (!Instance)
				{
					return false;
				}
				AssetsBrowserItem* Target = Browser->GetItem(B);
				if (!Target)
				{
					return true;
				}

				return Instance->Compare(*Target);
			});
		}

		return Result;
	}

	NxFr::Array<NxFr::GUID> AssetsBrowserEditContext::GetAll()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		return NxFr::ContainerUtility::ToArrayKeys(Browser->Items);
	}

	uint64 AssetsBrowserEditContext::GetCount()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		return Browser->Items.GetCount();
	}

	void AssetsBrowserEditContext::Create()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([&](NxFr::StringView Input)
		{
			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);

			NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(AssetsBrowserFilter::DirectoryOnly | AssetsBrowserFilter::MultiSelection);
			for (auto InstanceId : InstanceIds)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				NxFr::String Path = NxFr::Path::Combine(Instance->GetTargetPath(), Name);
				Browser->Create(Type, Path);
			}
		});
	}

	void AssetsBrowserEditContext::Rename()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([&](NxFr::StringView Input)
		{
			NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(AssetsBrowserFilter::MultiSelection);
			for (auto InstanceId : InstanceIds)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeName(Instance->GetTargetPath(), Input));
			}
		});
	}

	void AssetsBrowserEditContext::Move()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		if (Selected == 0 || Selection.GetCount() <= 1)
		{
			NX_LOG(Warning, Default, "Can only move AssetsBrowserItem if a target directory is selected");
			return;
		}

		AssetsBrowserItem* Target =  Browser->GetItem(Selected);
		if (Target->GetObjectType() != AssetsBrowserItemDirectory::GetClassType())
		{
			NX_LOG(Error, Default, "Can only move AssetsBrowserItem in directory");
			return;
		}

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(AssetsBrowserFilter::TopMost | AssetsBrowserFilter::MultiSelection | AssetsBrowserFilter::IgnoreSelected);
		for (auto InstanceId : InstanceIds)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeFolder(Instance->GetTargetPath(), Target->GetTargetPath()));
		}
	}

	void AssetsBrowserEditContext::Duplicate()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(AssetsBrowserFilter::TopMost | AssetsBrowserFilter::MultiSelection);
		for (auto InstanceId : InstanceIds)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			Browser->Duplicate(Instance->GetTargetPath(), Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Delete()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(AssetsBrowserFilter::TopMost | AssetsBrowserFilter::MultiSelection);
		for (auto InstanceId : InstanceIds)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			Browser->Delete(Instance->GetTargetPath());
		}
	}

	void AssetsBrowserEditContext::Cut()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		Clipboard.Clear();
		Clipboard.AppendRange(FilterSelection(AssetsBrowserFilter::TopMost | AssetsBrowserFilter::MultiSelection));
		IsCutting = true;
	}

	void AssetsBrowserEditContext::Copy()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		Clipboard.Clear();
		Clipboard.AppendRange(FilterSelection(AssetsBrowserFilter::TopMost | AssetsBrowserFilter::MultiSelection));
		IsCutting = false;
	}

	void AssetsBrowserEditContext::Paste()
	{
		NX_ASSERT(Browser, Default, "Invalid Browser");

		if (Selected == 0)
		{
			NX_LOG(Warning, Default, "Can only move AssetsBrowserItem if a target directory is selected");
			return;
		}

		AssetsBrowserItem* Target = Browser->GetItem(Selected);
		if (Target->GetObjectType() != AssetsBrowserItemDirectory::GetClassType())
		{
			NX_LOG(Error, Default, "Can only paste AssetsBrowserItem in directory");
			return;
		}

		for (auto InstanceId : Clipboard)
		{
			AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
			if (!Instance)
			{
				continue;
			}

			Browser->Duplicate(Instance->GetTargetPath(), NxFr::Path::ChangeFolder(Instance->GetTargetPath(), Target->GetTargetPath()));
		}

		if (IsCutting)
		{
			for (auto InstanceId : Clipboard)
			{
				AssetsBrowserItem* Instance = Browser->GetItem(InstanceId);
				if (!Instance)
				{
					continue;
				}

				Browser->Delete(Instance->GetTargetPath());
			}
		}

		if (IsCutting)
		{
			Clipboard.Clear();
			IsCutting = false;
		}
	}
}
