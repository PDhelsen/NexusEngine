#include "NexusEditor/Systems/Editor/EditorSystem.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEd
{
	const static NxFr::StringId InputSchemaId = NxFr::StringId("Editor");

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
		RecordActions();
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
		NxEn::Application::GetSystem<NxEn::InputSystem>()->AddSchema(InputSchemaId, &InputSchema);
	}

	void EditorSystem::PopInputSchema()
	{
		NxEn::Application::GetSystem<NxEn::InputSystem>()->RemoveSchema(InputSchemaId);
	}

	void EditorSystem::RecordActions()
	{
		NxEn::GUI::Menu& Menu = Window->GetMenu();
		NxFr::Dictionary<NxFr::StringId, NxEn::Input::Action>& Inputs = InputSchema.GetMapping();

		NxFr::Delegate<void()> InfoCallback = []() { NxEn::GUISystem::GetPanel<NxEn::ProjectPanel>()->ShowWithTarget(true, &NxEn::Application::GetInstance()->GetProject()); };
		NxFr::StringView InfoPath = "File/Project";
		Menu.AddMenuItem(InfoCallback, InfoPath);

		NxFr::Delegate<void()> SaveCallback = { this, &EditorSystem::Save };
		NxFr::StringView SavePath = "File/Save";
		Menu.AddMenuItem(SaveCallback, SavePath);
		Inputs.Append(NxFr::StringId(SavePath), NxEn::Input::Action(NxEn::Input::Button::S, NxEn::Input::State::Released, NxEn::Input::Modifier::Control, SaveCallback));
	}
}
