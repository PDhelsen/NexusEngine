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

		NEXUS_APP_API NexusAppApplication() = default;
		NEXUS_APP_API virtual ~NexusAppApplication() = default;

	protected:
		NEXUS_APP_API virtual void Initialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_APP_API virtual void Shutdown(NxEn::Bootstrapper& Bootstrap) override;
	};
}
