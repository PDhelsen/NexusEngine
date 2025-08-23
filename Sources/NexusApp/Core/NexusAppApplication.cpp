#include "NexusApp/Core/NexusAppApplication.h"

#include "NexusApp/Systems/App/AppSystem.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	NexusAppApplication::NexusAppApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
		NxEn::SystemManager& Systems = GetSystems();

		Systems.CreateSystem<AppSystem>();

		if (!IsHeadless())
		{
			NxEn::WindowSystem* Window = Systems.GetSystem<NxEn::WindowSystem>();
			Window->SetWindowMode(NxEn::Window::Mode::FullScreen);
			Window->SetCursorMode(NxEn::Cursor::Mode::Captured);
		}
	}

	void NexusAppApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<AppSystem>();
	}

	void NexusAppApplication::OnShutdown()
	{
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<AppSystem>();

		NexusEngineApplication::OnShutdown();
	}

	void NexusAppApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
		NxEn::Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<AppSystem>(NxEn::Ticker::TickBucket::Engine);
	}
}
