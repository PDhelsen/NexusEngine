#include "NexusApp/Systems/App/AppSystem.h"

namespace NxAp
{
	NEXUS_OBJECT_IMPLEMENTATION(AppSystem)

	AppSystem::AppSystem()
	{
	}

	AppSystem::~AppSystem()
	{
	}

	void AppSystem::OnInitialize()
	{
		System::OnInitialize();

		PushInputSchema();

		InputSchema.GetMapping().Append("Test"_Sid, NxEn::Input::Action(NxEn::Input::Button::Space, NxEn::Input::State::Released, NxEn::Input::Modifier::None, []()
			{
				NEXUS_LOG(Info, Default, "Test");
			}));
	}

	void AppSystem::OnShutdown()
	{
		PopInputSchema();

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
