#pragma once

#include "NexusApp/Core/NexusAppCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxAp
{
	NX_APPLICATION_DECLARATION(NX_APP_API, ::NxAp::NexusAppApplication)

	class NexusAppApplication : public NxEn::NexusEngineApplication
	{
	public:
		NX_APP_API NexusAppApplication(const NxEn::Project& ProjectInfo);
		NX_APP_API virtual ~NexusAppApplication();

	protected:
		NX_APP_API virtual void OnInitialize() override;
		NX_APP_API virtual void OnShutdown() override;
		NX_APP_API virtual void OnExecute() override;
	};
}
