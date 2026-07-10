#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/World/WorldInfo.h"
#include "NexusEngine/Systems/World/WorldStorage.h"

namespace NxEn
{
	class WorldManager
	{
	public:
		WorldManager();
		~WorldManager();

		void Tick(float TimeStep);
		void Reserve(NxFr::StringId Type, uint64 Size);

	private:
		NxFr::Handle<Object> AllocateStorage(WorldStorage* Storage, NxFr::GUID Id);
		void FreeStorage(WorldStorage* Storage, NxFr::Handle<Object> Instance);
		void UpdateStorage(WorldStorage* Storage, uint64 Index);
		void ResizeStorage(WorldStorage* Storage, uint64 Size);
		WorldStorage* GetStorage(NxFr::StringId Type);

		NxFr::Dictionary<NxFr::StringId, WorldStorage*> Storages;
		NxFr::Dictionary<NxFr::GUID, WorldObject> Objects;
		HandleManager Handles;
	};
}

