#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/World/WorldInfo.h"
#include "NexusEngine/Systems/World/WorldStorage.h"

namespace NxEn
{
	class NX_ENGINE_API WorldSystem : public System
	{
	public:
		NX_OBJECT(WorldSystem)

		WorldSystem();
		~WorldSystem();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep) override;

	private:
		class WorldManager* Manager;
	};
}
