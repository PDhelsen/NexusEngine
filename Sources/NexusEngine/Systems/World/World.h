#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	class World : public Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, World)

		NEXUS_ENGINE_API World(NxFr::StringId Id);
		NEXUS_ENGINE_API ~World();

		NEXUS_ENGINE_API GameObject* GetGameObject(NxFr::GUID Id);
		NEXUS_ENGINE_API GameObject* CreateGameObject(NxFr::StringView Name, GameObject* Parent = nullptr);
		NEXUS_ENGINE_API GameObject* DuplicateGameObject(GameObject* Instance, GameObject* Parent = nullptr);
		NEXUS_ENGINE_API void DestroyGameObject(GameObject* Instance);
		NEXUS_ENGINE_API void AttachGameObject(GameObject* Instance, GameObject* Parent, int64 Index = -1);

		NEXUS_ENGINE_API bool Belong(GameObject* Instance);
		NEXUS_ENGINE_API NxFr::Array<GameObject*> Find(NxFr::StringView Filter);

		NEXUS_ENGINE_API const NxFr::List<GameObject>& GetObjects() const { return Objects; }
		NEXUS_ENGINE_API uint64 GetObjectCount() const { return Objects.GetCount(); }
		NEXUS_ENGINE_API GameObject* GetRoot() { return Root; };

		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return Id; };

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NEXUS_ENGINE_API void Attach(GameObject* Instance, GameObject* Parent, uint64 Index);
		NEXUS_ENGINE_API void Detach(GameObject* Instance);

	private:
		NxFr::StringId Id;
		NxFr::String Name;

		NxFr::List<GameObject> Objects;
		NxFr::Dictionary<NxFr::GUID, uint64> Ids;
		GameObject* Root;
	};
}

