#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/World/WorldInfo.h"
#include "NexusEngine/Systems/World/WorldStorage.h"
#include "NexusEngine/Systems/World/World.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API WorldSystem : public System
	{
	public:
		NX_OBJECT(WorldSystem)

		inline static const NxFr::StringId MainWorldId = "World"_Sid;

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
		void DetachGameObject(NxFr::Handle<GameObject> Instance);

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep) override;

	private:
		class WorldManager* GetManager(NxFr::GUID WorldId);

		NxFr::Dictionary<NxFr::GUID, class WorldManager*> Managers;
	};
}
