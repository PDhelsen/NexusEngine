#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_ENGINE_API, ::NxEn::NexusEngineApplication)

	class NexusEngineApplication : public Application
	{
	public:
		static NexusEngineApplication* GetInstance() { return (NexusEngineApplication*)Application::GetInstance(); }

		NEXUS_ENGINE_API NexusEngineApplication(const NxEn::Project& ProjectInfo);
		NEXUS_ENGINE_API virtual ~NexusEngineApplication() = default;

		NEXUS_ENGINE_API bool IsHeadless() const { return Headless; }

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize() override;
		NEXUS_ENGINE_API virtual void OnShutdown() override;
		NEXUS_ENGINE_API virtual void OnExecute() override;

	private:
		bool Headless;
	};
}
