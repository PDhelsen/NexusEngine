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
		SystemManager& Systems = GetSystems();

		Systems.CreateSystem<DebugSystem>();
		Systems.CreateSystem<MemorySystem>();
		Systems.CreateSystem<InputSystem>();
		if (!IsHeadless())
		{
			WindowSystem* Window = Systems.CreateSystem<WindowSystem>();
			Window->GetOnClose() += NxFr::Delegate<void()>(this, &Application::Quit);
		}
	}

	void NexusEngineApplication::OnInitialize()
	{
		Application::OnInitialize();
		Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendStep(Glfw::Initialize, "Glfw - Initialize");

		Bootstrap.AppendSystem<MemorySystem>();
		Bootstrap.AppendSystem<DebugSystem>();
		Bootstrap.AppendSystem<InputSystem>();
		if (!IsHeadless()) Bootstrap.AppendSystem<WindowSystem>().AppendDependency<InputSystem, WindowSystem>();
	}

	void NexusEngineApplication::OnShutdown()
	{
		Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<DebugSystem>();
		Unbootstrap.AppendSystem<MemorySystem>();
		Unbootstrap.AppendSystem<InputSystem>();
		if (!IsHeadless()) Unbootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, InputSystem>();

		Unbootstrap.AppendStep([]()
		{
			NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		}, "Application duration");
		Unbootstrap.AppendStep(Glfw::Shutdown, "Glfw - Shutdown");

		Application::OnShutdown();
	}

	void NexusEngineApplication::OnExecute()
	{
		Application::OnExecute();
		Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<InputSystem>(NxEn::Ticker::TickBucket::Input);
		if (!IsHeadless()) Ticks.AppendSystem<WindowSystem>(NxEn::Ticker::TickBucket::Output);
		Ticks.AppendSystem<MemorySystem>(NxEn::Ticker::TickBucket::Cleanup);
		Ticks.AppendSystem<DebugSystem>(NxEn::Ticker::TickBucket::Cleanup).AppendDependency<DebugSystem, MemorySystem>();

		Ticks.AppendTickCallback([]()
		{
			if (Application::GetInstance()->GetSystems().GetSystem<NxEn::InputSystem>()->GetButton(NxEn::Input::Button::Space) == NxEn::Input::State::Pressed)
			{
				NEXUS_LOG(Info, Default, "Pressed");
			}
		});
	}
}
