#pragma once

#include "NexusSandbox-App/Core/NexusSandboxAppCore.h"
#include "NexusApp/Core/NexusAppApplication.h"

namespace NxSA
{
	NX_APPLICATION_DECLARATION(NX_SANDBOX_APP_API, ::NxSA::NexusSandboxAppApplication)

	class NX_SANDBOX_APP_API NexusSandboxAppApplication : public NxAp::NexusAppApplication
	{
	public:
		NexusSandboxAppApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusSandboxAppApplication();

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnExecute() override;
	};
}
