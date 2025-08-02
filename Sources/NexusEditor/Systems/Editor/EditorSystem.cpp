#include "NexusEditor/Systems/Editor/EditorSystem.h"

namespace NxEd
{
	const static NxFr::StringId InputSchemaId = NxFr::StringId("Editor");

	const static NxEn::Command CmdEditorSave = NxEn::Command::Create("Editor.Save"_Sid, "Save project", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditorSystem>()->Save();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(EditorSystem)

	EditorSystem::EditorSystem()
		: OnSave(), InputSchema()
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

		PushInputSchema();
	}

	void EditorSystem::OnShutdown()
	{
		PopInputSchema();

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
}
