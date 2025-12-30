#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	static const NxFr::String YamlRoot = "GameObject";
	static const NxFr::String YamlChildren = "Children";
	static const NxFr::String YamlType = "Type";
	static const NxFr::String YamlId = "Id";
	static const NxFr::String YamlReference = "Reference";

	NEXUS_OBJECT_IMPLEMENTATION(GameObject)

	GameObject::GameObject(NxFr::GUID WorldId)
		: WorldId(WorldId), GameObjectId(0), ReferenceId(0),
		Name(""),
		Parent(), Prev(), Next(), Child(),
		Behaviours(), Components()
	{
		SetTickable(true);
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Initialize()
	{
		if (!IsInitialized())
		{
			OnInitialize();
			SetFlag(ObjectFlags::Initialized, true);
		}

		for (auto& B : Behaviours)
		{
			B->Initialize();
		}

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

		for (auto& B : Behaviours)
		{
			B->Shutdown();
		}

		if (IsInitialized())
		{
			OnShutdown();
			SetFlag(ObjectFlags::Initialized, false);
		}
	}

	void GameObject::Tick(float TimeStep)
	{
		if (IsTicking())
		{
			OnTick(TimeStep);
		}

		for (auto& B : Behaviours)
		{
			B->Tick();
		}

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Tick(TimeStep);
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::DrawGui(float TimeStep)
	{
		OnGui(TimeStep);

		ImGui::Separator();

		for (auto& B : Behaviours)
		{
			ImGui::PushID(B->GetId());
			B->DrawGui(TimeStep);
			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::Separator();

		for (auto& C : Components)
		{
			ImGui::PushID(C->GetId());
			C->DrawGui(TimeStep);
			ImGui::Separator();
			ImGui::PopID();
		}
	}

	void GameObject::SetEnabled(bool Enabled)
	{
		SetFlag(ObjectFlags::Enabled, Enabled);

		UpdateEnabledInHierarchy();
	}

	bool GameObject::IsEnabledInHierarchy() const
	{
		return GetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy);
	}

	bool GameObject::IsTicking() const
	{
		return IsEnabledInHierarchy() && IsTickable();
	}

	YAML::Node GameObject::Save()
	{
		YAML::Node Node;

		YAML::Node Instance;
		OnSave(Instance);
		Node[YamlRoot] = Instance;

		YAML::Node Children;
		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			if (Iterator->GetReferenceId())
			{
				YAML::Node ChildNode;

				YAML::Node ChildInstance;
				Iterator->OnSave(ChildInstance);
				ChildNode[YamlRoot] = ChildInstance;
				ChildNode[YamlChildren] = YAML::Node();

				Children.push_back(ChildNode);

			}
			else
			{
				Children.push_back(Iterator->Save());
			}

			Iterator = Iterator->GetNext();
		}
		Node[YamlChildren] = Children;

		return Node;
	}

	void GameObject::Load(const YAML::Node& Node)
	{
		ObjectFactory& Factory = FactoryContext::GetFactory();
		NxFr::Handle<GameObject> This = Factory.GetGameObject(GameObjectId);

		YAML::Node Instance = Node[YamlRoot];
		OnLoad(Instance);

		NxFr::Delegate<NxFr::Handle<GameObject>(const YAML::Node&)> ChildLoad = [&](const YAML::Node& ChildNode)
			{
				NxFr::Handle<GameObject> ChildInstance;
				NxFr::GUID ChildReference = ChildNode[YamlRoot][YamlReference].as<NxFr::GUID>();

				if (ChildReference)
				{
					Prefab* PrefabInstance = Application::GetSystem<WorldSystem>()->LoadPrefab(ChildReference);
					ChildInstance = Factory.DuplicateGameObject(PrefabInstance->GetRoot(), This, true);
				}
				else
				{
					ChildInstance = Factory.CreateGameObject("", This, ChildNode[YamlRoot][YamlId].as<NxFr::GUID>());
					ChildInstance->Load(ChildNode);
				}

				return ChildInstance;
			};

		YAML::Node Children = Node[YamlChildren];
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
				Application::GetSystem<AssetsSystem>()->Release(Iterator->GetReferenceId(), true);
			}
			else
			{
				Iterator->Unload();
			}

			Iterator = Iterator->GetNext();
		}
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

	NxFr::Handle<Behaviour> GameObject::GetBehaviourById(NxFr::GUID Id)
	{
		for (auto& B : Behaviours)
		{
			if (B->GetId() == Id)
			{
				return B;
			}
		}

		return NxFr::Handle<Behaviour>();
	}

	NxFr::Handle<Behaviour> GameObject::GetBehaviourByType(NxFr::StringId Id)
	{
		for (auto& B : Behaviours)
		{
			if (B->GetObjectType() == Id)
			{
				return B;
			}
		}

		return NxFr::Handle<Behaviour>();
	}

	NxFr::Handle<Component> GameObject::GetComponentById(NxFr::GUID Id)
	{
		for (auto& C : Components)
		{
			if (C->GetId() == Id)
			{
				return C;
			}
		}

		return NxFr::Handle<Component>();
	}

	NxFr::Handle<Component> GameObject::GetComponentByType(NxFr::StringId Id)
	{
		for (auto& C : Components)
		{
			if (C->GetObjectType() == Id)
			{
				return C;
			}
		}

		return NxFr::Handle<Component>();
	}

	void GameObject::OnGui(float TimeStep)
	{
		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled);
		if (Enabled != IsEnabled())
		{
			SetEnabled(Enabled);
		}

		ImGui::SameLine();

		GUI::Drawer<NxFr::String>::Field(Name, "Name");


		GUI::Drawer<NxFr::GUID>::Property(GameObjectId, "Id");
		GUI::Drawer<NxFr::GUID>::Property(ReferenceId, "Reference");

		bool Tickable = IsTickable();
		GUI::Drawer<bool>::Field(Tickable, "Tickable");
		if (Tickable != IsTickable())
		{
			SetTickable(Tickable);
		}
	}

	void GameObject::OnClone(const Object& Other)
	{
		const GameObject& Instance = static_cast<const GameObject&>(Other);

		ReferenceId = Instance.ReferenceId;
		Name = Instance.Name;

		SetFlag(ObjectFlags::Enabled, Instance.IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance.IsTickable());
		SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, false);
	}

	void GameObject::OnSave(YAML::Node& Node)
	{
		Node["Name"] = Name;
		Node["Id"] = GameObjectId;
		Node["Reference"] = ReferenceId;
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);
		Node["Tickable"] = GetFlag(ObjectFlags::Tickable);

		YAML::Node NodeBehaviours;
		for (auto& B : Behaviours)
		{
			NodeBehaviours.push_back(B->Save());
		}
		Node["Behaviours"] = NodeBehaviours;

		YAML::Node NodeComponents;
		for (auto& C : Components)
		{
			NodeComponents.push_back(C->Save());
		}
		Node["Components"] = NodeComponents;
	}

	void GameObject::OnLoad(const YAML::Node& Node)
	{
		ObjectFactory& Factory = FactoryContext::GetFactory();
		NxFr::Handle<GameObject> This = Factory.GetGameObject(GameObjectId);

		Name = Node["Name"].as<NxFr::String>();
		NEXUS_ASSERT(GameObjectId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		ReferenceId = Node["Reference"].as<NxFr::GUID>();
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());
		SetFlag(ObjectFlags::Tickable, Node["Tickable"].as<bool>());
		SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, false);

		YAML::Node NodeBehaviours = Node["Behaviours"];
		for (uint64 Index = 0; Index < NodeBehaviours.size(); ++Index)
		{
			YAML::Node NodeBehaviour = NodeBehaviours[Index];
			NxFr::StringId Type = NodeBehaviour["Type"].as<NxFr::StringId>();
			NxFr::GUID Id = NodeBehaviour["Id"].as<NxFr::GUID>();

			NxFr::Handle<Behaviour> B = Factory.CreateBehaviour(Type, This, Id);
			B->Load(NodeBehaviour);
		}

		YAML::Node NodeComponents = Node["Components"];
		for (uint64 Index = 0; Index < NodeComponents.size(); ++Index)
		{
			YAML::Node NodeComponent = NodeComponents[Index];
			NxFr::StringId Type = NodeComponent["Type"].as<NxFr::StringId>();
			NxFr::GUID Id = NodeComponent["Id"].as<NxFr::GUID>();

			NxFr::Handle<Behaviour> B = Factory.CreateComponent(Type, This, Id);
			B->Load(NodeComponent);
		}
	}

	void GameObject::OnUnload()
	{
		for(auto& B : Behaviours)
		{
			B->Unload();
		}

		for (auto& C : Components)
		{
			C->Unload();
		}
	}

	void GameObject::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids)
	{
		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			if (Iterator->GetReferenceId())
			{
				Ids.Append(Iterator->GetReferenceId());
			}
			else
			{
				Iterator->OnGetDependencies(Ids);
			}

			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::UpdateEnabledInHierarchy()
	{
		bool Enabled = IsEnabled() && (Parent ? Parent->IsEnabledInHierarchy() : true);
		if (Enabled == IsEnabledInHierarchy())
		{
			return;
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

		for (auto& B : Behaviours)
		{
			B->UpdateEnabledInHierarchy();
		}

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->UpdateEnabledInHierarchy();
			Iterator = Iterator->Next;
		}
	}

	void GameObject::PatchReferences()
	{
		for (auto& B : Behaviours)
		{
			B->PatchReferences();
		}

		for (auto& C : Components)
		{
			C->PatchReferences();
		}

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->PatchReferences();
			Iterator = Iterator->GetNext();
		}
	}

	NxFr::GUID GameObject::ReadIdFromYaml(const YAML::Node& Node)
	{
		return Node[YamlRoot][YamlId].as<NxFr::GUID>();
	}
}
