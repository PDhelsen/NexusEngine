#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_ENGINE_API, ::NxEn::NexusEngineApplication)

	class NexusEngineApplication : public Application
	{
	public:
		static NexusEngineApplication* GetInstance() { return (NexusEngineApplication*)Application::GetInstance(); }

		NEXUS_ENGINE_API NexusEngineApplication();
		NEXUS_ENGINE_API virtual ~NexusEngineApplication();
		NEXUS_ENGINE_API virtual void Run() override;
	};
}
