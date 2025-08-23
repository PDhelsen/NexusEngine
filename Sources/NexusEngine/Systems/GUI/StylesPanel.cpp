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
	}

	void StylesPanel::OnEnable()
	{
		Panel::OnEnable();

		Styles = &Application::GetInstance()->GetSystem<GUISystem>()->Styles;
	}

	void StylesPanel::OnGui(float TimeStep)
	{
		for (auto& [Id, Style] : *Styles)
		{
			GUI::Scope::Style S(Style);

			NxFr::String Label("Button");

			ImGui::AlignTextToFramePadding(); 
			ImGui::Text(Label.C());
			ImGui::SameLine();

			Label += NxFr::StringView("##") + Id.C();
			ImGui::Button(Label.C(), {100, 20});
		}
	}
}
