#include "NexusEditor/Core/NexusEditorApplication.h"

#include "NexusEditor/Systems/Editor/EditorSystem.h"

#include "NexusEditor/Systems/World/Scene/ScenesManager.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
		NxEn::SystemManager& Systems = GetSystems();

		Systems.CreateSystem<EditorSystem>();

		NxFr::Event<>& OnSave = Systems.GetSystem<EditorSystem>()->GetOnSave();
		OnSave += [](){ Application::GetSystem<NxEn::GUISystem>()->SaveLayout(); };
		OnSave += [](){ Application::GetSystem<NxEn::SettingsSystem>()->SaveSettings(); };
		OnSave += [](){ Application::GetSystem<NxEn::AssetsSystem>()->Save(); };
		OnSave += [](){ Application::GetSystem<NxEn::WorldSystem>()->SaveScenes(); };

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
	}

	void NexusEditorApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<EditorSystem>();

		if (!IsHeadless())
		{
			Bootstrap.AppendDependency<EditorSystem, NxEn::GUISystem>();
		}

		Bootstrap.AppendStep(NxEn::Bootstrapper::StepBucket::AfterSystem, "Load Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->LoadLayout();
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::StepBucket::AfterSystem, "Refresh scenes list", []()
		{
			ScenesManager::Refresh();
		});
	}

	void NexusEditorApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<EditorSystem>();

		if (!IsHeadless())
		{
			Unbootstrap.AppendDependency<NxEn::GUISystem, EditorSystem>();
		}

		Unbootstrap.AppendStep(NxEn::Bootstrapper::StepBucket::BeforeSystem, "Save Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->SaveLayout();
		});
	}

	void NexusEditorApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
		NxEn::Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<EditorSystem>(NxEn::Ticker::TickBucket::Engine);
	}
}
