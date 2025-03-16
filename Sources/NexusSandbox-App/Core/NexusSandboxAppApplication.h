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

	protected:
		NEXUS_SANDBOX_APP_API virtual void OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_SANDBOX_APP_API virtual void OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_SANDBOX_APP_API virtual void OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems) override;
	};
}
