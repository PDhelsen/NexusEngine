#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/World/WorldInfo.h"
#include "NexusEngine/Systems/World/WorldStorage.h"
#include "NexusEngine/Systems/World/World.h"
#include "NexusEngine/Systems/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviour.h"
#include "NexusEngine/Systems/World/Component.h"
#include "NexusEngine/Systems/World/Prefab.h"
#include "NexusEngine/Systems/World/Scene.h"

namespace NxEn
{
	class NX_ENGINE_API WorldSystem : public System
	{
	public:
		NX_OBJECT(WorldSystem)

		inline static const NxFr::StringId MainWorldId = "Main World"_Sid;
		inline static const NxFr::StringId TemplateWorldId = "Template World"_Sid;

		inline static const NxFr::StringId EventCreatedId = "Created"_Sid;
		inline static const NxFr::StringId EventDestroyedId = "Destroyed"_Sid;
		inline static const NxFr::StringId EventMovedId = "Moved"_Sid;

		WorldSystem();
		~WorldSystem();

		World* CreateWorld(NxFr::StringId Name);
		void DestroyWorld(NxFr::GUID WorldId);
		World* GetWorld(NxFr::GUID WorldId = MainWorldId);
		NxFr::Array<NxFr::GUID> GetWorlds(bool IncludeTemplateWorld = false);
		bool IsWorld(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<GameObject> InstantiateScene(Scene* Instance, bool Single = false, NxFr::GUID WorldId = MainWorldId);
		void DestroyScene(Scene* Instance, NxFr::GUID WorldId = MainWorldId);
		void DestroyScenes(NxFr::GUID WorldId = MainWorldId);
		void PackScene(Scene* Instance, NxFr::GUID WorldId = MainWorldId);
		void PackScenes(NxFr::GUID WorldId = MainWorldId);
		bool IsSceneInstantiated(NxFr::GUID SceneId, NxFr::GUID WorldId = MainWorldId);
		bool IsScene(NxFr::Handle<GameObject> Instance);

		void PackPrefab(Prefab* Instance, NxFr::Handle<GameObject> Original);
		void UnpackPrefab(NxFr::Handle<GameObject> Original);
		bool IsPrefab(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = MainWorldId);
		NxFr::Handle<GameObject> InstantiateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = MainWorldId);
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<const GameObject> Original, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		YAML::Node SerializeGameObject(NxFr::Handle<GameObject> Instance);
		NxFr::Handle<GameObject> DeserializeGameObject(YAML::Node Node);
		void UnloadGameObject(NxFr::Handle<GameObject> Instance);
		NxFr::Handle<GameObject> TemplateGameObject(NxFr::Handle<GameObject> Original, NxFr::GUID TemplateId);
		void UntemplateGameObject(NxFr::Handle<GameObject> Instance);

		template<typename T> NxFr::Handle<T> CreateBehaviour(NxFr::Handle<GameObject> Target) { return static_cast<NxFr::Handle<T>>(CreateBehaviour(T::GetClassType(), Target)); }
		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Behaviour> InstantiateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Behaviour> DuplicateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		template<typename T> NxFr::Handle<T> CreateComponent(NxFr::Handle<GameObject> Target) { return static_cast<NxFr::Handle<T>>(CreateComponent(T::GetClassType(), Target)); }
		NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Component> InstantiateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Component> DuplicateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target);
		void DestroyComponent(NxFr::Handle<Component> Instance);

		bool Belong(NxFr::Handle<Object> Instance, NxFr::GUID WorldId = MainWorldId);
		WorldObjectType GetType(NxFr::Handle<Object> Instance, NxFr::GUID WorldId = Object::NullId);
		NxFr::Handle<Object> GetObject(NxFr::GUID ObjectId, NxFr::GUID WorldId = Object::NullId);
		NxFr::Array<NxFr::Handle<Object>> GetObjects(WorldObjectType Type = WorldObjectType::All, NxFr::GUID WorldId = Object::NullId);
		NxFr::Handle<GameObject> GetScene(NxFr::GUID SceneId, NxFr::GUID WorldId = Object::NullId);
		NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>> GetScenes(NxFr::GUID WorldId = Object::NullId);
		NxFr::Array<NxFr::GUID> Find(NxFr::StringView Query, WorldObjectType Type = WorldObjectType::All, NxFr::GUID WorldId = Object::NullId);
		NxFr::Array<NxFr::GUID> FindGameObjects(NxFr::StringView Query, NxFr::GUID WorldId = Object::NullId);
		NxFr::Array<NxFr::GUID> GetDependencies(NxFr::GUID Id, bool Recursive = false);
		void GetDependencies(NxFr::GUID Id, bool Recursive, NxFr::Set<NxFr::GUID>& Result);

		template<typename T>
		NxFr::Handle<T> Cast(NxFr::Handle<Object> Instance, NxFr::GUID WorldId = Object::NullId)
		{
			if (Instance->GetObjectType() == T::GetClassType())
			{
				return Instance;
			}

			WorldObjectType Type = GetType(Instance, WorldId);
			if (Type == WorldObjectType::GameObject && T::GetClassType() == GameObject::GetClassType())
			{
				return Instance;
			}
			else if (Type == WorldObjectType::Behaviour && T::GetClassType() == Behaviour::GetClassType())
			{
				return Instance;
			}
			else if (Type == WorldObjectType::Component && T::GetClassType() == Component::GetClassType())
			{
				return Instance;
			}

			return NxFr::Handle<T>();
		}

		template<typename T> Iterator::WorldObjectOf<T> Begin(NxFr::GUID WorldId = MainWorldId) { return Begin(T::GetClassType(), WorldId); }
		Iterator::WorldObject Begin(NxFr::StringId Type, NxFr::GUID WorldId = MainWorldId);
		template<typename T> Iterator::WorldObjectOf<T> End(NxFr::GUID WorldId = MainWorldId) { return End(T::GetClassType(), WorldId); }
		Iterator::WorldObject End(NxFr::StringId Type, NxFr::GUID WorldId = MainWorldId);
		template<typename T> NxFr::Iterator::View<Iterator::WorldObjectOf<T>> View(NxFr::GUID WorldId = MainWorldId) { return { Begin<T>(WorldId), End<T>(WorldId) }; }

		NxFr::Event<NxFr::StringId, NxFr::GUID, bool>& GetOnWorldChange() { return OnWorldChange; }
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnWorldObjectChange() { return OnWorldObjectChange; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep) override;

	private:
		WorldManager* GetManager(NxFr::GUID WorldId);

		NxFr::Event<NxFr::StringId, NxFr::GUID, bool> OnWorldChange;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnWorldObjectChange;

		NxFr::Dictionary<NxFr::GUID, WorldManager*> Managers;
	};
}
