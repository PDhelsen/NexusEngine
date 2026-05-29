#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	static const NxFr::String YamlRoot = "GameObject";
	static const NxFr::String YamlChildren = "Children";
	static const NxFr::String YamlType = "Type";
	static const NxFr::String YamlId = "Id";
	static const NxFr::String YamlReference = "Reference";

	GameObject::GameObject()
		: WorldId(0), GameObjectId(0), ReferenceId(0),
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

		for (auto& C : Components)
		{
			C->Initialize();
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

		for (auto& C : Components)
		{
			C->Shutdown();
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

		//Components are ticked by systems

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->Tick(TimeStep);
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::DrawGui(float TimeStep)
	{
		float HalfWindowSize = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * (1.0f / 2.0f);
		float ThirdWindowSize = (ImGui::GetContentRegionAvail().x - 2.0f * ImGui::GetStyle().ItemSpacing.x) * (1.0f / 3.0f);

		OnGui(TimeStep);
		ImGui::Separator();

		for (uint64 Index = 0; Index < Behaviours.GetCount(); ++Index)
		{
			NxFr::Handle<Behaviour> B = Behaviours[Index];
			ImGui::PushID(B->GetId());

			if (GUI::Draw::Button("Move Up", NxFr::Vector2f(ThirdWindowSize, 0.0f)) && Index > 0)
			{
				NxFr::ContainerUtility::Swap<NxFr::Handle<Behaviour>>(Behaviours, Index, Index - 1);
			}
			ImGui::SameLine();
			if (GUI::Draw::Button("Move Down", NxFr::Vector2f(ThirdWindowSize, 0.0f)) && Index < Behaviours.GetCount() - 1)
			{
				NxFr::ContainerUtility::Swap<NxFr::Handle<Behaviour>>(Behaviours, Index, Index + 1);
			}
			ImGui::SameLine();
			if (GUI::Draw::Button("Remove", NxFr::Vector2f(ThirdWindowSize, 0.0f)))
			{
				Behaviours.Remove(Index);
				ImGui::PopID();
				Index--;
				continue;
			}

			B->DrawGui(TimeStep);
			ImGui::Separator();

			ImGui::PopID();
		}

		for (uint64 Index = 0; Index < Components.GetCount(); ++Index)
		{
			NxFr::Handle<Component> C = Components[Index];
			ImGui::PushID(C->GetId());

			if (GUI::Draw::Button("Move Up", NxFr::Vector2f(ThirdWindowSize, 0.0f)) && Index > 0)
			{
				NxFr::ContainerUtility::Swap<NxFr::Handle<Component>>(Components, Index, Index - 1);
			}
			ImGui::SameLine();
			if (GUI::Draw::Button("Move Down", NxFr::Vector2f(ThirdWindowSize, 0.0f)) && Index < Components.GetCount() - 1)
			{
				NxFr::ContainerUtility::Swap<NxFr::Handle<Component>>(Components, Index, Index + 1);
			}
			ImGui::SameLine();
			if (GUI::Draw::Button("Remove", NxFr::Vector2f(ThirdWindowSize, 0.0f)))
			{
				Components.Remove(Index);
				ImGui::PopID();
				Index--;
				continue;
			}

			C->DrawGui(TimeStep);
			ImGui::Separator();

			ImGui::PopID();
		}

		if (GUI::Draw::Button("Add Behaviour", NxFr::Vector2f(HalfWindowSize , 0.0f)))
		{
			NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
			Popup->RegisterCallback([=](NxFr::StringView Input)
			{
				GetWorld()->CreateBehaviour(Input, GetThis(this));
			});
		}

		ImGui::SameLine();

		if (GUI::Draw::Button("Add Component", NxFr::Vector2f(HalfWindowSize, 0.0f)))
		{
			NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
			Popup->RegisterCallback([=](NxFr::StringView Input)
			{
				GetWorld()->CreateComponent(Input, GetThis(this));
			});
		}
	}

	void GameObject::SetEnabled(bool Enabled)
	{
		if (IsEnabled() != Enabled)
		{
			SetFlag(ObjectFlags::Enabled, Enabled);
		}

		UpdateHierarchy();
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

		YAML::Node NodeBehaviours;
		for (auto& B : Behaviours)
		{
			NodeBehaviours.push_back(B->Save());
		}
		Instance["Behaviours"] = NodeBehaviours;

		YAML::Node NodeComponents;
		for (auto& C : Components)
		{
			NodeComponents.push_back(C->Save());
		}
		Instance["Components"] = NodeComponents;

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
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();
		NxFr::Handle<GameObject> This = Factory->GetGameObject(GameObjectId);
		YAML::Node Instance = Node[YamlRoot];

		NxFr::Delegate<NxFr::Handle<GameObject>(const YAML::Node&)> ChildLoad = [&](const YAML::Node& ChildNode)
		{
			NxFr::Handle<GameObject> ChildInstance;
			NxFr::GUID ChildReference = ChildNode[YamlRoot][YamlReference].as<NxFr::GUID>();

			if (ChildReference)
			{
				Prefab* PrefabInstance = Application::GetSystem<WorldSystem>()->LoadPrefab(ChildReference);
				ChildInstance = Factory->DuplicateGameObject(PrefabInstance->GetRoot(), This, true);
			}
			else
			{
				ChildInstance = Factory->CreateGameObject("", This, ChildNode[YamlRoot][YamlId].as<NxFr::GUID>());
				ChildInstance->Load(ChildNode);
			}

			return ChildInstance;
		};

		OnLoad(Instance);

		YAML::Node NodeBehaviours = Instance["Behaviours"];
		for (uint64 Index = 0; Index < NodeBehaviours.size(); ++Index)
		{
			YAML::Node NodeBehaviour = NodeBehaviours[Index];
			NxFr::StringId Type = NodeBehaviour["Type"].as<NxFr::StringId>();
			NxFr::GUID Id = NodeBehaviour["Id"].as<NxFr::GUID>();

			NxFr::Handle<Behaviour> B = Factory->CreateBehaviour(Type, This, Id);
			B->Load(NodeBehaviour);
		}

		YAML::Node NodeComponents = Instance["Components"];
		for (uint64 Index = 0; Index < NodeComponents.size(); ++Index)
		{
			YAML::Node NodeComponent = NodeComponents[Index];
			NxFr::StringId Type = NodeComponent["Type"].as<NxFr::StringId>();
			NxFr::GUID Id = NodeComponent["Id"].as<NxFr::GUID>();

			NxFr::Handle<Behaviour> B = Factory->CreateComponent(Type, This, Id);
			B->Load(NodeComponent);
		}

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
		OnUnload();

		for (auto& B : Behaviours)
		{
			B->Unload();
		}

		for (auto& C : Components)
		{
			C->Unload();
		}

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

	NxFr::Array<NxFr::GUID> GameObject::GetDependencies()
	{
		NxFr::Set<NxFr::GUID> Ids;

		OnGetDependencies(Ids);

		for (auto& B : Behaviours)
		{
			B->OnGetDependencies(Ids);
		}

		for (auto& C : Components)
		{
			C->OnGetDependencies(Ids);
		}

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

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Ids);
	}

	void GameObject::PatchReferences()
	{
		OnPatchReferences();

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

	void GameObject::UpdateHierarchy()
	{
		OnUpdateHierarchy();

		for (auto& B : Behaviours)
		{
			B->UpdateHierarchy();
		}

		for (auto& C : Components)
		{
			C->UpdateHierarchy();
		}

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->UpdateHierarchy();
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

	NxFr::Array<NxFr::Handle<Behaviour>> GameObject::GetBehavioursByType(NxFr::StringId Id)
	{
		NxFr::List<NxFr::Handle<Behaviour>> Result;
		GetBehavioursByType(Id, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	void GameObject::GetBehavioursByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result)
	{
		for (auto& B : Behaviours)
		{
			if (B->GetObjectType() == Id)
			{
				Result.Append(B);
			}
		}
	}

	NxFr::Array<NxFr::Handle<Behaviour>> GameObject::GetBehavioursInChildrenByType(NxFr::StringId Id)
	{
		NxFr::List<NxFr::Handle<Behaviour>> Result;
		GetBehavioursInChildrenByType(Id, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	void GameObject::GetBehavioursInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result)
	{
		GetBehavioursByType(Id, Result);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->GetBehavioursInChildrenByType(Id, Result);
			Iterator = Iterator->GetNext();
		}
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

	NxFr::Array<NxFr::Handle<Component>> GameObject::GetComponentsByType(NxFr::StringId Id)
	{
		NxFr::List<NxFr::Handle<Component>> Result;
		GetComponentsByType(Id, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Component>>(Result);
	}

	void GameObject::GetComponentsByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result)
	{
		for (auto& C : Components)
		{
			if (C->GetObjectType() == Id)
			{
				Result.Append(C);
			}
		}
	}

	NxFr::Array<NxFr::Handle<Component>> GameObject::GetComponentsInChildrenByType(NxFr::StringId Id)
	{
		NxFr::List<NxFr::Handle<Component>> Result;
		GetComponentsInChildrenByType(Id, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Component>>(Result);
	}

	void GameObject::GetComponentsInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result)
	{
		GetComponentsByType(Id, Result);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->GetComponentsInChildrenByType(Id, Result);
			Iterator = Iterator->GetNext();
		}
	}

	void GameObject::OnGui(float TimeStep)
	{
		GUI::Drawer<NxFr::String>::Field(Name, "Name");

		GUI::Drawer<NxFr::GUID>::Property(GameObjectId, "Id");
		GUI::Drawer<NxFr::GUID>::Property(ReferenceId, "Reference");

		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled, "Enabled");
		if (Enabled != IsEnabled())
		{
			SetEnabled(Enabled);
		}

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
	}

	void GameObject::OnLoad(const YAML::Node& Node)
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();
		NxFr::Handle<GameObject> This = Factory->GetGameObject(GameObjectId);

		Name = Node["Name"].as<NxFr::String>();
		NX_ASSERT(GameObjectId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		ReferenceId = Node["Reference"].as<NxFr::GUID>();
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());
		SetFlag(ObjectFlags::Tickable, Node["Tickable"].as<bool>());
		SetFlag((ObjectFlags)ObjectFlag_EnabledInHierarchy, false);
	}

	void GameObject::OnUnload()
	{
		
	}

	void GameObject::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids)
	{
		
	}

	void GameObject::OnPatchReferences()
	{
	}

	void GameObject::OnUpdateHierarchy()
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
	}

	NxFr::GUID GameObject::ReadIdFromYaml(const YAML::Node& Node)
	{
		return Node[YamlRoot][YamlId].as<NxFr::GUID>();
	}

	NxFr::Handle<GameObject> GameObject::GetThis(GameObject* Instance)
	{
		return Instance->GetWorld()->GetGameObject(Instance->GetId());
	}
}
