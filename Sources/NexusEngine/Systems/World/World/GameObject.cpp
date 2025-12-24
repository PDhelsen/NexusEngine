#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GameObject)

	GameObject::GameObject(NxFr::GUID WorldId)
		: WorldId(WorldId), GameObjectId(0), ReferenceId(0),
		Parent(), Prev(), Next(), Child(),
		Name("")
	{
		SetTickable(true);
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Initialize()
	{
		Object::Initialize();

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Initialize();
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::Shutdown()
	{
		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Shutdown();
			Iterator = Iterator->GetNext();
		}

		Object::Shutdown();
	}

	void GameObject::Tick(float TimeStep)
	{
		if (!IsEnabledInHierarchy())
		{
			return;
		}

		Object::Tick(TimeStep);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Tick(TimeStep);
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::SetEnabled(bool Enabled)
	{
		if (IsEnabled() == Enabled)
		{
			return;
		}

		Object::SetEnabled(Enabled);

		if (!UpdateEnabledInHierarchy())
		{
			return;
		}

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->UpdateEnabledInHierarchy();
			Iterator = Iterator->GetNext();
		}
	}

	bool GameObject::IsEnabledInHierarchy() const
	{
		return GetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy);
	}

	Object* GameObject::Clone() const
	{
		GameObject* Instance = new GameObject(WorldId);
		Instance->Clone(const_cast<GameObject*>(this));
		return Instance;
	}

	void GameObject::Clone(Object* Target)
	{
		GameObject* Instance = static_cast<GameObject*>(Target);

		Instance->ReferenceId = ReferenceId;

		Instance->Name = Name;

		Instance->SetFlag(ObjectFlags::Enabled, IsEnabled());
		Instance->SetFlag(ObjectFlags::Tickable, IsTickable());
		Instance->SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, false);
	}

	void GameObject::Clone(const Object* Target)
	{
		const GameObject* Instance = static_cast<const GameObject*>(Target);

		ReferenceId = Instance->ReferenceId;

		Name = Instance->Name;

		SetFlag(ObjectFlags::Enabled, Instance->IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance->IsTickable());
		SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, false);
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
			NEXUS_LOG(Error, System, "Failed to find requested child on GameObject %s", Name.C());
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
			NEXUS_LOG(Error, System, "Failed to find requested child on GameObject %s", Name.C());
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

		NxFr::Handle<GameObject> Iterator = GetWorld()->GetRootGameObject();
		while (Iterator && Iterator.GetRedirectedPointer() != this)
		{
			Order++;
			Iterator = Iterator->GetIterator();
		}

		return Order;
	}

	void GameObject::OnSave(YAML::Node& Node)
	{
		Node["Name"] = Name;
		Node["Id"] = GameObjectId;
		Node["Reference"] = ReferenceId;
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);
		Node["Tickable"] = GetFlag(ObjectFlags::Tickable);
	}

	void GameObject::OnLoad(const YAML::Node& Node)
	{
		Name = Node["Name"].as<NxFr::String>();
		NEXUS_ASSERT(GameObjectId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		ReferenceId = Node["Reference"].as<NxFr::GUID>();
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());
		SetFlag(ObjectFlags::Tickable, Node["Tickable"].as<bool>());
		SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, false);
	}

	void GameObject::OnUnload()
	{
	}

	bool GameObject::UpdateEnabledInHierarchy()
	{
		bool Enabled = IsEnabled() && (Parent ? Parent->IsEnabledInHierarchy() : true);
		if (Enabled == IsEnabledInHierarchy())
		{
			return false;
		}

		SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, Enabled);

		if (Enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}

		return true;
	}

	YAML::Node GameObject::Save()
	{
		YAML::Node Node;

		YAML::Node Instance;
		OnSave(Instance);
		Node["Instance"] = Instance;

		YAML::Node Children;
		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			if (Iterator->GetReferenceId())
			{
				YAML::Node ChildNode;

				YAML::Node ChildInstance;
				Iterator->OnSave(ChildInstance);
				ChildNode["Instance"] = ChildInstance;
				ChildNode["Children"] = YAML::Node();

				Children.push_back(ChildNode);

			}
			else
			{
				Children.push_back(Iterator->Save());
			}

			Iterator = Iterator->GetNext();
		}
		Node["Children"] = Children;

		return Node;
	}

	void GameObject::Load(const YAML::Node& Node)
	{
		ObjectFactory& Factory = ObjectFactory::GetFactory();
		NxFr::Handle<GameObject> This = Factory.GetGameObject(GameObjectId);

		YAML::Node Instance = Node["Instance"];
		OnLoad(Instance);

		NxFr::Delegate<NxFr::Handle<GameObject>(const YAML::Node&)> ChildLoad = [&](const YAML::Node& ChildNode)
		{
			NxFr::Handle<GameObject> ChildInstance;
			NxFr::GUID ChildReference = ReadReferenceFromYaml(ChildNode);

			if (ChildReference)
			{
				Prefab* PrefabInstance = Application::GetSystem<AssetsSystem>()->Acquire<Prefab>(ChildReference);
				ChildInstance = Factory.Instantiate(*PrefabInstance, This);
			}
			else
			{
				ChildInstance = Factory.CreateGameObject("", This, ReadIdFromYaml(ChildNode));
				ChildInstance->Load(ChildNode);
			}

			return ChildInstance;
		};

		YAML::Node Children = Node["Children"];
		if (Children.size() > 0)
		{
			Child = ChildLoad.Invoke(Children[0]);

			NxFr::Handle<GameObject> Iterator = GetChild();
			for (uint64 Index = 1; Index < Children.size(); ++Index)
			{
				Iterator->Next = ChildLoad.Invoke(Children[Index]);
				Iterator = Iterator->GetNext();
			}
		}
	}

	void GameObject::Unload()
	{
		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			if (Iterator->GetReferenceId())
			{
				Application::GetSystem<AssetsSystem>()->Release(Iterator->GetReferenceId());
			}
			else
			{
				Iterator->Unload();
			}

			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::GatherDependencies(NxFr::Set<NxFr::GUID>& Result)
	{
		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			if (Iterator->GetReferenceId())
			{
				Result.Append(Iterator->GetReferenceId());
			}
			else
			{
				Iterator->GatherDependencies(Result);
			}

			Iterator = Iterator->GetNext();
		}
	}
}
