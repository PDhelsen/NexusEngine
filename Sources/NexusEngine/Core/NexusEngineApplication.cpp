#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	static void Tick()
	{
		// TEMP: Avoid looping too fast for now since the app is empty
		//NxFr::Platform::GetInstance()->Sleep(1);

		if (Application::GetInstance()->GetTime().GetFrameIndex() == 5)
		{
			Application::GetInstance()->GetTicker().AppendTickOnceCallback([]() { Application::GetInstance()->GetTime().SetMultiplier(0.1f); });
		}

		if (Application::GetInstance()->GetTime().GetFrameIndex() > 10)
		{
			Application::GetInstance()->Quit();
		}
	}

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		Application::OnInitialize(Bootstrap, Systems);

		Bootstrap.AppendSystem(Systems.CreateSystem<System>());
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		Unbootstrap.AppendSystem(Systems.GetSystem<System>());

		Application::OnShutdown(Unbootstrap, Systems);
	}

	void NexusEngineApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		Application::OnExecute(Ticks, Systems);

		Ticks.AppendTickCallback(&Tick);
		Ticks.AppendTickOnceCallback([]() { NEXUS_LOG(Info, Default, "Tick Once Callback"); });

		Ticks.AppendSystem(Systems.GetSystem<System>(), NxEn::Ticker::TickBucket::Engine);
	}
}
