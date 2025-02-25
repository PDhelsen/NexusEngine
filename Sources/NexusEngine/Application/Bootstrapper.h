#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/System.h"

namespace NxEn
{
	class Bootstrapper
	{
		friend class Application;

	public:
		NEXUS_ENGINE_API Bootstrapper();
		NEXUS_ENGINE_API ~Bootstrapper();

		NEXUS_ENGINE_API void AddStep(NxFr::Delegate<void()> Step);
		NEXUS_ENGINE_API void AddSystem(NxFr::Delegate<System*()> System);

	private:
		void Boot(NxFr::Array<System*>& Systems);
		void Unboot(NxFr::Array<System*>& Systems);

		void ExecuteSteps();
		NxFr::Array<System*> CreateSystems();
		void DestroySystems(NxFr::Array<System*>& Systems);

		NxFr::Event<> Steps;
		NxFr::List<NxFr::Delegate<System*()>> Dependencies;
	};
}
