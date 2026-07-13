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

		for (auto& C : Components)
		{
			C->Shutdown();
		}

		for (auto& B : Behaviours)
		{
			B->Shutdown();
		}

		OnShutdown();
		SetFlag(ObjectFlags::Initialized, false);
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

	void GameObject::Tick(float TimeStep)
	{
		if (!IsTicking())
		{
			return;
		}

		OnTick(TimeStep);

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

	void GameObject::Draw()
	{
		float HalfWindowSize = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * (1.0f / 2.0f);
		float ThirdWindowSize = (ImGui::GetContentRegionAvail().x - 2.0f * ImGui::GetStyle().ItemSpacing.x) * (1.0f / 3.0f);

		OnDraw();
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

			B->Draw();
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

			C->Draw();
			ImGui::Separator();

			ImGui::PopID();
		}

		if (GUI::Draw::Button("Add Behaviour", NxFr::Vector2f(HalfWindowSize, 0.0f)))
		{
			NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
			Popup->RegisterCallback([&](NxFr::StringView Input)
			{
				WorldSystem* World = Application::GetSystem<WorldSystem>();
				World->CreateBehaviour(Input, World->GetObject(GameObjectId, WorldId));
			});
		}

		ImGui::SameLine();

		if (GUI::Draw::Button("Add Component", NxFr::Vector2f(HalfWindowSize, 0.0f)))
		{
			NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
			Popup->RegisterCallback([&](NxFr::StringView Input)
			{
				WorldSystem* World = Application::GetSystem<WorldSystem>();
				World->CreateComponent(Input, World->GetObject(GameObjectId, WorldId));
			});
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

	NxFr::Handle<GameObject> GameObject::GetThis() const
	{
		return Application::GetSystem<WorldSystem>()->GetObject(GameObjectId);
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

	Iterator::WorldHierarchy GameObject::BeginChild() const
	{
		return Iterator::WorldHierarchy(GetThis());
	}

	Iterator::WorldHierarchy GameObject::EndChild() const
	{
		return Iterator::WorldHierarchy(Next);
	}

	NxFr::Handle<Behaviour> GameObject::GetBehaviourById(NxFr::GUID BehaviourId)
	{
		for (auto& B : Behaviours)
		{
			if (B->GetId() == BehaviourId)
			{
				return B;
			}
		}

		return NxFr::Handle<Behaviour>();
	}

	NxFr::Handle<Behaviour> GameObject::GetBehaviourByType(NxFr::StringId BehaviourType)
	{
		for (auto& B : Behaviours)
		{
			if (B->GetObjectType() == BehaviourType)
			{
				return B;
			}
		}

		return NxFr::Handle<Behaviour>();
	}

	NxFr::Array<NxFr::Handle<Behaviour>> GameObject::GetBehavioursByType(NxFr::StringId BehaviourType)
	{
		NxFr::List<NxFr::Handle<Behaviour>> Result;
		GetBehavioursByType(BehaviourType, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	void GameObject::GetBehavioursByType(NxFr::StringId BehaviourType, NxFr::List<NxFr::Handle<Behaviour>>& Result)
	{
		for (auto& B : Behaviours)
		{
			if (B->GetObjectType() == BehaviourType)
			{
				Result.Append(B);
			}
		}
	}

	NxFr::Array<NxFr::Handle<Behaviour>> GameObject::GetBehavioursInChildrenByType(NxFr::StringId BehaviourType)
	{
		NxFr::List<NxFr::Handle<Behaviour>> Result;
		GetBehavioursInChildrenByType(BehaviourType, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	void GameObject::GetBehavioursInChildrenByType(NxFr::StringId BehaviourType, NxFr::List<NxFr::Handle<Behaviour>>& Result)
	{
		GetBehavioursByType(BehaviourType, Result);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->GetBehavioursInChildrenByType(BehaviourType, Result);
			Iterator = Iterator->GetNext();
		}
	}

	NxFr::Handle<Component> GameObject::GetComponentById(NxFr::GUID ComponentId)
	{
		for (auto& C : Components)
		{
			if (C->GetId() == ComponentId)
			{
				return C;
			}
		}

		return NxFr::Handle<Component>();
	}

	NxFr::Handle<Component> GameObject::GetComponentByType(NxFr::StringId ComponentType)
	{
		for (auto& C : Components)
		{
			if (C->GetObjectType() == ComponentType)
			{
				return C;
			}
		}

		return NxFr::Handle<Component>();
	}

	NxFr::Array<NxFr::Handle<Component>> GameObject::GetComponentsByType(NxFr::StringId ComponentType)
	{
		NxFr::List<NxFr::Handle<Component>> Result;
		GetComponentsByType(ComponentType, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Component>>(Result);
	}

	void GameObject::GetComponentsByType(NxFr::StringId ComponentType, NxFr::List<NxFr::Handle<Component>>& Result)
	{
		for (auto& C : Components)
		{
			if (C->GetObjectType() == ComponentType)
			{
				Result.Append(C);
			}
		}
	}

	NxFr::Array<NxFr::Handle<Component>> GameObject::GetComponentsInChildrenByType(NxFr::StringId ComponentType)
	{
		NxFr::List<NxFr::Handle<Component>> Result;
		GetComponentsInChildrenByType(ComponentType, Result);
		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Component>>(Result);
	}

	void GameObject::GetComponentsInChildrenByType(NxFr::StringId ComponentType, NxFr::List<NxFr::Handle<Component>>& Result)
	{
		GetComponentsByType(ComponentType, Result);

		NxFr::Handle<GameObject> Iterator = GetChild();
		while (Iterator)
		{
			Iterator->GetComponentsInChildrenByType(ComponentType, Result);
			Iterator = Iterator->GetNext();
		}
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

	void GameObject::OnDraw()
	{
		GUI::Drawer<NxFr::String>::Field(Name, "Name");

		GUI::Drawer<NxFr::GUID>::Property(GameObjectId, "Id");

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

		Name = Instance.Name;

		SetFlag(ObjectFlags::Enabled, Instance.IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance.IsTickable());
		SetFlag(ObjectFlags::EnabledInHierarchy, false);
	}
}
