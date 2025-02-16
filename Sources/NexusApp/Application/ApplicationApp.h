#pragma once

#include "NexusApp/Core/NexusAppCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxAp
{
	class ApplicationApp : public NxEn::Application
	{
	public:
		NEXUS_APP_API ApplicationApp();
		NEXUS_APP_API ~ApplicationApp();
		NEXUS_APP_API virtual void Run() override;
	};
}
