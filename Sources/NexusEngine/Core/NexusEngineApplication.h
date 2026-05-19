#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	NX_APPLICATION_DECLARATION(NX_ENGINE_API, ::NxEn::NexusEngineApplication)

	class NexusEngineApplication : public Application
	{
	public:
		NX_ENGINE_API NexusEngineApplication(const NxEn::Project& ProjectInfo);
		NX_ENGINE_API virtual ~NexusEngineApplication();

		NX_ENGINE_API Input::Schema& GetInputSchema() { return InputSchema; }
		NX_ENGINE_API bool IsHeadless() const { return Headless; }

	protected:
		NX_ENGINE_API virtual void OnInitialize() override;
		NX_ENGINE_API virtual void OnShutdown() override;
		NX_ENGINE_API virtual void OnExecute() override;

		void ParseCommands();

	private:
		Input::Schema InputSchema;
		bool Headless;
	};
}
