#pragma once

#include "NexusSandbox-App/Core/NexusSandboxAppCore.h"
#include "NexusApp/Core/NexusAppApplication.h"

namespace NxSA
{
	NX_APPLICATION_DECLARATION(NX_SANDBOX_APP_API, ::NxSA::NexusSandboxAppApplication)

	class NexusSandboxAppApplication : public NxAp::NexusAppApplication
	{
	public:
		NX_SANDBOX_APP_API NexusSandboxAppApplication(const NxEn::Project& ProjectInfo);
		NX_SANDBOX_APP_API virtual ~NexusSandboxAppApplication();

	protected:
		NX_SANDBOX_APP_API virtual void OnInitialize() override;
		NX_SANDBOX_APP_API virtual void OnShutdown() override;
		NX_SANDBOX_APP_API virtual void OnExecute() override;
	};
}
