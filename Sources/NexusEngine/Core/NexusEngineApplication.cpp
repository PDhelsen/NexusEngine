#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEngine/Misc/DebugManager.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	void NexusEngineApplication::OnInitialize(Bootstrapper& Bootstrap)
	{
		Bootstrap.AddStep(&NxFr::Paths::CreateFrameworkFolders, "Generate Folders");
		Bootstrap.AddStep(&DebugManager::Initialize, "Initialize Debug Manager");

		Bootstrap.AddSystem<System>();
	}

	void NexusEngineApplication::OnShutdown(Bootstrapper& Unbootstrap)
	{
		Unbootstrap.AddStep(&DebugManager::Shutdown, "Shutdown Debug Manager");

		Unbootstrap.AddSystem<System>();
	}

	void NexusEngineApplication::OnExecute(Ticker& Ticks)
	{
		Ticks.AddSystem<System>(Ticker::Bucket::Engine);
	}
}
