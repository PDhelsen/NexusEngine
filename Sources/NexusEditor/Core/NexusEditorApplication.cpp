#include "NexusEditor/Core/NexusEditorApplication.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/Editor/EditorSystem.h"

namespace NxEd
{
	NX_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo)
	{
		NxEn::SystemManager& Systems = GetSystems();

		Systems.CreateSystem<EditorSystem>();
		Systems.CreateSystem<EditSystem>();

		NxFr::Event<>& OnSave = Systems.GetSystem<EditorSystem>()->GetOnSave();
		OnSave += [](){ Application::GetSystem<NxEn::GUISystem>()->SaveLayout(); };
		OnSave += [](){ Application::GetSystem<NxEn::SettingsSystem>()->SaveSettings(); };
		OnSave += [](){ Application::GetSystem<NxEn::AssetsSystem>()->SaveDirty(); };
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
		Systems.DestroySystem<EditSystem>();
	}

	void NexusEditorApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<EditorSystem>().AppendDependency<EditorSystem, NxEn::AssetsSystem>().AppendDependency<EditorSystem, NxEn::WorldSystem>();
		Bootstrap.AppendSystem<EditSystem>().AppendDependency<EditSystem, EditorSystem>();

		if (!IsHeadless())
		{
			Bootstrap.AppendDependency<EditorSystem, NxEn::GUISystem>();
		}

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Load Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->LoadLayout();
		});
	}

	void NexusEditorApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<EditSystem>();
		Unbootstrap.AppendSystem<EditorSystem>().AppendDependency<EditorSystem, EditSystem>();
		Unbootstrap.AppendDependency<NxEn::AssetsSystem, EditorSystem>();
		Unbootstrap.AppendDependency<NxEn::WorldSystem, EditorSystem>();

		if (!IsHeadless())
		{
			Unbootstrap.AppendDependency<NxEn::GUISystem, EditorSystem>();
		}

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Save Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->SaveLayout();
		});
	}

	void NexusEditorApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
		NxEn::Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<EditorSystem>(NxEn::Ticker::TickBucket::Engine);
		Ticks.AppendSystem<EditSystem>(NxEn::Ticker::TickBucket::Engine);
	}
}
