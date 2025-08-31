#pragma once

#include "NexusApp/Core/NexusAppCore.h"

namespace NxAp
{
	class AppSystem : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_APP_API, AppSystem)

		NEXUS_APP_API AppSystem();
		NEXUS_APP_API ~AppSystem();

		NEXUS_APP_API NxEn::Input::Schema& GetInputsSchema() { return InputSchema; }
		NEXUS_APP_API NxEn::GUI::Window& GetWindow() { return *Window; }

	protected:
		NEXUS_APP_API void OnInitialize() override;
		NEXUS_APP_API void OnShutdown() override;
		NEXUS_APP_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_APP_API void ShowWindow();

	private:
		NxEn::Input::Schema InputSchema;
		NxEn::GUI::Window* Window;
	};
}
