#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	NX_WORLD_OBJECT(GameObject)

	GameObject::GameObject()
		: GameObjectId(0), WorldId(0), Name(),
		Parent(), Prev(), Next(), Child()
	{
		SetTickable(false);
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Initialize()
	{
		if (IsInitialized())
		{
			return;
		}

		OnInitialize();
		SetFlag(ObjectFlags::Initialized, true);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Initialize();
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::Shutdown()
	{
		if (!IsInitialized())
		{
			return;
		}

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Shutdown();
			Iterator = Iterator->GetNext();
		}

		OnShutdown();
		SetFlag(ObjectFlags::Initialized, false);
	}

	void GameObject::UpdateHierarchy()
	{
		OnUpdateHierarchy();

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->UpdateHierarchy();
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::Tick(float TimeStep)
	{
		if (!IsTicking())
		{
			return;
		}

		OnTick(TimeStep);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Tick(TimeStep);
			Iterator = Iterator->GetNext();
		}
	}

	NxFr::GUID GameObject::GetId() const
	{
		return GetGameObjectId();
	};

	NxFr::GUID GameObject::GetWorldId() const
	{
		return WorldId;
	};

	NxFr::GUID GameObject::GetGameObjectId() const
	{
		return GameObjectId;
	};

	NxFr::StringView GameObject::GetName() const
	{
		return Name;
	};

	void GameObject::SetName(NxFr::StringView Name)
	{
		this->Name = Name;
	};

	bool GameObject::IsRoot() const
	{
		return !Parent && !Prev && !Next;
	}

	World* GameObject::GetWorld() const
	{
		return Application::GetSystem<WorldSystem>()->GetWorld(WorldId);
	}

	NxFr::Handle<GameObject> GameObject::GetParent() const
	{
		return Parent;
	}

	NxFr::Handle<GameObject> GameObject::GetPrevious() const
	{
		return Prev;
	}

	NxFr::Handle<GameObject> GameObject::GetNext() const
	{
		return Next;
	}

	NxFr::Handle<GameObject> GameObject::GetSibling() const
	{
		return Parent->Child;
	}

	NxFr::Handle<GameObject> GameObject::GetChild() const
	{
		return Child;
	}

	NxFr::Handle<GameObject> GameObject::GetSibling(uint64 Index) const
	{
		NxFr::Handle<GameObject> Target = GetSibling();

		while (Target && Index > 0)
		{
			Index--;
			Target = Target->Next;
		}

		if (!Target || Index > 0)
		{
			NX_LOG(Error, System, "Failed to find requested child on GameObject %s", Name.C());
			return NxFr::Handle<GameObject>();
		}

		return Target;
	}

	NxFr::Handle<GameObject> GameObject::GetChild(uint64 Index) const
	{
		NxFr::Handle<GameObject> Target = GetChild();

		while (Target && Index > 0)
		{
			Index--;
			Target = Target->Next;
		}

		if (!Target || Index > 0)
		{
			NX_LOG(Error, System, "Failed to find requested child on GameObject %s", Name.C());
			return NxFr::Handle<GameObject>();
		}

		return Target;
	}

	uint64 GameObject::GetSiblingCount() const
	{
		uint64 Count = 0;

		NxFr::Handle<GameObject> Instance = GetSibling();
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

		NxFr::Handle<GameObject> Instance = GetChild();
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

	NxFr::Handle<GameObject> GameObject::GetIterator() const
	{
		if (Child)
		{
			return Child;
		}

		if (Next)
		{
			return Next;
		}

		NxFr::Handle<GameObject> Iterator = Parent;
		while (Iterator && !Iterator->Next)
		{
			Iterator = Iterator->Parent;
		}

		if (Iterator)
		{
			return Iterator->Next;
		}

		return NxFr::Handle<GameObject>();
	}

	uint64 GameObject::GetOrderIndex() const
	{
		uint64 Order = 0;

		NxFr::Handle<GameObject> Iterator = GetWorld()->GetRoot();
		while (Iterator && Iterator.GetRedirectedPointer() != this)
		{
			Order++;
			Iterator = Iterator->GetIterator();
		}

		return Order;
	}

	void GameObject::OnUpdateHierarchy()
	{
		bool Enabled = IsEnabled() && (Parent ? Parent->IsEnabledInHierarchy() : true);
		if (Enabled == IsEnabledInHierarchy())
		{
			return;
		}

		SetFlag(ObjectFlags::EnabledInHierarchy, Enabled);

		if (Enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	void GameObject::OnClone(const Object& Other)
	{
		const GameObject& Instance = static_cast<const GameObject&>(Other);

		Name = Instance.Name;

		SetFlag(ObjectFlags::Enabled, Instance.IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance.IsTickable());
		SetFlag(ObjectFlags::EnabledInHierarchy, false);
	}
}
