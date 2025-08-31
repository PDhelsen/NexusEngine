#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

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

		NEXUS_EDITOR_API NxEn::Input::Schema& GetInputsSchema() { return InputSchema; }
		NEXUS_EDITOR_API NxEn::GUI::Window& GetWindow() { return *Window; }


	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Event<> OnSave;

		NxEn::Input::Schema InputSchema;
		NxEn::GUI::Window* Window;
	};
}
