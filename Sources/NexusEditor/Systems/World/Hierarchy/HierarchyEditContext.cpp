#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	HierarchyEditContext::HierarchyEditContext(HierarchyManager* Manager, NxFr::StringId Id)
		: Edit::Context(Id), Manager(Manager)
	{
	}

	HierarchyEditContext::~HierarchyEditContext()
	{
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::FilterSelection(HierarchyFilter Mode, NxFr::GUID* Active) const
	{
		const bool Unfiltered = Mode == HierarchyFilter::Unfiltered;
		const bool MultiSelection = NxFr::Enum::CheckFlag(Mode, HierarchyFilter::MultiSelection);
		const bool Recursive = NxFr::Enum::CheckFlag(Mode, HierarchyFilter::Recursive);
		const bool TopMost = NxFr::Enum::CheckFlag(Mode, HierarchyFilter::TopMost);
		const bool IgnoreSelected = NxFr::Enum::CheckFlag(Mode, HierarchyFilter::IgnoreSelected);
		const bool Sorted = NxFr::Enum::CheckFlag(Mode, HierarchyFilter::Sorted);

		NX_ASSERT(!(TopMost && Recursive), Default, "Can't filter TopMost and Recursive at the same time");

		if (Active)
		{
			*Active = Selected;
		}

		if (Unfiltered)
		{
			return FilterSelection();
		}

		NxFr::Set<NxFr::GUID> Filtered = Selection.GetCapacity();
		NxFr::Set<NxFr::GUID> Roots = MultiSelection ? Selection : NxFr::Set<NxFr::GUID>({ Selected });

		if (TopMost)
		{
			for (auto InstanceId : Roots)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(InstanceId)->GetTarget();
				if (!Instance)
				{
					continue;
				}

				NxFr::Handle<NxEn::GameObject> Parent = Instance->GetParent();
				bool ParentSelected = false;

				while (Parent)
				{
					if (IsSelected(Parent->GetId()) && !(IgnoreSelected && Parent->GetId() == Selected))
					{
						ParentSelected = true;
						break;
					}

					Parent = Parent->GetParent();
				}

				if (!ParentSelected)
				{
					Filtered.Append(InstanceId);
				}
			}
		}
		else if (Recursive)
		{
			NxFr::Delegate<void(NxFr::GUID)> Traverse = [&](NxFr::GUID Id)
			{
				Filtered.Append(Id);

				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
				if (!Instance)
				{
					return;
				}

				for (NxFr::Handle<NxEn::GameObject> Child = Instance->GetChild();
					Child;
					Child = Child->GetNext())
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

		NxFr::Array Result = NxFr::ContainerUtility::ToArray<NxFr::GUID>(Filtered);

		if (Sorted)
		{
			NxFr::ContainerUtility::Sort<NxFr::GUID>(Result, [&](const NxFr::GUID& A, const NxFr::GUID& B)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(A)->GetTarget();
				if (!Instance)
				{
					return false;
				}
				NxFr::Handle<NxEn::GameObject> Target = Manager->GetItem(B)->GetTarget();
				if (!Target)
				{
					return true;
				}

				return Instance->GetOrderIndex() < Target->GetOrderIndex();
			});
		}

		return Result;
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetAll()
	{
		NxFr::List<NxFr::GUID> Result;
		NxFr::Handle<NxEn::GameObject> Root = NxEn::Application::GetSystem<NxEn::WorldSystem>()->GetWorld(Id)->GetRoot();

		Result.Append(Root->GetId());
		for (auto It = Root->BeginChild(); It != Root->EndChild(); ++It)
		{
			Result.Append(It->GetId());
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	uint64 HierarchyEditContext::GetCount()
	{
		NxFr::Handle<NxEn::GameObject> Root = NxEn::Application::GetSystem<NxEn::WorldSystem>()->GetWorld(Id)->GetRoot();
		return Root->GetChildCount(true) + 1;
	}

	void HierarchyEditContext::Create()
	{
		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([&](NxFr::StringView Input)
		{
			NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

			NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection);
			for (auto& InstanceId : InstanceIds)
			{
				NxFr::Handle<NxEn::GameObject> Parent = Manager->GetItem(InstanceId)->GetTarget();
				System->CreateGameObject(Input, Parent, Parent->GetWorld()->GetId());
			}
		});
	}

	void HierarchyEditContext::Rename()
	{
		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([&](NxFr::StringView Input)
		{
			NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection);
			for (auto& InstanceId : InstanceIds)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(InstanceId)->GetTarget();
				Instance->SetName(Input);
			}
		});
	}

	void HierarchyEditContext::Move()
	{
		if (Selected == 0 || Selection.GetCount() <= 1)
		{
			NX_LOG(Warning, System, "Can only move GameObject if a target is selected");
			return;
		}

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::GameObject> Parent = static_cast<HierarchyItem*>(Manager->GetItem(Selected))->GetTarget();

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost);
		for (auto InstanceId : InstanceIds)
		{
			if (InstanceId == Parent->GetId())
			{
				continue;
			}

			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Manager->GetItem(InstanceId))->GetTarget();
			System->AttachGameObject(Instance, Parent);
		}
	}

	void HierarchyEditContext::Duplicate()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost);
		for (auto& InstanceId : InstanceIds)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(InstanceId)->GetTarget();
			System->DuplicateGameObject(Instance);
		}
	}

	void HierarchyEditContext::Delete()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost);
		for (auto& InstanceId : InstanceIds)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(InstanceId)->GetTarget();
			System->DestroyGameObject(Instance);
		}
	}

	void HierarchyEditContext::Cut()
	{
		Clipboard.Clear();
		Clipboard.AppendRange(FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost));
		IsCutting = true;
	}

	void HierarchyEditContext::Copy()
	{
		Clipboard.Clear();
		Clipboard.AppendRange(FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost));
		IsCutting = false;
	}

	void HierarchyEditContext::Paste()
	{
		if (Selected == 0)
		{
			NX_LOG(Warning, System, "Can only move GameObject if a target is selected");
			return;
		}

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::GameObject> Target = Manager->GetItem(Selected)->GetTarget();

		for (auto InstanceId : Clipboard)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(InstanceId)->GetTarget();
			if (!Instance)
			{
				continue;
			}

			System->DuplicateGameObject(Instance, Target);
		}

		if (IsCutting)
		{
			for (auto InstanceId : Clipboard)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(InstanceId)->GetTarget();
				if (!Instance)
				{
					continue;
				}

				System->DestroyGameObject(Instance);
			}
		}

		if (IsCutting)
		{
			Clipboard.Clear();
			IsCutting = false;
		}
	}

	void HierarchyEditContext::OnSelectionChanged(NxFr::GUID InstanceId, bool State) const
	{
		Manager->SelectItem(Manager->GetItem(InstanceId)->GetTarget(), State);
	}
}
