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

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_ENGINE_API virtual void OnShutdown(NxEn::Bootstrapper& Bootstrap) override;
	};
}
