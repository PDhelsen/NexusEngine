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

		NEXUS_EDITOR_API void LoadLayout(NxFr::StringView Name) const;
		NEXUS_EDITOR_API void SaveLayout() const;
		NEXUS_EDITOR_API void SaveLayout(NxFr::StringView Name) const;

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnTick(float TimeStep) override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

		void RecordPanels(NxEn::GUI::Menu& Menu, NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>>& Panels) const;

		void RecordLayouts(NxEn::GUI::Menu& Menu);
		void LoadLayout(const NxFr::Path& Path, NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>>& Panels) const;
		void SaveLayout(const NxFr::Path& Path, NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>>& Panels) const;
		NxFr::Path GetLayoutPath(NxFr::StringView Name = "") const;

	private:
		ImGuiWindowFlags GuiFlags;
		NxEn::GUI::Style Style;
	};
}
