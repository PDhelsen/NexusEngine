#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyEditContext::HierarchyEditContext(NxFr::StringId Id, HierarchyPanel* Hierarchy)
		: Edit::Context(Id), Hierarchy(Hierarchy), World(nullptr)
	{
		World = NxEn::Application::GetSystem<NxEn::WorldSystem>()->CreateWorld(Id);
	}

	HierarchyEditContext::~HierarchyEditContext()
	{
		NxEn::Application::GetSystem<NxEn::WorldSystem>()->DestroyWorld(Id);
	}

	NxFr::Array<NxFr::GUID> HierarchyEditContext::GetAll()
	{
		HierarchyItem* Item = static_cast<HierarchyItem*>(Hierarchy->Root);

		NxFr::List<NxFr::GUID> Result;

		NxFr::Handle<NxEn::GameObject> Iterator = Item->GetTarget();
		while (Iterator)
		{
			Result.Append(Iterator->GetId());
			Iterator = Iterator->GetIterator();
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	uint64 HierarchyEditContext::GetCount()
	{
		HierarchyItem* Item = static_cast<HierarchyItem*>(Hierarchy->Root);
		return Item->GetTarget()->GetChildCount() + 1;
	}

	void HierarchyEditContext::Rename()
	{
		Hierarchy->RunAction<HierarchyActionRename>();
	}

	void HierarchyEditContext::Duplicate()
	{
		Hierarchy->RunAction<HierarchyActionDuplicate>();
	}

	void HierarchyEditContext::Delete()
	{
		Hierarchy->RunAction<HierarchyActionDelete>();
	}

	void HierarchyEditContext::Cut()
	{
		ClearClipboard();
		CopySelection();
		DestroySelection();
	}

	void HierarchyEditContext::Copy()
	{
		ClearClipboard();
		CopySelection();
	}

	void HierarchyEditContext::Paste()
	{
		PasteClipboard();
	}

	NxFr::Set<NxFr::GUID> HierarchyEditContext::FilterSelection()
	{
		NxFr::Set<NxFr::GUID> Result = Selection.GetCapacity();

		for (auto Id : Selection)
		{
			NxFr::Handle<NxEn::GameObject> Target = Hierarchy->Manager->Items[Id]->GetTarget();
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

	void HierarchyEditContext::CopySelection()
	{
		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		for (auto Id : Instances)
		{
			NxFr::Handle<NxEn::GameObject> Target = Hierarchy->Manager->Items[Id]->GetTarget();
			NxFr::Handle<NxEn::GameObject> Instance = World->DuplicateGameObject(Target, World->GetRootGameObject());
			Clipboard.Append(Instance->GetId());
		}
	}

	void HierarchyEditContext::DestroySelection()
	{
		NxEn::World* Instance = Hierarchy->Manager->Items[Selected]->GetTarget()->GetWorld();

		NxFr::Set<NxFr::GUID> Instances = FilterSelection();
		for (auto Id : Instances)
		{
			NxFr::Handle<NxEn::GameObject> Target = Instance->GetGameObject(Id);
			Instance->DestroyGameObject(Target);
		}
	}

	void HierarchyEditContext::PasteClipboard()
	{
		NxFr::Handle<NxEn::GameObject> Parent = Hierarchy->Manager->Items[Selected]->GetTarget();
		NxEn::World* Instance = Parent->GetWorld();

		for (auto Id : Clipboard)
		{
			NxFr::Handle<NxEn::GameObject> Target = World->GetGameObject(Id);
			Instance->DuplicateGameObject(Target, Parent);
		}
	}

	void HierarchyEditContext::ClearClipboard()
	{
		for (auto Id : Clipboard)
		{
			NxFr::Handle<NxEn::GameObject> Target = World->GetGameObject(Id);
			World->DestroyGameObject(Target);
		}

		Clipboard.Clear();
	}
}
