#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/System.h"

namespace NxEd
{
	class EditorSystem : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, EditorSystem)

		NEXUS_EDITOR_API EditorSystem();
		NEXUS_EDITOR_API ~EditorSystem();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnTick(float TimeStep = 0.0f) override;
	};
}
