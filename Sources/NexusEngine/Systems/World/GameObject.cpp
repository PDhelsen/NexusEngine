#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GameObject)

	GameObject::GameObject(NxFr::StringId WorldId, NxFr::StringView Name)
		: Id(NxFr::Integer::GenerateGuid()), WorldId(WorldId), Name(Name),
		Parent(nullptr), Prev(nullptr), Next(nullptr), Child(nullptr)
	{
		SetTickable(true);
	}

	GameObject::~GameObject()
	{
	}

	World* GameObject::GetWorld() const
	{
		return Application::GetSystem<WorldSystem>()->GetWorld(WorldId);
	}

	GameObject* GameObject::GetParent() const
	{
		return Parent;
	}

	GameObject* GameObject::GetPrevious() const
	{
		return Prev;
	}

	GameObject* GameObject::GetNext() const
	{
		return Next;
	}

	GameObject* GameObject::GetSibling() const
	{
		return Parent->Child;
	}

	GameObject* GameObject::GetChild() const
	{
		return Child;
	}

	GameObject* GameObject::GetSibling(uint64 Index) const
	{
		GameObject* Target = GetSibling();

		while (Target && Index > 0)
		{
			Index--;
			Target = Target->Next;
		}

		if (!Target || Index > 0)
		{
			NEXUS_LOG(Error, System, "Failed to find requested child on GameObject %s", Name.C());
			return nullptr;
		}

		return Target;
	}

	GameObject* GameObject::GetChild(uint64 Index) const
	{
		GameObject* Target = GetChild();

		while (Target && Index > 0)
		{
			Index--;
			Target = Target->Next;
		}

		if (!Target || Index > 0)
		{
			NEXUS_LOG(Error, System, "Failed to find requested child on GameObject %s", Name.C());
			return nullptr;
		}

		return Target;
	}

	uint64 GameObject::GetSiblingCount() const
	{
		uint64 Count = 0;

		GameObject* Instance = GetSibling();
		while (Instance)
		{
			Count++;
			Instance = Instance->Next;
		}

		return Count;
	}

	uint64 GameObject::GetChildCount(bool Recursive) const
	{
		uint64 Count = 0;

		GameObject* Instance = GetChild();
		while (Instance)
		{
			Count++;
			if (Recursive)
			{
				Count += Instance->GetChildCount(Recursive);
			}

			Instance = Instance->Next;
		}

		return Count;
	}

	GameObject* GameObject::GetIterator() const
	{
		if (Child)
		{
			return Child;
		}

		if (Next)
		{
			return Next;
		}

		GameObject* Iterator = Parent;
		while (Iterator && !Iterator->Next)
		{
			Iterator = Iterator->Parent;
		}

		if (Iterator)
		{
			return Iterator->Next;
		}

		return nullptr;
	}

	uint64 GameObject::GetOrderIndex() const
	{
		uint64 Order = 0;

		GameObject* Iterator = GetWorld()->GetRoot();
		while (Iterator != this)
		{
			Order++;
			Iterator = Iterator->GetIterator();
		}

		return Order;
	}

	void GameObject::OnInitialize()
	{
	}

	void GameObject::OnShutdown()
	{
	}

	void GameObject::OnTick(float TimeStep)
	{
	}
}
