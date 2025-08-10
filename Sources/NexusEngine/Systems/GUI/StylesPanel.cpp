#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/StylesPanel.h"

namespace NxEn
{
	static StylesPanel* Panel = GUI::Panel::Create<StylesPanel>();

	NEXUS_OBJECT_IMPLEMENTATION(StylesPanel)

	void StylesPanel::SetTarget(void* Instance)
	{
		Panel::SetTarget(Instance);

		NxFr::Dictionary<NxFr::StringId, GUI::Style>* Styles = GetStyles();
		if (Styles->ContainsKey("Style1"_Sid))
		{
			return;
		}

		GUI::Style Style1;
		Style1.AppendColor(ImGuiCol_Button, NxFr::Color(1.0f, 0.5f, 0.0f, 1.0f));
		Style1.AppendVar(ImGuiStyleVar_FrameRounding, 5.0f);
		Style1.AppendVarXY(ImGuiStyleVar_ButtonTextAlign, NxFr::Vector2f(0.5f, 0.5f));
		Styles->Append("Style1"_Sid, Style1);

		GUI::Style Style2;
		Style2.AppendColor(ImGuiCol_Button, NxFr::Color(0.0f, 1.0f, 0.5f, 1.0f));
		Style2.AppendVarXY(ImGuiStyleVar_ButtonTextAlign, NxFr::Vector2f(0.0f, 0.5f));
		Styles->Append("Style2"_Sid, Style2);

		GUI::Style Style3;
		Style3.AppendColor(ImGuiCol_Button, NxFr::Color(0.5f, 0.0f, 1.0f, 1.0f));
		Style3.AppendVarXY(ImGuiStyleVar_ButtonTextAlign, NxFr::Vector2f(1.0f, 0.0f));
		Styles->Append("Style3"_Sid, Style3);
	}

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
