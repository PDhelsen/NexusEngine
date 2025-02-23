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

		NEXUS_SANDBOX_APP_API NexusSandboxAppApplication() = default;
		NEXUS_SANDBOX_APP_API virtual ~NexusSandboxAppApplication() = default;

	protected:
		NEXUS_SANDBOX_APP_API virtual void Initialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_SANDBOX_APP_API virtual void Shutdown(NxEn::Bootstrapper& Bootstrap) override;
	};
}
