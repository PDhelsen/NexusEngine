#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldManager.h"

#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	WorldManager::WorldManager()
		: Storages(), Objects(), Remap(), Handles(),
		WorldInstance(nullptr), SceneInstances()
	{
		
	}

	WorldManager::~WorldManager()
	{
		for (auto [Type, Storage] : Storages)
		{
			delete Storage;
		}
	}

	void WorldManager::Tick(float TimeStep)
	{
		WorldInstance->Tick(TimeStep);
	}

	void WorldManager::Reserve(NxFr::StringId Type, uint64 Size)
	{
		WorldStorage* Storage = GetStorage(Type);
		Resize(Storage, Size);
	}

	void WorldManager::RecordIds(NxFr::GUID OrignalId, NxFr::GUID InstanceId)
	{
		NxFr::Dictionary<NxFr::GUID, NxFr::GUID>* Ids = Remap.TryGet();
		if (Ids)
		{
			Ids->Append(OrignalId, InstanceId);
		}
	}

	NxFr::GUID WorldManager::ResolveId(NxFr::GUID InstanceId)
	{
		NxFr::Dictionary<NxFr::GUID, NxFr::GUID>* Ids = Remap.TryGet();
		if (Ids)
		{
			NxFr::GUID* ResolvedId = Ids->TryGet(InstanceId);
			InstanceId = ResolvedId ? *ResolvedId : InstanceId;
		}

		return InstanceId;
	}

	NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>& WorldManager::GetIdsRemap()
	{
		return Remap;
	}

	World* WorldManager::CreateWorld(NxFr::StringId Name)
	{
		WorldInstance = new World();
		WorldInstance->WorldId = Name;
		return WorldInstance;
	}

	void WorldManager::DestroyWorld()
	{
		delete WorldInstance;
	}

	World* WorldManager::GetWorld()
	{
		return WorldInstance;
	}

	void WorldManager::SetWorldRoot(NxFr::Handle<GameObject> Instance)
	{
		WorldInstance->Root = Instance;
		if (Instance)
		{
			Instance->SetTickable(true);
		}
	}

	void WorldManager::RegisterScene(NxFr::GUID SceneId, NxFr::Handle<GameObject> Root)
	{
		SceneInstances.Append(SceneId, Root);
	}

	void WorldManager::UnregisterScene(NxFr::GUID SceneId)
	{
		SceneInstances.Remove(SceneId);
	}

	NxFr::Handle<GameObject> WorldManager::CreateGameObject(NxFr::Handle<GameObject> Parent, NxFr::GUID GameObjectId, NxFr::GUID TemplateId, ReferenceMode Mode)
	{
		if (GameObjectId == 0)
		{
			GameObjectId = NxFr::Integer::GenerateGuid();
		}

		WorldStorage* Storage = GetStorage(GameObject::GetClassType());
		NxFr::Handle<GameObject> Instance = Allocate(Storage, GameObjectId, WorldObjectType::GameObject);

		Instance->WorldId = WorldInstance->GetId();
		Instance->GameObjectId = GameObjectId;
		Instance->TemplateId = TemplateId;
		Instance->Proxy = Mode == ReferenceMode::Keep && TemplateId;

		if (Parent)
		{
			AttachGameObject(Instance, Parent, Parent->GetChildCount());
		}

		return Instance;
	}

	NxFr::Handle<GameObject> WorldManager::CreateGameObject(YAML::Node Node, NxFr::Handle<GameObject> Parent, ReferenceMode Mode)
	{
		NxFr::GUID Id = Node["Id"].as<NxFr::GUID>();
		NxFr::GUID TemplateId = Node["TemplateId"].as<NxFr::GUID>();

		NxFr::Handle<GameObject> Instance = CreateGameObject(Parent, !Objects.TryGet(Id) ? Id : 0, TemplateId, Mode);
		RecordIds(Id, Instance->GetId());

		YAML::Node NodeBehaviours = Node["Behaviours"];
		for (uint64 Index = 0; Index < NodeBehaviours.size(); ++Index)
		{
			CreateBehaviour(NodeBehaviours[Index], Instance);
		}

		YAML::Node NodeComponents = Node["Components"];
		for (uint64 Index = 0; Index < NodeComponents.size(); ++Index)
		{
			CreateComponent(NodeComponents[Index], Instance);
		}

		YAML::Node NodeChildren = Node["Children"];
		for (uint64 Index = 0; Index < NodeChildren.size(); ++Index)
		{
			CreateGameObject(NodeChildren[Index], Instance, ReferenceMode::Keep);
		}

		return Instance;
	}

	NxFr::Handle<GameObject> WorldManager::DuplicateGameObject(NxFr::Handle<const GameObject> Original, NxFr::Handle<GameObject> Parent, ReferenceMode Mode)
	{
		NxFr::Handle<GameObject> Instance = CreateGameObject(Parent, 0, Original->GetTemplateId(), ReferenceMode::Ignore);
		RecordIds(Original->GetId(), Instance->GetId());

		for (auto& B : Original->Behaviours)
		{
			DuplicateBehaviour(B, Instance);
		}

		for (auto& C : Original->Components)
		{
			DuplicateComponent(C, Instance);
		}

		NxFr::Handle<GameObject> Child = Original->GetChild();
		while (Child)
		{
			if (Child->GetTemplateId())
			{
				if (Mode == ReferenceMode::Keep)
				{
					CreateGameObject(Instance, 0, Child->GetTemplateId(), ReferenceMode::Keep);
				}
				else if (Mode == ReferenceMode::Resolve)
				{
					NxFr::Handle<GameObject> Target = Application::GetSystem<AssetsSystem>()->Load<Prefab>(Child->GetTemplateId())->GetRoot();
					DuplicateGameObject(Target, Instance, ReferenceMode::Resolve);
				}
				else // Mode == ReferenceMode::Ignore
				{
					DuplicateGameObject(Child, Instance, ReferenceMode::Ignore);
				}
			}
			else
			{
				DuplicateGameObject(Child, Instance, Mode);
			}

			Child = Child->GetNext();
		}

		return Instance;
	}

	void WorldManager::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		NxFr::Handle<GameObject> Child = Instance->GetChild();
		while (Child)
		{
			NxFr::Handle<GameObject> Next = Child->GetNext();
			DestroyGameObject(Child);
			Child = Next;
		}

		while (Instance->Components.GetCount())
		{
			DestroyComponent(Instance->Components[0]);
		}

		while (Instance->Behaviours.GetCount())
		{
			DestroyBehaviour(Instance->Behaviours[0]);
		}

		DetachGameObject(Instance);

		WorldStorage* Storage = GetStorage(GameObject::GetClassType());
		Free(Storage, Instance);
	}

	void WorldManager::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index)
	{
		DetachGameObject(Instance);

		Instance->Parent = Parent;

		if (!Parent->Child)
		{
			Parent->Child = Instance;
		}
		else
		{
			NxFr::Handle<GameObject> Target = Parent->Child;
			while (Target->Next && Index > 0)
			{
				Index--;
				Target = Target->Next;
			}

			if (Index > 0)
			{
				Target->Next = Instance;
				Instance->Prev = Target;
			}
			else
			{
				if (Target->Prev)
				{
					Target->Prev->Next = Instance;
					Instance->Prev = Target->Prev;
				}
				Target->Prev = Instance;
				Instance->Next = Target;
			}
		}
	}

	void WorldManager::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (Instance->Parent && Instance->Parent->Child == Instance)
		{
			Instance->Parent->Child = Instance->Next;
		}
		if (Instance->Prev)
		{
			Instance->Prev->Next = Instance->Next;
		}
		if (Instance->Next)
		{
			Instance->Next->Prev = Instance->Prev;
		}

		Instance->Parent = NxFr::Handle<GameObject>();
		Instance->Prev = NxFr::Handle<GameObject>();
		Instance->Next = NxFr::Handle<GameObject>();
	}

	NxFr::Handle<Behaviour> WorldManager::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId)
	{
		if (BehaviourId == 0)
		{
			BehaviourId = NxFr::Integer::GenerateGuid();
		}

		WorldStorage* Storage = GetStorage(Type);
		NxFr::Handle<Behaviour> Instance = Allocate(Storage, BehaviourId, WorldObjectType::Behaviour);

		Instance->BehaviourId = BehaviourId;
		Instance->Target = Target;
		Target->Behaviours.Append(Instance);

		return Instance;
	}

	NxFr::Handle<Behaviour> WorldManager::CreateBehaviour(YAML::Node Node, NxFr::Handle<GameObject> Target)
	{
		NxFr::StringId Type = Node["Type"].as<NxFr::StringId>();
		NxFr::GUID Id = Node["Id"].as<NxFr::GUID>();

		NxFr::Handle<Behaviour> Instance = CreateBehaviour(Type, Target, !Objects.TryGet(Id) ? Id : 0);
		RecordIds(Id, Instance->GetId());

		return Instance;
	}

	NxFr::Handle<Behaviour> WorldManager::DuplicateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target)
	{
		NxFr::Handle<Behaviour> Instance = CreateBehaviour(Original->GetObjectType(), Target);
		RecordIds(Original->GetId(), Instance->GetId());

		return Instance;
	}

	void WorldManager::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NxFr::Handle<GameObject> Target = Instance->GetGameObject();
		Target->Behaviours.RemoveSwap(NxFr::ContainerUtility::Find(Target->Behaviours, Instance).Id());
		Instance->Target = NxFr::Handle<GameObject>();

		WorldStorage* Storage = GetStorage(Instance->GetObjectType());
		Free(Storage, Instance);
	}

	NxFr::Handle<Component> WorldManager::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID ComponentId)
	{
		if (ComponentId == 0)
		{
			ComponentId = NxFr::Integer::GenerateGuid();
		}

		WorldStorage* Storage = GetStorage(Type);
		NxFr::Handle<Component> Instance = Allocate(Storage, ComponentId, WorldObjectType::Component);

		Instance->ComponentId = ComponentId;
		Instance->Target = Target;
		Target->Components.Append(Instance);

		return Instance;
	}

	NxFr::Handle<Component> WorldManager::CreateComponent(YAML::Node Node, NxFr::Handle<GameObject> Target)
	{
		NxFr::StringId Type = Node["Type"].as<NxFr::StringId>();
		NxFr::GUID Id = Node["Id"].as<NxFr::GUID>();

		NxFr::Handle<Component> Instance = CreateComponent(Type, Target, !Objects.TryGet(Id) ? Id : 0);
		RecordIds(Id, Instance->GetId());

		return Instance;
	}

	NxFr::Handle<Component> WorldManager::DuplicateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target)
	{
		NxFr::Handle<Component> Instance = CreateComponent(Original->GetObjectType(), Target);
		RecordIds(Original->GetId(), Instance->GetId());

		return Instance;
	}

	void WorldManager::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		NxFr::Handle<GameObject> Target = Instance->GetGameObject();
		Target->Components.RemoveSwap(NxFr::ContainerUtility::Find(Target->Components, Instance).Id());
		Instance->Target = NxFr::Handle<GameObject>();

		WorldStorage* Storage = GetStorage(Instance->GetObjectType());
		Free(Storage, Instance);
	}

	bool WorldManager::Belong(NxFr::Handle<Object> Instance)
	{
		NxFr::GUID ObjectId = Instance->GetId();
		const WorldObject* Info = Objects.TryGet(ObjectId);

		if (!Info)
		{
			return false;
		}
		else if (NxFr::Enum::CheckFlag(Info->Type, WorldObjectType::GameObject))
		{
			return ((NxFr::Handle<GameObject>)Instance)->GetWorldId() == WorldInstance->GetId();
		}
		else if (NxFr::Enum::CheckFlag(Info->Type, WorldObjectType::Behaviour))
		{
			return ((NxFr::Handle<Behaviour>)Instance)->GetGameObject()->GetWorldId() == WorldInstance->GetId();
		}
		else if (NxFr::Enum::CheckFlag(Info->Type, WorldObjectType::Component))
		{
			return ((NxFr::Handle<Component>)Instance)->GetGameObject()->GetWorldId() == WorldInstance->GetId();
		}
		else
		{
			return false;
		}
	}

	WorldObjectType WorldManager::GetType(NxFr::Handle<Object> Instance)
	{
		NxFr::GUID ObjectId = Instance->GetId();
		const WorldObject* Info = Objects.TryGet(ObjectId);
		return Info ? Info->Type : WorldObjectType::None;
	}

	NxFr::Handle<Object> WorldManager::GetObject(NxFr::GUID ObjectId)
	{
		ObjectId = ResolveId(ObjectId);
		const WorldObject* Info = Objects.TryGet(ObjectId);
		return Info ? Info->Handle : NxFr::Handle<Object>();
	}

	NxFr::Handle<GameObject> WorldManager::GetScene(NxFr::GUID SceneId)
	{
		NxFr::Handle<GameObject>* Root = SceneInstances.TryGet(SceneId);
		return Root ? *Root : NxFr::Handle<GameObject>();
	}

	NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>> WorldManager::GetScenes()
	{
		return SceneInstances;
	}

	NxFr::Array<NxFr::Handle<Object>> WorldManager::GetObjects(WorldObjectType Type)
	{
		NxFr::List<NxFr::Handle<Object>> Result;

		for (auto& [Id, Info] : Objects)
		{
			if (NxFr::Enum::CheckFlag(Info.Type, Type))
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainerUtility::ToArray<NxFr::Handle<Object>>(Result);
	}

	NxFr::Array<NxFr::GUID> WorldManager::Find(NxFr::StringView Query, WorldObjectType Type)
	{
		NxFr::List<NxFr::GUID> Result;

		Utils::Filter Filter(Query);
		for (auto& [Id, Info] : Objects)
		{
			if (NxFr::Enum::CheckFlag(Info.Type, Type) && Filter.FilterObject(*Info.Handle.GetRedirectedPointer()))
			{
				Result.Append(Id);
			}
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	NxFr::Array<NxFr::GUID> WorldManager::FindGameObjects(NxFr::StringView Query)
	{
		NxFr::List<NxFr::GUID> Result;

		Utils::Filter Filter(Query);
		for (auto& [Id, Info] : Objects)
		{
			if (Filter.FilterObject(*Info.Handle.GetRedirectedPointer()))
			{
				if (NxFr::Enum::CheckFlag(Info.Type, WorldObjectType::GameObject))
				{
					Result.Append(Info.Handle->GetId());
				}
				else if (NxFr::Enum::CheckFlag(Info.Type, WorldObjectType::Behaviour))
				{
					Result.Append(((NxFr::Handle<Behaviour>)Info.Handle)->GetGameObject()->GetId());
				}
				else if (NxFr::Enum::CheckFlag(Info.Type, WorldObjectType::Component))
				{
					Result.Append(((NxFr::Handle<Component>)Info.Handle)->GetGameObject()->GetId());
				}
				else
				{
				}
			}
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
	}

	Iterator::WorldObject WorldManager::Begin(NxFr::StringId Type)
	{
		WorldStorage* Storage = GetStorage(Type);
		return Storage->Begin();
	}

	Iterator::WorldObject WorldManager::End(NxFr::StringId Type)
	{
		WorldStorage* Storage = GetStorage(Type);
		return Storage->End();
	}

	NxFr::Handle<Object> WorldManager::Allocate(WorldStorage* Storage, NxFr::GUID ObjectId, WorldObjectType Type)
	{
		Resize(Storage, Storage->GetCount() + 1);

		Object* Instance = &Storage->Append();
		uint64 Index = Storage->GetCount() - 1;

		NxFr::Handle<Object> Handle = Handles.AcquireHandle(Instance);
		Objects.Append(ObjectId, WorldObject{ .Id = ObjectId, .Type = Type, .Index = Index, .Handle = Handle });

		return Handle;
	}

	void WorldManager::Free(WorldStorage* Storage, NxFr::Handle<Object> Instance)
	{
		NxFr::GUID Id = Instance->GetId();
		WorldObject Info = Objects.Get(Id);
		uint64 Index = Info.Index;

		Handles.ReleaseHandle(Instance);
		Objects.Remove(Id);
		Storage->Remove(Info.Index);

		// Remove use RemoveSwap
		// So after calling remove, the last element is now at the removed index
		// So the handle needs to be updated
		Update(Storage, Info.Index);
	}

	void WorldManager::Update(WorldStorage* Storage, uint64 Index)
	{
		uint64 Count = Storage->GetCount();
		if (Count == 0 || Count == Index)
		{
			return;
		}

		Object& Instance = Storage->Get(Index);
		NxFr::GUID Id = Instance.GetId();

		WorldObject& Info = Objects.Get(Id);
		Info.Index = Index;

		Handles.UpdateHandle(Info.Handle, &Instance);
	}

	void WorldManager::Resize(WorldStorage* Storage, uint64 Size)
	{
		if (Size < Storage->GetCapacity())
		{
			return;
		}

		Size = NxFr::Math::Max(Size, Storage->GetCapacity() * 2);
		Storage->Reserve(Size);

		for (uint64 Index = 0; Index < Storage->GetCount(); ++Index)
		{
			Update(Storage, Index);
		}
	}

	WorldStorage* WorldManager::GetStorage(NxFr::StringId Type)
	{
		WorldStorage** Storage = Storages.TryGet(Type);

		if (!Storage)
		{
			Storage = &Storages.Append(Type, WorldStorage::GetFactory().Create(Type));
		}

		return *Storage;
	}
}
