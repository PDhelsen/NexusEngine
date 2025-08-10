#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/StylesPanel.h"

namespace NxEn
{
	static StylesPanel* Panel = GUI::Panel::Create<StylesPanel>();

	NEXUS_OBJECT_IMPLEMENTATION(StylesPanel)

	void StylesPanel::OnInitialize()
	{
		Panel::OnInitialize();

		Title = "Styles";
		GuiFlags = NxFr::Integer::SetFlag(GuiFlags, ImGuiWindowFlags_MenuBar, false);
	}

	void StylesPanel::OnGui(float TimeStep)
	{
		NxFr::Dictionary<NxFr::StringId, GUI::Style>* Styles = GetStyles();

		for (auto& [Id, Style] : *Styles)
		{
			NxFr::String Label("Button");

			ImGui::AlignTextToFramePadding(); 
			ImGui::Text(Label.C());
			ImGui::SameLine();

			GUI::Scope::Style S(Style);
			Label += NxFr::StringView("##") + Id.C();
			ImGui::Button(Label.C(), {100, 20});
		}
	}

	void* StylesPanel::FetchDefaultTarget() const
	{
		return &Application::GetInstance()->GetSystem<GUISystem>()->Styles;
	}
}
