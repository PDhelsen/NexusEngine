#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	NX_APPLICATION_DECLARATION(NX_ENGINE_API, ::NxEn::NexusEngineApplication)

	class NX_ENGINE_API NexusEngineApplication : public Application
	{
	public:
		NexusEngineApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusEngineApplication();

		Input::Schema& GetInputSchema() { return InputSchema; }
		bool IsHeadless() const { return Headless; }

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnRun() override;

		void ParseCommands();

	private:
		Input::Schema InputSchema;
		bool Headless;
	};
}
