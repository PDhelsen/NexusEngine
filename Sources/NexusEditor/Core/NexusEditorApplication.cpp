#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
	}

	void NexusEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnInitialize(Bootstrap, Systems);

		if (!IsHeadless())
		{
			NxEn::WindowSystem* Window = Systems.GetSystem<NxEn::WindowSystem>();
			Window->SetWindowMode(NxEn::Window::Mode::Windowed);
			Window->SetCursorMode(NxEn::Cursor::Mode::Default);
		}
	}

	void NexusEditorApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnShutdown(Unbootstrap, Systems);
	}

	void NexusEditorApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnExecute(Ticks, Systems);
	}
}
