#include "NexusApp/Core/NexusAppApplication.h"

#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxAp
{
	NX_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	NexusAppApplication::NexusAppApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo), Inputs(nullptr)
	{
	}

	NexusAppApplication::~NexusAppApplication()
	{
	}

	void NexusAppApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "HID - App", [&]()
		{
			Inputs = new NxEn::Input::Schema();
			Inputs->GetMapping().Append("Window"_Sid,
				NxEn::Input::Action(
					NxEn::Input::Button::Equal,
					NxEn::Input::State::Released,
					NxEn::Input::Modifier::None,
					{ this, &NexusAppApplication::ShowWindow }
				)
			);
			GetSystem<NxEn::InputSystem>()->AddSchema("App"_Sid, Inputs);

			if (!IsHeadless())
			{
				NxEn::WindowSystem* Window = GetSystem<NxEn::WindowSystem>();
				Window->SetWindowMode(NxEn::Window::Mode::FullScreen);
				Window->SetCursorMode(NxEn::Cursor::Mode::Captured);
			}
		});
	}

	void NexusAppApplication::OnShutdown()
	{
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "HID - App", [&]()
		{
			GetSystem<NxEn::InputSystem>()->RemoveSchema("App"_Sid);
			delete Inputs;
		});

		NexusEngineApplication::OnShutdown();
	}

	void NexusAppApplication::OnRun()
	{
		NexusEngineApplication::OnRun();
	}

	void NexusAppApplication::ShowWindow()
	{
		NxEn::GUI::Window* Window = GetWindow();

		bool State = !Window->IsEnabled();
		Window->SetEnabled(State);

		NxEn::ConsolePanel* Console = NxEn::GUISystem::GetPanel<NxEn::ConsolePanel>();
		Console->SetDock(Window->GetImGuiId());
		Console->SetEnabled(State);
	}
}
