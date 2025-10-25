#include "NexusEditor/Systems/Editor/EditorSystem.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"

namespace NxEd
{
	static NxEn::SettingMap<NxFr::String>* SettingShortcuts = NxEn::SettingMap<NxFr::String>::Create("Editor", "Shortcuts", {});

	const static NxEn::Command CmdEditorSave = NxEn::Command::Create("Editor.Save"_Sid, "Save project", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditorSystem>()->Save();
	}));

	const static NxEn::GUI::Menu::Item MenuItemSave = NxEn::GUI::Menu::Item::Create("File/Save", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Editor.Save");
	}), "", 1);

	NEXUS_OBJECT_IMPLEMENTATION(EditorSystem)

	EditorSystem::EditorSystem()
		: OnSave(), InputSchema(), Window(NxEn::GUISystem::GetWindow())
	{
	}

	EditorSystem::~EditorSystem()
	{
	}

	void EditorSystem::Save()
	{
		NEXUS_PROFILE_FUNCTION();

		OnSave.Invoke();
		NEXUS_LOG(Info, System, "Saved");
	}

	void EditorSystem::OnInitialize()
	{
		System::OnInitialize();

		Window->Show();
		NxEn::Application::GetSystem<NxEn::InputSystem>()->AddSchema("Editor"_Sid, &InputSchema);
		NxEn::Application::GetSystem<NxEn::SettingsSystem>()->GetOnChange() += { this, &EditorSystem::ApplySettings };
	}

	void EditorSystem::OnShutdown()
	{
		Window->Hide();
		NxEn::Application::GetSystem<NxEn::InputSystem>()->RemoveSchema("Editor"_Sid);

		System::OnShutdown();
	}

	void EditorSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
	}

	void EditorSystem::ApplySettings()
	{
		auto& Shortcuts = SettingShortcuts->GetValue();
		auto& Mapping = InputSchema.GetMapping();

		for (auto& [Command, Shortcut] : Shortcuts)
		{
			if (Shortcut.IsEmpty())
			{
				continue;
			}

			NxFr::List<NxFr::StringView> Keys = NxFr::StringUtility::SplitAll(Shortcut, "+");

			NxEn::Input::State State = NxEn::Input::State::Released;
			NxEn::Input::Button Button = NxFr::StringUtility::FromString<NxEn::Input::Button>(Keys[Keys.GetCount() - 1]);
			NxEn::Input::Modifier Modifiers = NxEn::Input::Modifier::None;
			if (Keys.GetCount() > 1)
			{
				for (int64 Index = 0; Index < Keys.GetCount() - 1; ++Index)
				{
					Modifiers |= NxFr::StringUtility::FromString<NxEn::Input::Modifier>(Keys[Index]);
				}
			}

			NxEn::Input::Action Action(Button, State, Modifiers, [=]() { NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute(Command); });
			Mapping.AppendOrAssign(NxFr::StringId(Command), Action);
		}
	}
}
