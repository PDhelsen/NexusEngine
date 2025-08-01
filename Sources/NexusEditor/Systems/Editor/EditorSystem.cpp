#include "NexusEditor/Systems/Editor/EditorSystem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(EditorSystem)

	EditorSystem::EditorSystem()
	{
	}

	EditorSystem::~EditorSystem()
	{
	}

	void EditorSystem::OnInitialize()
	{
		System::OnInitialize();
	}

	void EditorSystem::OnShutdown()
	{
		System::OnShutdown();
	}

	void EditorSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
	}
}
