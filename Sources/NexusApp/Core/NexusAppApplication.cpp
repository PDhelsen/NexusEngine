#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	NexusAppApplication::NexusAppApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
	}

	void NexusAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnInitialize(Bootstrap, Systems);

		if (!IsHeadless())
		{
			NxEn::WindowSystem* Window = Systems.GetSystem<NxEn::WindowSystem>();
			Window->SetWindowMode(NxEn::Window::Mode::FullScreen);
		}
	}

	void NexusAppApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnShutdown(Unbootstrap, Systems);
	}

	void NexusAppApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnExecute(Ticks, Systems);
	}
}
