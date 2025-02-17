#pragma once

#include "NexusSandbox-App/Core/NexusSandboxAppCore.h"
#include "NexusApp/Core/NexusAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_SANDBOX_APP_API, ::NxSA::NexusSandboxAppApplication)

	class NexusSandboxAppApplication : public NxAp::NexusAppApplication
	{
	public:
		static NexusSandboxAppApplication* GetInstance() { return (NexusSandboxAppApplication*)Application::GetInstance(); }

		NEXUS_SANDBOX_APP_API NexusSandboxAppApplication();
		NEXUS_SANDBOX_APP_API virtual ~NexusSandboxAppApplication();
		NEXUS_SANDBOX_APP_API virtual void Run() override;
	};
}
