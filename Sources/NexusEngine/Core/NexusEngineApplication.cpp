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
		Bootstrap.AppendStep(&NxFr::Paths::CreateFrameworkFolders, "Generate Folders");
		Bootstrap.AppendStep(&DebugManager::Initialize, "Initialize Debug Manager");
		Bootstrap.AppendStep(&TimeManager::Initialize, "Initialize Time Manager");

		Bootstrap.AppendSystem(Systems.CreateSystem<System>());
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		Unbootstrap.AppendStep(NxFr::Delegate<void()>(&Systems, &SystemManager::ClearSystems), "Clear Systems");
		Unbootstrap.AppendStep(&TimeManager::Shutdown, "Shutdown Time Manager");
		Unbootstrap.AppendStep(&DebugManager::Shutdown, "Shutdown Debug Manager");

		Unbootstrap.AppendSystem(Systems.GetSystem<System>());
	}

	void NexusEngineApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		Ticks.AppendTickOnceCallback([]() { NEXUS_LOG(Info, Default, "Tick Once Callback"); });

		Ticks.AppendSystem(Systems.GetSystem<System>(), NxEn::Ticker::TickBucket::Engine, 10.0f, true);
	}
}
