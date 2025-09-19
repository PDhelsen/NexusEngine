#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/StylesPanel.h"

namespace NxEn
{
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
		static float ButtonWidth = 150.0f;
		static float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

		Menu.Tick(TimeStep);

		ImGui::Text("Styles");
		for (auto& [Id, Style] : GUI::Styles::Styles)
		{
			GUI::Scope::Style S(Style);

			NxFr::String Label = Id.ToString();

			ImGui::AlignTextToFramePadding(); 
			ImGui::Text(Label.C());
			ImGui::SameLine();

			ImGui::SetCursorPosX(ButtonWidth + ItemSpacing);
			ImGui::Button((Label + "##" + Id.C()).C(), { ButtonWidth, 0 });
		}

		ImGui::Dummy({ 0, 20 });

		ImGui::Text("Colors");
		for (auto& [Id, Color] : GUI::Styles::Colors)
		{
			GUI::Scope::Color C(ImGuiCol_Button, Color);

			NxFr::String Label = Id.ToString();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(Label.C());
			ImGui::SameLine();

			ImGui::SetCursorPosX(ButtonWidth + ItemSpacing);
			ImGui::Button((Label + "##" + Id.C()).C(), { ButtonWidth, 0 });
		}

		ImGui::Dummy({ 0, 20 });

		ImGui::Text("Vars");
		for (auto& [Id, Var] : GUI::Styles::Vars)
		{
			NxFr::String Label = Id.ToString();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(Label.C());
			ImGui::SameLine();

			ImGui::SetCursorPosX(ButtonWidth + ItemSpacing);
			ImGui::Text("%f", Var);
		}
	}
}
