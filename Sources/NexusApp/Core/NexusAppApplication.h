#pragma once

#include "NexusApp/Core/NexusAppCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxAp
{
	NX_APPLICATION_DECLARATION(NX_APP_API, ::NxAp::NexusAppApplication)

	class NX_APP_API NexusAppApplication : public NxEn::NexusEngineApplication
	{
	public:
		NexusAppApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusAppApplication();

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnRun() override;
	};
}
