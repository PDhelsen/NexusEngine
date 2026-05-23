#include "NexusEditor/Core/NexusEditorApplication.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"
#include "NexusEngine/Systems/Resources/Resources/Image.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"


namespace NxEd
{
	static NxEn::SettingMap<NxFr::String>* SettingShortcuts = NxEn::SettingMap<NxFr::String>::Create("Editor", "Shortcuts", {});

	const static NxEn::Command CmdEditorSave = NxEn::Command::Create("Editor.Save"_Sid, "Save project", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->SaveAll();
	}));

	const static NxEn::GUI::Menu::Item MenuItemSave = NxEn::GUI::Menu::Item::Create("File/Save", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Editor.Save");
	}), 1);

	NX_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo), OnSave(), Inputs(), Browser(), Hierarchy(), Stages()
	{
		NxEn::SystemManager& Systems = GetSystems();
		Systems.CreateSystem<EditSystem>();

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

		Systems.DestroySystem<EditSystem>();
	}

	void NexusEditorApplication::SaveAll()
	{
		NX_INSTUMENT_FUNCTION();

		OnSave.Invoke();
		NX_LOG(Info, Application, "Saved");
	}

	void NexusEditorApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::SystemManager& Systems = GetSystems();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<EditSystem>();
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Assets & Worlds", [&]()
		{
			Browser = new AssetsBrowser();
			Hierarchy = new HierarchyManager();
			Stages = new StageManager();
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Set Icon", []()
		{
			auto* Icon = Application::GetSystem<NxEn::ResourcesSystem>()->Load<NxEn::Image>("Logo_Small.png");
			Application::GetSystem<NxEn::WindowSystem>()->SetWindowIcon(Icon);
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Load Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->LoadLayout();
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Show Window", []()
		{
			Application::GetInstance<NexusEditorApplication>()->GetWindow().Show();
		});

		Systems.GetSystem<NxEn::InputSystem>()->AddSchema("Editor"_Sid, &Inputs);
		Systems.GetSystem<NxEn::SettingsSystem>()->GetOnChange() += { this, &NexusEditorApplication::ApplySettings };
	}

	void NexusEditorApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
		NxEn::SystemManager& Systems = GetSystems();
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<EditSystem>();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Save Layout", []()
		{
			Application::GetSystem<NxEn::GUISystem>()->SaveLayout();
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Hide Window", []()
		{
			Application::GetInstance<NexusEditorApplication>()->GetWindow().Hide();
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Set Icon", []()
		{
			Application::GetSystem<NxEn::ResourcesSystem>()->Unload("Logo_Small.png");
			Application::GetSystem<NxEn::WindowSystem>()->SetWindowIcon(nullptr);
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Assets & Worlds", [&]()
		{
			delete Stages;
			delete Hierarchy;
			delete Browser;
		});

		Systems.GetSystem<NxEn::InputSystem>()->RemoveSchema("Editor"_Sid);
		Systems.GetSystem<NxEn::SettingsSystem>()->GetOnChange() -= { this, &NexusEditorApplication::ApplySettings };
	}

	void NexusEditorApplication::OnRun()
	{
		NexusEngineApplication::OnRun();
		NxEn::Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<EditSystem>(NxEn::Ticker::TickBucket::Engine);
		Ticks.AppendTick(NxEn::Ticker::TickBucket::Engine, "Stages", [&]() { Stages->DestroyDisableStage(); });
	}

	void NexusEditorApplication::ApplySettings()
	{
		auto& Shortcuts = SettingShortcuts->GetValue();
		auto& Mapping = Inputs.GetMapping();

		for (auto& [Command, Shortcut] : Shortcuts)
		{
			if (Shortcut.IsEmpty())
			{
				continue;
			}

			NxFr::List<NxFr::StringView> Keys = NxFr::StringUtility::SplitAll(Shortcut, "+");

			NxEn::Input::State State = NxEn::Input::State::Released;
			NxEn::Input::Button Button = NxFr::StringUtility::FromString<NxEn::Input::Button>(Keys.Last());
			NxEn::Input::Modifier Modifiers = NxEn::Input::Modifier::None;
			if (Keys.GetCount() > 1)
			{
				for (uint64 Index = 0; Index < Keys.GetCount() - 1; ++Index)
				{
					Modifiers |= NxFr::StringUtility::FromString<NxEn::Input::Modifier>(Keys[Index]);
				}
			}

			NxEn::Input::Action Action(Button, State, Modifiers, [=]() { NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute(Command); });
			Mapping.AppendOrAssign(NxFr::StringId(Command), Action);
		}
	}
}
