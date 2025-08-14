#include "NexusEditor/Systems/Editor/EditorSystem.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEd
{
	const static NxEn::GUI::Menu::Item MenuItemSave = NxEn::GUI::Menu::Item::Create("File/Save", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditorSystem>()->Save();
	}), "", 1);

	const static NxEn::Command CmdEditorSave = NxEn::Command::Create("Editor.Save"_Sid, "Save project", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditorSystem>()->Save();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(EditorSystem)

	EditorSystem::EditorSystem()
		: OnSave(), InputSchema(), Window(nullptr)
	{
	}

	EditorSystem::~EditorSystem()
	{
	}

	void EditorSystem::Save()
	{
		NEXUS_PROFILE_FUNCTION();

		OnSave.Invoke();
		NEXUS_LOG(Info, Default, "Saved");
	}

	void EditorSystem::OnInitialize()
	{
		System::OnInitialize();

		Window = NxEn::Object::Create<EditorWindow>();
		PushInputSchema();
	}

	void EditorSystem::OnShutdown()
	{
		PopInputSchema();
		Window = NxEn::Object::Destroy(Window);

		System::OnShutdown();
	}

	void EditorSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
	}

	void EditorSystem::PushInputSchema()
	{
		NxEn::Application::GetSystem<NxEn::InputSystem>()->AddSchema("Editor"_Sid, &InputSchema);
	}

	void EditorSystem::PopInputSchema()
	{
		NxEn::Application::GetSystem<NxEn::InputSystem>()->RemoveSchema("Editor"_Sid);
	}
}
