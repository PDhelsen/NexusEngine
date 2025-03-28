#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"

#include "NexusEngine/Misc/DebugManager.h"
#include "NexusEngine/Misc/Memory/MemoryManager.h"

namespace NxEn
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_ENGINE_API, ::NxEn::NexusEngineApplication)

	class NexusEngineApplication : public Application
	{
	public:
		static NexusEngineApplication* GetInstance() { return (NexusEngineApplication*)Application::GetInstance(); }

		DebugManager& GetDebug() { return *Debug; }
		MemoryManager& GetMemory() { return *Memory; }

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_ENGINE_API virtual void OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_ENGINE_API virtual void OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems) override;

	private:
		DebugManager* Debug = nullptr;
		MemoryManager* Memory = nullptr;
	};
}
