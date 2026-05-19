#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

#include "NexusEngine/Systems/World/Components/Components/Tags.h"

namespace NxEn
{
	class World : public Object
	{
		friend struct WorldObjectFactoryContext;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, World)

		NX_ENGINE_API World(NxFr::StringId Name, bool References = false);
		NX_ENGINE_API ~World();

		NX_ENGINE_API NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NX_ENGINE_API NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool Instantiate = false);
		NX_ENGINE_API void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		NX_ENGINE_API void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		NX_ENGINE_API void DetachGameObject(NxFr::Handle<GameObject> Instance);

		template<typename T> NxFr::Handle<T> CreateBehaviour(NxFr::Handle<GameObject> Target);
		NX_ENGINE_API NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NX_ENGINE_API void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		template<typename T> NxFr::Handle<T> CreateComponent(NxFr::Handle<GameObject> Target);
		NX_ENGINE_API NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NX_ENGINE_API void DestroyComponent(NxFr::Handle<Component> Instance);

		NX_ENGINE_API bool Belong(NxFr::Handle<GameObject> Instance) const;
		NX_ENGINE_API bool Belong(NxFr::Handle<Behaviour> Instance) const;
		NX_ENGINE_API bool Belong(NxFr::Handle<Component> Instance) const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> Find(NxFr::StringView Query) const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> FindGameObjects(NxFr::StringView Query) const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Behaviour>> FindBehaviours(NxFr::StringView Query) const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Component>> FindComponents(NxFr::StringView Query) const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Tags>> FindTags(NxFr::StringView Query, bool MatchAll = false);

		NX_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetRootGameObject() const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Behaviour>> GetBehaviours() const;
		NX_ENGINE_API NxFr::Handle<Behaviour> GetBehaviour(NxFr::GUID BehaviourId) const;
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Component>> GetComponents() const;
		NX_ENGINE_API NxFr::Handle<Component> GetComponent(NxFr::GUID ComponentId) const;

		WorldObjectIterator<GameObject> BeginGameObjects() { return Factory.BeginGameObjects(); }
		WorldObjectIterator<GameObject> EndGameObjects() { return Factory.EndGameObjects(); }
		template<typename T> WorldObjectIterator<T> BeginBehaviours() { return Factory.BeginBehaviours<T>(); }
		template<typename T> WorldObjectIterator<T> EndBehaviours() { return Factory.EndBehaviours<T>(); }
		template<typename T> WorldObjectIterator<T> BeginComponents() { return Factory.BeginComponents<T>(); }
		template<typename T> WorldObjectIterator<T> EndComponents() { return Factory.EndComponents<T>(); }

		NX_ENGINE_API NxFr::GUID GetId() const override { return GetWorldId(); };
		NX_ENGINE_API NxFr::GUID GetWorldId() const { return WorldId; };
		NX_ENGINE_API NxFr::StringView GetName() const override { return WorldId.GetString(); };

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::StringId WorldId;

		WorldObjectFactory Factory;
		NxFr::Handle<GameObject> Root;
	};

	template<typename T>
	inline NxFr::Handle<T> World::CreateBehaviour(NxFr::Handle<GameObject> Target)
	{
		return static_cast<NxFr::Handle<T>>(CreateBehaviour(T::GetClassType(), Target));
	}

	template<typename T>
	inline NxFr::Handle<T> World::CreateComponent(NxFr::Handle<GameObject> Target)
	{
		return static_cast<NxFr::Handle<T>>(CreateComponent(T::GetClassType(), Target));
	}
}

