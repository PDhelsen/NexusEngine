#include "NexusEditor/Core/NexusEditorApplication.h"

#include "NexusEditor/Systems/Editor/EditorSystem.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
		NxEn::SystemManager& Systems = GetSystems();

		Systems.CreateSystem<EditorSystem>();
		Systems.CreateSystem<EditSystem>();

		if (!IsHeadless())
		{
			NxEn::WindowSystem* Window = Systems.GetSystem<NxEn::WindowSystem>();
			Window->SetWindowMode(NxEn::Window::Mode::Windowed);
			Window->SetCursorMode(NxEn::Cursor::Mode::Default);
		}
	}

	NexusEditorApplication::~NexusEditorApplication()
	{
		NxEn::SystemManager& Systems = GetSystems();

		Systems.DestroySystem<EditorSystem>();
		Systems.DestroySystem<EditSystem>();
	}

	void NexusEditorApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<EditorSystem>();
		Bootstrap.AppendSystem<EditSystem>().AppendDependency<EditSystem, EditorSystem>();

		if (!IsHeadless())
		{
			Bootstrap.AppendDependency<EditorSystem, NxEn::GUISystem>();
		}

		Bootstrap.AppendStep(NxEn::Bootstrapper::StepBucket::AfterSystem, "Load Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->LoadLayout();
		});
	}

	void NexusEditorApplication::OnShutdown()
	{
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<EditorSystem>();
		Unbootstrap.AppendSystem<EditSystem>();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::StepBucket::BeforeSystem, "Save Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->SaveLayout();
		});

		NexusEngineApplication::OnShutdown();
	}

	void NexusEditorApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
		NxEn::Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<EditorSystem>(NxEn::Ticker::TickBucket::Engine);
	}
}
