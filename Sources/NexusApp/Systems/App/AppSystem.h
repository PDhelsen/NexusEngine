#pragma once

#include "NexusApp/Core/NexusAppCore.h"

namespace NxAp
{
	class NX_APP_API AppSystem : public NxEn::System
	{
	public:
		NX_OBJECT_DECLARATION(AppSystem)

		AppSystem();
		~AppSystem();

		NxEn::Input::Schema& GetInputsSchema() { return InputSchema; }
		NxEn::GUI::Window& GetWindow() { return *Window; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

		void ShowWindow();

	private:
		NxEn::Input::Schema InputSchema;
		NxEn::GUI::Window* Window;
	};
}
