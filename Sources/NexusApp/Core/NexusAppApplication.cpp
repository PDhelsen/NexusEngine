#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	NexusAppApplication::NexusAppApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
		NxEn::SystemManager& Systems = GetSystems();

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
	}

	void NexusAppApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
	}

	void NexusAppApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
	}
}
