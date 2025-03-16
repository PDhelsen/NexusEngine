#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEngine/Misc/DebugManager.h"
#include "NexusEngine/Misc/TimeManager.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	static void Tick()
	{
		// TEMP: Avoid looping too fast for now since the app is empty
		//NxFr::Platform::GetInstance()->Sleep(1);

		if (TimeManager::GetInstance()->GetFrameIndex() == 5)
		{
			Application::GetInstance()->GetTicker().AppendTickOnceCallback([]() { TimeManager::GetInstance()->SetMultiplier(0.1f); });
		}

		if (TimeManager::GetInstance()->GetFrameIndex() > 10)
		{
			Application::GetInstance()->Quit();
		}
	}

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
		Ticks.AppendTickCallback(&Tick);
		Ticks.AppendTickOnceCallback([]() { NEXUS_LOG(Info, Default, "Tick Once Callback"); });

		Ticks.AppendSystem(Systems.GetSystem<System>(), NxEn::Ticker::TickBucket::Engine);
	}
}
