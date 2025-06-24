#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEngine/External/Glfw.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication(const NxEn::Project& ProjectInfo)
		: Application(ProjectInfo), Headless(NxFr::Arguments::HasFlag("Headless"))
	{
	}

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		Application::OnInitialize(Bootstrap, Systems);

		Bootstrap.AppendSystem(Systems.CreateSystem<DebugSystem>());
		Bootstrap.AppendSystem(Systems.CreateSystem<MemorySystem>());
		if (!IsHeadless())
		{
			WindowSystem* Window = Systems.CreateSystem<WindowSystem>();
			Window->OnClose += NxFr::Delegate<void()>(this, &Application::Quit);
			Bootstrap.AppendSystem(Window);
		}

		Bootstrap.AppendStep(Glfw::Initialize, "Glfw - Initialize");
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		Unbootstrap.AppendSystem(Systems.GetSystem<MemorySystem>());
		Unbootstrap.AppendSystem(Systems.GetSystem<DebugSystem>());
		if (!IsHeadless())
		{
			Unbootstrap.AppendSystem(Systems.GetSystem<WindowSystem>());
		}

		Unbootstrap.AppendStep([]()
		{
			NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		}, "Application duration");
		Unbootstrap.AppendStep(Glfw::Shutdown, "Glfw - Shutdown");

		Application::OnShutdown(Unbootstrap, Systems);
	}

	void NexusEngineApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		Application::OnExecute(Ticks, Systems);

		Ticks.AppendSystem(Systems.GetSystem<DebugSystem>(), NxEn::Ticker::TickBucket::Cleanup).AppendDependency<DebugSystem, MemorySystem>();
		Ticks.AppendSystem(Systems.GetSystem<MemorySystem>(), NxEn::Ticker::TickBucket::Cleanup);
		if (!IsHeadless())
		{
			Ticks.AppendSystem(Systems.GetSystem<WindowSystem>(), NxEn::Ticker::TickBucket::Output);
		}
	}
}
