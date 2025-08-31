#include "NexusEditor/Systems/Editor/EditorSystem.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEd
{
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
}
