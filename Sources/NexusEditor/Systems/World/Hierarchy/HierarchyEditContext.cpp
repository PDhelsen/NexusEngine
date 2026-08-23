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

			NxFr::Array<NxFr::GUID> Ids = GetSelection();
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
			NxFr::Array<NxFr::GUID> Ids = GetSelection();
			for (auto& Id : Ids)
			{
				NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
				Instance->SetName(Input);
			}
		});
	}

	void HierarchyEditContext::Move()
	{
		if (Selected == 0)
		{
			return;
		}

		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::GameObject> Parent = static_cast<HierarchyItem*>(Manager->GetItem(Selected))->GetTarget();
		NxEn::World* World = Parent->GetWorld();

		NxFr::Array<NxFr::GUID> InstanceIds = GetSelection();
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
		NxFr::Array<NxFr::GUID> Ids = GetSelection();
		for (auto& Id : Ids)
		{
			NxFr::Handle<NxEn::GameObject> Instance = Manager->GetItem(Id)->GetTarget();
			System->DuplicateGameObject(Instance);
		}
	}

	void HierarchyEditContext::Delete()
	{
		NxEn::WorldSystem* System = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Array<NxFr::GUID> Ids = GetSelection();
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
