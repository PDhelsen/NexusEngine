#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class World : public Object
	{
		friend struct WorldObjectFactoryContext;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, World)

		NEXUS_ENGINE_API World(NxFr::GUID WorldId, NxFr::StringView Name = "", bool References = false);
		NEXUS_ENGINE_API ~World();

		NEXUS_ENGINE_API NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NEXUS_ENGINE_API NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool Instantiate = false);
		NEXUS_ENGINE_API void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		NEXUS_ENGINE_API void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		NEXUS_ENGINE_API void DetachGameObject(NxFr::Handle<GameObject> Instance);

		template<typename T> NxFr::Handle<T> CreateBehaviour(NxFr::Handle<GameObject> Target);
		NEXUS_ENGINE_API NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NEXUS_ENGINE_API void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		template<typename T> NxFr::Handle<T> CreateComponent(NxFr::Handle<GameObject> Target);
		NEXUS_ENGINE_API NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target);
		NEXUS_ENGINE_API void DestroyComponent(NxFr::Handle<Component> Instance);

		NEXUS_ENGINE_API bool Belong(NxFr::Handle<GameObject> Instance) const;
		NEXUS_ENGINE_API bool Belong(NxFr::Handle<Behaviour> Instance) const;
		NEXUS_ENGINE_API bool Belong(NxFr::Handle<Component> Instance) const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> FindGameObjects(NxFr::StringView Filter) const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<Behaviour>> FindBehaviours(NxFr::StringView Filter) const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<Component>> FindComponents(NxFr::StringView Filter) const;

		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetRootGameObject() const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<Behaviour>> GetBehaviours() const;
		NEXUS_ENGINE_API NxFr::Handle<Behaviour> GetBehaviour(NxFr::GUID BehaviourId) const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<Component>> GetComponents() const;
		NEXUS_ENGINE_API NxFr::Handle<Component> GetComponent(NxFr::GUID ComponentId) const;

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return GetWorldId(); };
		NEXUS_ENGINE_API NxFr::GUID GetWorldId() const { return WorldId; };
		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_ENGINE_API void SetName(NxFr::StringView Name) { this->Name = Name; GetRootGameObject()->SetName(Name); };

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::GUID WorldId;
		NxFr::String Name;

		class WorldObjectFactory* Factory;
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

