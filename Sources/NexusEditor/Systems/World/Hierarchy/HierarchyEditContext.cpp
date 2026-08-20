#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
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
		return Root->GetChildCount(true);
	}

	void HierarchyEditContext::Create()
	{
		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();

			NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
			for (auto& Id : Ids)
			{
				NxFr::Handle<NxEn::GameObject> Parent = Manager->GetItem(Id)->GetTarget();
				System->CreateGameObject(Input, Parent, Parent->GetWorld()->GetId());
			}
		});
	}

	void HierarchyEditContext::Rename()
	{
		NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
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
		NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
		for (auto& Id : Ids)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
			System->DuplicateGameObject(Instance);
		}
	}

	void HierarchyEditContext::Delete()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Array<NxFr::GUID> Ids = GetSelection(true);
		for (auto& Id : Ids)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
			System->DestroyGameObject(Instance);
		}
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
		}

		Context::Paste();
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetSelection(bool Filtered) const
	{
		if (!Filtered)
		{
			return Context::GetSelection(Filtered);
		}

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

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	void HierarchyEditContext::OnSelectionChanged(NxFr::GUID InstanceId, bool State) const
	{
		Manager->SelectItem(Manager->GetItem(Id)->GetTarget(), State, GetId());
	}

	HierarchyEditContext::HierarchyEditContext(HierarchyManager* Manager, NxFr::StringId Id)
		: Edit::Context(Id), Manager(Manager), IsCutting(false)
	{
	}

	HierarchyEditContext::~HierarchyEditContext()
	{
	}
}
