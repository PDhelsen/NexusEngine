#pragma once

#include "NexusApp/Core/NexusAppCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_APP_API, ::NxAp::NexusAppApplication)

	class NexusAppApplication : public NxEn::NexusEngineApplication
	{
	public:
		static NexusAppApplication* GetInstance() { return (NexusAppApplication*)Application::GetInstance(); }

	protected:
		NEXUS_APP_API virtual void OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_APP_API virtual void OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_APP_API virtual void OnExecute() override;
	};
}
