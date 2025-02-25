#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Bootstrapper
	{
		friend class Application;

	public:
		NEXUS_ENGINE_API Bootstrapper();
		NEXUS_ENGINE_API ~Bootstrapper();

		NEXUS_ENGINE_API void AddStep(NxFr::Delegate<void()> Step);

		uint64 GetStepsCount() const { return Steps.GetCount(); }

	private:
		void Boot();
		void Unboot();

		void ExecuteSteps();

		NxFr::Event<> Steps;
	};
}
