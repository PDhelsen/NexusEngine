#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/World/WorldInfo.h"
#include "NexusEngine/Systems/World/WorldStorage.h"
#include "NexusEngine/Systems/World/World.h"
#include "NexusEngine/Systems/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviour.h"
#include "NexusEngine/Systems/World/Component.h"

namespace NxEn
{
	class NX_ENGINE_API WorldSystem : public System
	{
	public:
		NX_OBJECT(WorldSystem)

		inline static const NxFr::StringId MainWorldId = "Main World"_Sid;

		inline static const NxFr::StringId EventCreatedId = "Created"_Sid;
		inline static const NxFr::StringId EventDestroyedId = "Destroyed"_Sid;
		inline static const NxFr::StringId EventMovedId = "Moved"_Sid;

		WorldSystem();
		~WorldSystem();

		World* CreateWorld(NxFr::StringId Name);
		void DestroyWorld(NxFr::GUID WorldId);
		World* GetWorld(NxFr::GUID WorldId = MainWorldId);
		NxFr::Array<NxFr::GUID> GetWorlds();

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = MainWorldId);
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = MainWorldId);
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);

		template<typename T> NxFr::Handle<T> CreateBehaviour(NxFr::Handle<GameObject> Target) { return static_cast<NxFr::Handle<T>>(CreateBehaviour(T::GetClassType(), Target)); }
		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Behaviour> DuplicateBehaviour(NxFr::Handle<Behaviour> Original, NxFr::Handle<GameObject> Target);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		template<typename T> NxFr::Handle<T> CreateComponent(NxFr::Handle<GameObject> Target) { return static_cast<NxFr::Handle<T>>(CreateComponent(T::GetClassType(), Target)); }
		NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Component> DuplicateComponent(NxFr::Handle<Component>, NxFr::Handle<GameObject> Target);
		void DestroyComponent(NxFr::Handle<Component> Instance);

		bool Belong(NxFr::Handle<Object> Instance, NxFr::GUID WorldId = MainWorldId);
		NxFr::Handle<Object> GetObject(NxFr::GUID ObjectId, NxFr::GUID WorldId = 0);
		NxFr::Array<NxFr::Handle<Object>> GetObjects(WorldObjectType Type = WorldObjectType::GameObject, NxFr::GUID WorldId = MainWorldId);
		NxFr::Array<NxFr::Handle<Object>> Find(NxFr::StringView Query, WorldObjectType Type = WorldObjectType::GameObject, NxFr::GUID WorldId = MainWorldId);
		NxFr::Array<NxFr::Handle<GameObject>> FindGameObjects(NxFr::StringView Query, NxFr::GUID WorldId = MainWorldId);

		template<typename T> Iterator::WorldObjectOf<T> Begin(NxFr::GUID WorldId = MainWorldId) { return Begin(T::GetClassType(), WorldId); }
		Iterator::WorldObject Begin(NxFr::StringId Type, NxFr::GUID WorldId = MainWorldId);
		template<typename T> Iterator::WorldObjectOf<T> End(NxFr::GUID WorldId = MainWorldId) { return End(T::GetClassType(), WorldId); }
		Iterator::WorldObject End(NxFr::StringId Type, NxFr::GUID WorldId = MainWorldId);
		template<typename T> NxFr::Iterator::View<Iterator::WorldObjectOf<T>> View(NxFr::GUID WorldId = MainWorldId) { return { Begin<T>(WorldId), End<T>(WorldId) }; }

		NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnWorldChange() { return OnWorldChange; }
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnWorldObjectChange() { return OnWorldObjectChange; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep) override;

	private:
		WorldManager* GetManager(NxFr::GUID WorldId);

		NxFr::Event<NxFr::StringId, NxFr::GUID> OnWorldChange;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnWorldObjectChange;

		NxFr::Dictionary<NxFr::GUID, WorldManager*> Managers;
	};
}
