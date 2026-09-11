#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	HierarchyEditContext::HierarchyEditContext(HierarchyManager* Manager, HierarchyPanel* Panel, NxFr::StringId Id)
		: Edit::Context(Id), Manager(Manager), Panel(Panel)
	{
		if (Manager)
		{
			Manager->Contexts.Append(this);
		}

		OnSelection += [this](NxFr::GUID InstanceId, bool State) { this->Panel->Select(InstanceId, State, true); };
	}

	HierarchyEditContext::~HierarchyEditContext()
	{
		if (Manager)
		{
			Manager->Contexts.Remove(this);
		}
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::FilterSelection(HierarchyFilter Mode, NxFr::GUID* Active) const
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

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
			return GetSelection();
		}

		NxFr::Set<NxFr::GUID> Filtered = Selection.GetCapacity();
		NxFr::Set<NxFr::GUID> Roots = MultiSelection ? Selection : NxFr::Set<NxFr::GUID>({ Selected });

		if (TopMost)
		{
			for (auto InstanceId : Roots)
			{
				HierarchyItem* Instance = Manager->GetItem(InstanceId);
				if (!Instance)
				{
					continue;
				}

				HierarchyItem* Parent = Manager->GetItem(Instance->GetParent());
				bool ParentSelected = false;

				while (Parent)
				{
					if (IsSelected(Parent->GetId()) && !(IgnoreSelected && Parent->GetId() == Selected))
					{
						ParentSelected = true;
						break;
					}

					Parent = Manager->GetItem(Parent->GetParent());
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

				HierarchyItem* Instance = Manager->GetItem(InstanceId);
				if (!Instance)
				{
					return;
				}

				for (HierarchyItem* Child = Manager->GetItem(Instance->GetChild());
					Child;
					Child = Manager->GetItem(Child->GetNext()))
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
				HierarchyItem* Instance = Manager->GetItem(A);
				if (!Instance)
				{
					return false;
				}
				HierarchyItem* Target = Manager->GetItem(B);
				if (!Target)
				{
					return true;
				}

				return Instance->Compare(*Target);
			});
		}

		return Result;
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetAll()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		HierarchyItem* Instance = Manager->GetItem(Panel->GetRoot());
		if (!Instance)
		{
			return NxFr::Array<NxFr::GUID>();
		}

		NxFr::Handle<NxEn::GameObject> Root = Instance->GetTarget();

		NxFr::List<NxFr::GUID> Result;
		Result.Append(Root->GetId());
		for (auto It = Root->BeginChild(); It != Root->EndChild(); ++It)
		{
			Result.Append(It->GetId());
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	uint64 HierarchyEditContext::GetCount()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		HierarchyItem* Instance = Manager->GetItem(Panel->GetRoot());
		if (!Instance)
		{
			return 0;
		}

		NxFr::Handle<NxEn::GameObject> Root = Instance->GetTarget();
		return Root->GetChildCount(true) + 1;
	}

	void HierarchyEditContext::Create()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([&](NxFr::StringView Input)
		{
			NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

			NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection);
			for (auto& InstanceId : InstanceIds)
			{
				HierarchyItem* Parent = Manager->GetItem(InstanceId);
				System->CreateGameObject(Input, Parent->GetTarget());
			}
		});
	}

	void HierarchyEditContext::Rename()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([&](NxFr::StringView Input)
		{
			NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection);
			for (auto& InstanceId : InstanceIds)
			{
				HierarchyItem* Instance = Manager->GetItem(InstanceId);
				Instance->GetTarget()->SetName(Input);
			}
		});
	}

	void HierarchyEditContext::Move()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		if (Selected == 0 || Selection.GetCount() <= 1)
		{
			NX_LOG(Warning, Default, "Can only move GameObject if a target is selected");
			return;
		}

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		HierarchyItem* Parent = Manager->GetItem(Selected);

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost | HierarchyFilter::IgnoreSelected);
		for (auto InstanceId : InstanceIds)
		{
			HierarchyItem* Instance = Manager->GetItem(InstanceId);
			System->AttachGameObject(Instance->GetTarget(), Parent->GetTarget());
		}
	}

	void HierarchyEditContext::Duplicate()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost);
		for (auto& InstanceId : InstanceIds)
		{
			HierarchyItem* Instance = Manager->GetItem(InstanceId);
			System->DuplicateGameObject(Instance->GetTarget());
		}
	}

	void HierarchyEditContext::Delete()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

		NxFr::Array<NxFr::GUID> InstanceIds = FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost);
		for (auto& InstanceId : InstanceIds)
		{
			HierarchyItem* Instance = Manager->GetItem(InstanceId);
			System->DestroyGameObject(Instance->GetTarget());
		}
	}

	void HierarchyEditContext::Cut()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		Clipboard.Clear();
		Clipboard.AppendRange(FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost));
		IsCutting = true;
	}

	void HierarchyEditContext::Copy()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		Clipboard.Clear();
		Clipboard.AppendRange(FilterSelection(HierarchyFilter::MultiSelection | HierarchyFilter::TopMost));
		IsCutting = false;
	}

	void HierarchyEditContext::Paste()
	{
		NX_ASSERT(Manager, Default, "Invalid Manager");

		if (Selected == 0)
		{
			NX_LOG(Warning, Default, "Can only move GameObject if a target is selected");
			return;
		}

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		HierarchyItem* Parent = Manager->GetItem(Selected);

		for (auto InstanceId : Clipboard)
		{
			HierarchyItem* Instance = Manager->GetItem(InstanceId);
			if (!Instance)
			{
				continue;
			}

			System->DuplicateGameObject(Instance->GetTarget(), Parent->GetTarget());
		}

		if (IsCutting)
		{
			for (auto InstanceId : Clipboard)
			{
				HierarchyItem* Instance = Manager->GetItem(InstanceId);
				if (!Instance)
				{
					continue;
				}

				System->DestroyGameObject(Instance->GetTarget());
			}
		}

		if (IsCutting)
		{
			Clipboard.Clear();
			IsCutting = false;
		}
	}
}
