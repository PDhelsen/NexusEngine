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
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		NexusEngineApplication::OnInitialize();

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Input - App", [&]()
		{
			Inputs = new NxEn::Input::Schema();
			GetSystem<NxEn::InputSystem>()->AddSchema("App"_Sid, Inputs);
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect Window", [&]()
		{
			Inputs->Mapping.Append("Window"_Sid, NxEn::Input::Action(
				{ NxEn::Input::Button::Equal, NxEn::Input::State::Released, NxEn::Input::Modifier::None },
				{ this, &NexusAppApplication::ToggleWindow })
			);
		});

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Show Window", [&]()
		{
			GetSystem<NxEn::GUISystem>()->GetWindow()->Show();
		});
	}

	void NexusAppApplication::OnShutdown()
	{
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Hide Window", []()
		{
			GetSystem<NxEn::GUISystem>()->GetWindow()->Hide();
		});

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Input - App", [&]()
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

	void NexusAppApplication::ToggleWindow()
	{
		NxEn::GUISystem* GUI = NxEn::Application::GetSystem<NxEn::GUISystem>();
		NxEn::ConsolePanel* Console = GUI->GetPanel<NxEn::ConsolePanel>();
		NxEn::GUI::Window* Window = GUI->GetWindow();

		bool State = !Console->IsEnabled();
		Console->SetEnabled(State);
		Window->GetDock().DockElement(Console, ImGuiDir_None, 0.0f, true);
	}
}
