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

		NEXUS_SANDBOX_APP_API NexusSandboxAppApplication(const NxEn::Project& ProjectInfo);
		NEXUS_SANDBOX_APP_API virtual ~NexusSandboxAppApplication() = default;

	protected:
		NEXUS_SANDBOX_APP_API virtual void OnInitialize() override;
		NEXUS_SANDBOX_APP_API virtual void OnShutdown() override;
		NEXUS_SANDBOX_APP_API virtual void OnExecute() override;
	};
}
