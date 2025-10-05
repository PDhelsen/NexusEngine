#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/StylesPanel.h"

namespace NxEn
{
	static NxFr::StringId WidthLabel = "WidthLabel"_Sid;
	static NxFr::StringId WidthButton = "WidthButton"_Sid;
	static NxFr::StringId Text_Title = "Text_Title"_Sid;

	static StylesPanel* Panel = GUI::Panel::Create<StylesPanel>();

	NEXUS_OBJECT_IMPLEMENTATION(StylesPanel)

	void StylesPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Styles");

		Menu.AddMenuItem("Reload", []() { Application::GetSystem<GUISystem>()->LoadTheme(); });
	}

	void StylesPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void StylesPanel::OnEnable()
	{
		Panel::OnEnable();
	}

	void StylesPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		{
			GUI::Style::Scope Style(Text_Title);
			ImGui::Text("Vars");
		}

		for (auto& [Id, Var] : GUI::Style::GetVars())
		{
			const NxFr::String& Label = Id.GetString();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(Label.C());
			ImGui::SameLine();

			ImGui::SetCursorPosX(GUI::Style::GetVar(WidthLabel) + ImGui::GetStyle().ItemSpacing.x);
			ImGui::Text("%f", Var);
		}

		ImGui::Dummy({ 0, ImGui::GetTextLineHeight() });

		{
			GUI::Style::Scope Style(Text_Title);
			ImGui::Text("Colors");
		}

		for (auto& [Id, Color] : GUI::Style::GetColors())
		{
			const NxFr::String& Label = Id.GetString();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(Label.C());
			ImGui::SameLine();

			ImGui::SetCursorPosX(GUI::Style::GetVar(WidthLabel) + ImGui::GetStyle().ItemSpacing.x);
			ImGui::PushStyleColor(ImGuiCol_Button, Color);
			ImGui::Button((Label + "##" + Id.C()).C(), { GUI::Style::GetVar(WidthButton), 0 });
			ImGui::PopStyleColor();
		}

		ImGui::Dummy({ 0, ImGui::GetTextLineHeight() });

		{
			GUI::Style::Scope Style(Text_Title);
			ImGui::Text("Styles");
		}

		for (auto& [Id, Style] : GUI::Style::GetStyles())
		{
			GUI::Style::Scope S(Id);

			const NxFr::String& Label = Id.GetString();

			ImGui::AlignTextToFramePadding(); 
			ImGui::Text(Label.C());
			ImGui::SameLine();

			ImGui::SetCursorPosX(GUI::Style::GetVar(WidthLabel) + ImGui::GetStyle().ItemSpacing.x);
			ImGui::Button((Label + "##" + Id.C()).C(), { GUI::Style::GetVar(WidthButton), 0 });
		}
	}
}
