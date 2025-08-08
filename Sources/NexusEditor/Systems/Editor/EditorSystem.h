#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Editor/EditorWindow.h"

namespace NxEd
{
	class EditorSystem : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, EditorSystem)

		NEXUS_EDITOR_API EditorSystem();
		NEXUS_EDITOR_API ~EditorSystem();

		NEXUS_EDITOR_API void Save();

		NEXUS_EDITOR_API NxFr::Event<>& GetOnSave() { return OnSave; }

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_EDITOR_API void PushInputSchema();
		NEXUS_EDITOR_API void PopInputSchema();

	private:
		NxFr::Event<> OnSave;

		NxEn::Input::Schema InputSchema;
		EditorWindow* Window;
	};
}
