#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	class WorldSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, WorldSystem)

		inline static const NxFr::StringId MainId = "Main"_Sid;

		inline static const NxFr::StringId AppendedId = "Added"_Sid;
		inline static const NxFr::StringId RemovedId = "Removed"_Sid;
		inline static const NxFr::StringId ChangedId = "Changed"_Sid;

		NEXUS_ENGINE_API World* CreateWorld(NxFr::StringId Id);
		NEXUS_ENGINE_API World* GetWorld(NxFr::StringId Id = MainId);
		NEXUS_ENGINE_API NxFr::Array<NxFr::StringId> GetWorlds();
		NEXUS_ENGINE_API void DestroyWorld(NxFr::StringId Id);

		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::StringId>& GetOnWorldEvent() { return OnWorldEvent; }
		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::StringId, NxFr::GUID>& GetOnGameObjectEvent() { return OnGameObjectEvent; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Event<NxFr::StringId, NxFr::StringId> OnWorldEvent;
		NxFr::Event<NxFr::StringId, NxFr::StringId, NxFr::GUID> OnGameObjectEvent;

		NxFr::Dictionary<NxFr::StringId, World*> Worlds;
	};
}
