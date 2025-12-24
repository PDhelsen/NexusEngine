#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class World : public Object
	{
		friend class WorldSystem;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, World)

		NEXUS_ENGINE_API World(NxFr::GUID WorldId, NxFr::StringView Name = "");
		NEXUS_ENGINE_API ~World();

		NEXUS_ENGINE_API NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NEXUS_ENGINE_API NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool Instantiate = false);
		NEXUS_ENGINE_API void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		NEXUS_ENGINE_API void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		NEXUS_ENGINE_API void DetachGameObject(NxFr::Handle<GameObject> Instance);

		NEXUS_ENGINE_API bool Belong(NxFr::Handle<GameObject> Instance) const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> Find(NxFr::StringView Filter) const;

		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetRootGameObject() const;

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

		ObjectFactory Factory;
		NxFr::Handle<GameObject> Root;
	};
}

