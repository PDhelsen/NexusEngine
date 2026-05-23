#include "NexusApp/Core/NexusAppApplication.h"

#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxAp
{
	NX_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	NexusAppApplication::NexusAppApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo), Inputs()
	{
		NxEn::SystemManager& Systems = GetSystems();

		Inputs.GetMapping().Append("Window"_Sid, NxEn::Input::Action(NxEn::Input::Button::Equal, NxEn::Input::State::Released, NxEn::Input::Modifier::None, { this, &NexusAppApplication::ShowWindow }));
		Systems.GetSystem<NxEn::InputSystem>()->AddSchema("App"_Sid, &Inputs);
		if (!IsHeadless())
		{
			NxEn::WindowSystem* Window = Systems.GetSystem<NxEn::WindowSystem>();
			Window->SetWindowMode(NxEn::Window::Mode::FullScreen);
			Window->SetCursorMode(NxEn::Cursor::Mode::Captured);
		}
	}

	NexusAppApplication::~NexusAppApplication()
	{
		NxEn::SystemManager& Systems = GetSystems();

		Systems.GetSystem<NxEn::InputSystem>()->RemoveSchema("App"_Sid);
	}

	void NexusAppApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
	}

	void NexusAppApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
	}

	void NexusAppApplication::OnRun()
	{
		NexusEngineApplication::OnRun();
	}

	void NexusAppApplication::ShowWindow()
	{
		NxEn::GUI::Window& Window = GetWindow();

		bool State = !Window.IsEnabled();
		Window.SetEnabled(State);

		NxEn::ConsolePanel* Console = NxEn::GUISystem::GetPanel<NxEn::ConsolePanel>();
		Console->SetDock(Window.GetImGuiId());
		Console->SetEnabled(State);
	}
}
