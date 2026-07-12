#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyEditContext::HierarchyEditContext()
		: Edit::Context(ContextId), Manager(nullptr), IsCutting(false)
	{
		OnSelectionChanged += { this, & HierarchyEditContext::OnSelectItem };

	}

	HierarchyEditContext::~HierarchyEditContext()
	{
		OnSelectionChanged -= { this, & HierarchyEditContext::OnSelectItem };
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetAll()
	{
		return NxFr::ContainerUtility::ToArrayKeys(Manager->Items);
	}

	uint64 HierarchyEditContext::GetCount()
	{
		return Manager->Items.GetCount();;
	}

	void HierarchyEditContext::Rename()
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Set<NxFr::GUID> Ids = FilterSelection();
			for (auto& Id : Ids)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
				Instance->SetName(Input);
			}
		});
	}

	void HierarchyEditContext::Duplicate()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Set<NxFr::GUID> Ids = FilterSelection();
		for (auto& Id : Ids)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
			System->DuplicateGameObject(Instance);
		}
	}

	void HierarchyEditContext::Delete()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Set<NxFr::GUID> Ids = FilterSelection();
		for (auto& Id : Ids)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
			System->DestroyGameObject(Instance);
		}
	}

	void HierarchyEditContext::Cut()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		Clipboard.Clear();
		Clipboard.AppendRange(Instances);
		IsCutting = true;
	}

	void HierarchyEditContext::Copy()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		Clipboard.Clear();
		Clipboard.AppendRange(Instances);
		IsCutting = false;
	}

	void HierarchyEditContext::Paste()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::GameObject> Target = Manager->GetItem(Selected)->GetTarget();

		for (auto Id : Clipboard)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
			System->DuplicateGameObject(Instance, Target);
		}

		if (IsCutting)
		{
			for (auto Id : Clipboard)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
				System->DestroyGameObject(Instance);
			}

			Clipboard.Clear();
			IsCutting = false;
		}
	}

	NxFr::Set<NxFr::GUID> HierarchyEditContext::FilterSelection()
	{
		NxFr::Set<NxFr::GUID> Result = Selection.GetCapacity();

		for (auto Id : Selection)
		{
			NxFr::Handle<NxEn::GameObject> Target = Manager->Items[Id]->GetTarget();
			NxFr::Handle<NxEn::GameObject> Parent = Target->GetParent();
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

	void HierarchyEditContext::OnSelectItem(NxFr::GUID Id, bool State)
	{
		Manager->SelectItem(Manager->GetItem(Id)->GetTarget(), State, GetId());
	}
}
