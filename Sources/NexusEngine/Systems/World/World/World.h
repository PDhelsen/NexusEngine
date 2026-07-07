#pragma once

#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

namespace NxEn
{
	class NX_ENGINE_API World : public Object
	{
		friend struct WorldObjectFactoryContext;

	public:
		NX_OBJECT(World)

		World(NxFr::StringId Name, bool References = false);
		~World();

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool Instantiate = false);
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		void DetachGameObject(NxFr::Handle<GameObject> Instance);

		template<typename T> NxFr::Handle<T> CreateBehaviour(NxFr::Handle<GameObject> Target);
		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		template<typename T> NxFr::Handle<T> CreateComponent(NxFr::Handle<GameObject> Target);
		NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		void DestroyComponent(NxFr::Handle<Component> Instance);

		bool Belong(NxFr::Handle<GameObject> Instance) const;
		bool Belong(NxFr::Handle<Behaviour> Instance) const;
		bool Belong(NxFr::Handle<Component> Instance) const;
		NxFr::Array<NxFr::Handle<GameObject>> Find(NxFr::StringView Query) const;
		NxFr::Array<NxFr::Handle<GameObject>> FindGameObjects(NxFr::StringView Query) const;
		NxFr::Array<NxFr::Handle<Behaviour>> FindBehaviours(NxFr::StringView Query) const;
		NxFr::Array<NxFr::Handle<Component>> FindComponents(NxFr::StringView Query) const;

		NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;
		NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NxFr::Handle<GameObject> GetRootGameObject() const;
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehaviours() const;
		NxFr::Handle<Behaviour> GetBehaviour(NxFr::GUID BehaviourId) const;
		NxFr::Array<NxFr::Handle<Component>> GetComponents() const;
		NxFr::Handle<Component> GetComponent(NxFr::GUID ComponentId) const;

		WorldObjectIterator<GameObject> BeginGameObjects() { return Factory.BeginGameObjects(); }
		WorldObjectIterator<GameObject> EndGameObjects() { return Factory.EndGameObjects(); }
		template<typename T> WorldObjectIterator<T> BeginBehaviours() { return Factory.BeginBehaviours<T>(); }
		template<typename T> WorldObjectIterator<T> EndBehaviours() { return Factory.EndBehaviours<T>(); }
		template<typename T> WorldObjectIterator<T> BeginComponents() { return Factory.BeginComponents<T>(); }
		template<typename T> WorldObjectIterator<T> EndComponents() { return Factory.EndComponents<T>(); }

		NxFr::GUID GetId() const override { return GetWorldId(); };
		NxFr::GUID GetWorldId() const { return WorldId; };
		NxFr::StringView GetName() const override { return WorldId.GetString(); };

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

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

