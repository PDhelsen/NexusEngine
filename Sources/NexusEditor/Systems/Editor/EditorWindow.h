#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class EditorWindow : public NxEn::GUI::Element
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, EditorWindow)

		NEXUS_EDITOR_API EditorWindow();
		NEXUS_EDITOR_API ~EditorWindow();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnTick(float TimeStep) override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		ImGuiWindowFlags GuiFlags;
		NxEn::GUI::Style Style;
	};
}
