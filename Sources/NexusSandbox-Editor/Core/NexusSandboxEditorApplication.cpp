#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	NexusSandboxEditorApplication::NexusSandboxEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEditorApplication(ProjectInfo)
	{
	}

	void NexusSandboxEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnInitialize(Bootstrap, Systems);
	}

	void NexusSandboxEditorApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnShutdown(Unbootstrap, Systems);
	}

	void NexusSandboxEditorApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnExecute(Ticks, Systems);

		Ticks.AppendTickCallback([]()
		{
			if (Application::GetInstance()->GetSystems().GetSystem<NxEn::InputSystem>()->GetButton(NxEn::Input::Button::Space) == NxEn::Input::State::Pressed)
			{
				NEXUS_LOG(Info, Default, "Pressed");
			}
		});
	}
}
