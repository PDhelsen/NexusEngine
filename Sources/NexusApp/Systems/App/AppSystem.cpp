#include "NexusApp/Systems/App/AppSystem.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxAp
{
	NX_OBJECT_IMPLEMENTATION(AppSystem)

	AppSystem::AppSystem()
		: InputSchema(), Window(NxEn::GUISystem::GetWindow())
	{
	}

	AppSystem::~AppSystem()
	{
	}

	void AppSystem::OnInitialize()
	{
		System::OnInitialize();

		NxEn::Application::GetSystem<NxEn::InputSystem>()->AddSchema("App"_Sid, &InputSchema);
		InputSchema.GetMapping().Append("Window"_Sid, NxEn::Input::Action(NxEn::Input::Button::Equal, NxEn::Input::State::Released, NxEn::Input::Modifier::None, { this, &AppSystem::ShowWindow }));
	}

	void AppSystem::OnShutdown()
	{
		NxEn::Application::GetSystem<NxEn::InputSystem>()->RemoveSchema("App"_Sid);

		System::OnShutdown();
	}

	void AppSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
	}

	void AppSystem::ShowWindow()
	{
		bool State = !Window->IsEnabled();
		Window->SetEnabled(State);

		NxEn::ConsolePanel* Console = NxEn::GUISystem::GetPanel<NxEn::ConsolePanel>();
		Console->SetDock(NxEn::GUISystem::GetWindow()->GetImGuiId());
		Console->SetEnabled(State);
	}
}
