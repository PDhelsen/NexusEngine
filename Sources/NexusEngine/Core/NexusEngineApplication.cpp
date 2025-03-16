#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEngine/Misc/DebugManager.h"
#include "NexusEngine/Misc/TimeManager.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		Bootstrap.AddStep(&NxFr::Paths::CreateFrameworkFolders, "Generate Folders");
		Bootstrap.AddStep(&DebugManager::Initialize, "Initialize Debug Manager");
		Bootstrap.AddStep(&TimeManager::Initialize, "Initialize Time Manager");

		Bootstrap.AddSystem(Systems.CreateSystem<System>());
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		Unbootstrap.AddStep(NxFr::Delegate<void()>(&Systems, &SystemManager::ClearSystems), "Clear Systems");
		Unbootstrap.AddStep(&DebugManager::Shutdown, "Shutdown Debug Manager");
		Unbootstrap.AddStep(&TimeManager::Shutdown, "Shutdown Time Manager");

		Unbootstrap.AddSystem(Systems.GetSystem<System>());
	}

	void NexusEngineApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		Ticks.AddTickOnceCallback([]() { NEXUS_LOG(Info, Default, "Tick Once Callback"); });

		Ticks.AddSystem(Systems.GetSystem<System>(), NxEn::Ticker::TickBucket::Engine, 10.0f, true);
	}
}
