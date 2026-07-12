#include "NexusEditor/Core/NexusEditorApplication.h"

#include "NexusEngine/Systems/Resources/Resources/Image.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"

namespace NxEd
{
	static NxEn::SettingMap<NxFr::String>* SettingShortcuts = NxEn::SettingMap<NxFr::String>::Create("Editor", "Shortcuts", {});

	static NxEn::Command* CmdSave = NxEn::Command::Create("Editor.Save"_Sid, "Save project", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->SaveAll();
	}));

	static NxEn::GUI::Menu::Item MenuItemSave = NxEn::GUI::Menu::Item::Create("File/Save", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Editor.Save");
	}), 1);

	NX_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo), OnSave(), Inputs(nullptr), Browser(nullptr), Hierarchy(nullptr)/*, Stages(nullptr)*/
	{
		NxEn::SystemManager& Systems = GetSystems();
		Systems.CreateSystem<EditSystem>();
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
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect event HID - Editor", [&]()
		{
			Inputs = new NxEn::Input::Schema();
			GetSystem<NxEn::InputSystem>()->AddSchema("Editor"_Sid, Inputs);
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect event Shortcuts", [&]()
		{
			GetSystem<NxEn::SettingsSystem>()->GetOnChange() += { this, & NexusEditorApplication::ApplySettings };
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect event Save", [&]()
		{
			OnSave += []() { GetSystem<NxEn::GUISystem>()->SaveLayout(); };
			OnSave += []() { GetSystem<NxEn::SettingsSystem>()->SaveSettings(); };
			OnSave += []() { GetSystem<NxEn::AssetsSystem>()->SaveDirty(); };
			//OnSave += []() { GetSystem<NxEn::WorldSystem>()->SaveScenes(); };
		});

		Bootstrap.AppendSystem<EditSystem>();

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Create Assets & Worlds Managers", [&]()
		{
			Browser = new AssetsBrowser();
			Hierarchy = new HierarchyManager();
			//Stages = new StageManager();
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Set Icon", []()
		{
			NxEn::Image* Icon = GetSystem<NxEn::ResourcesSystem>()->Load<NxEn::Image>("Logo_Small.png");
			GetSystem<NxEn::WindowSystem>()->SetWindowIcon(Icon);
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Show Window", [&]()
		{
			GetInstance<NexusEditorApplication>()->GetWindow()->Show();
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Load Layout", []()
		{
			GetSystem<NxEn::GUISystem>()->LoadLayout();
		});
	}

	void NexusEditorApplication::OnShutdown()
	{
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Save Layout", []()
		{
			GetSystem<NxEn::GUISystem>()->SaveLayout();
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Hide Window", []()
		{
			GetInstance<NexusEditorApplication>()->GetWindow()->Hide();
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Set Icon", []()
		{
			GetSystem<NxEn::WindowSystem>()->SetWindowIcon(nullptr);
			GetSystem<NxEn::ResourcesSystem>()->Unload("Logo_Small.png");
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Destroy Assets & Worlds Managers", [&]()
		{
			//delete Stages;
			delete Hierarchy;
			delete Browser;
		});

		Unbootstrap.AppendSystem<EditSystem>();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Disconnect event HID - Editor", [&]()
		{
			GetSystem<NxEn::InputSystem>()->RemoveSchema("Editor"_Sid);
			delete Inputs;
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Disconnect event Shortcuts", [&]()
		{
			GetSystem<NxEn::SettingsSystem>()->GetOnChange() -= { this, & NexusEditorApplication::ApplySettings };
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Disconnect event Save", [&]()
		{
			OnSave -= []() { GetSystem<NxEn::GUISystem>()->SaveLayout(); };
			OnSave -= []() { GetSystem<NxEn::SettingsSystem>()->SaveSettings(); };
			OnSave -= []() { GetSystem<NxEn::AssetsSystem>()->SaveDirty(); };
			//OnSave -= []() { GetSystem<NxEn::WorldSystem>()->SaveScenes(); };
		});

		NexusEngineApplication::OnShutdown();
	}

	void NexusEditorApplication::OnRun()
	{
		NexusEngineApplication::OnRun();
		NxEn::Ticker& Ticks = GetTicker();

		//Ticks.AppendTick(NxEn::Ticker::TickBucket::Engine, "Stages", [&]() { Stages->DestroyDisableStage(); });
	}

	void NexusEditorApplication::ApplySettings()
	{
		auto& Shortcuts = SettingShortcuts->GetValue();

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
			Inputs->Mapping.AppendOrAssign(NxFr::StringId(Command), Action);
		}
	}
}
