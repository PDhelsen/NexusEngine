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

		NEXUS_ENGINE_API NexusEngineApplication() = default;
		NEXUS_ENGINE_API virtual ~NexusEngineApplication() = default;

	protected:
		NEXUS_ENGINE_API virtual void Initialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_ENGINE_API virtual void Shutdown(NxEn::Bootstrapper& Bootstrap) override;
	};
}
