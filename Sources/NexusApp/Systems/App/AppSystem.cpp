#include "NexusApp/Systems/App/AppSystem.h"

namespace NxAp
{
	NEXUS_OBJECT_IMPLEMENTATION(AppSystem)

	AppSystem::AppSystem()
		: InputSchema(), Window(nullptr)
	{
	}

	AppSystem::~AppSystem()
	{
	}

	void AppSystem::OnInitialize()
	{
		System::OnInitialize();

		Window = NxEn::Object::Create<NxEn::GUI::Window>(false);
		PushInputSchema();

		InputSchema.GetMapping().Append("Window"_Sid, NxEn::Input::Action(NxEn::Input::Button::Equal, NxEn::Input::State::Released, NxEn::Input::Modifier::None, [=]()
		{
			Window->SetEnabled(!Window->IsEnabled());
		}));
	}

	void AppSystem::OnShutdown()
	{
		PopInputSchema();
		Window = NxEn::Object::Destroy(Window);

		System::OnShutdown();
	}

	void AppSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
	}

	void AppSystem::PushInputSchema()
	{
		NxEn::Application::GetSystem<NxEn::InputSystem>()->AddSchema("App"_Sid, &InputSchema);
	}

	void AppSystem::PopInputSchema()
	{
		NxEn::Application::GetSystem<NxEn::InputSystem>()->RemoveSchema("App"_Sid);
	}
}
