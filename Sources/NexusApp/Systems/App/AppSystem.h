#pragma once

#include "NexusApp/Core/NexusAppCore.h"

namespace NxAp
{
	class AppSystem : public NxEn::System
	{
	public:
		NX_OBJECT_DECLARATION(NX_APP_API, AppSystem)

		NX_APP_API AppSystem();
		NX_APP_API ~AppSystem();

		NX_APP_API NxEn::Input::Schema& GetInputsSchema() { return InputSchema; }
		NX_APP_API NxEn::GUI::Window& GetWindow() { return *Window; }

	protected:
		NX_APP_API void OnInitialize() override;
		NX_APP_API void OnShutdown() override;
		NX_APP_API void OnTick(float TimeStep = 0.0f) override;

		NX_APP_API void ShowWindow();

	private:
		NxEn::Input::Schema InputSchema;
		NxEn::GUI::Window* Window;
	};
}
