#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	NX_APPLICATION_DECLARATION(NX_ENGINE_API, ::NxEn::NexusEngineApplication)

	class NX_ENGINE_API NexusEngineApplication : public Application
	{
	public:
		NexusEngineApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusEngineApplication();

		Input::Schema* GetInputsEngine() { return Inputs; }
		bool IsHeadless() const { return Headless; }

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnRun() override;

	private:
		Input::Schema* Inputs;
		bool Headless;
	};
}
