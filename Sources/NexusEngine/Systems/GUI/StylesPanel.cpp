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
		Menu.SetEnabled(true);

		Style.Reset();
		Style.Width = GUI::Style::GetVar(GUI::Style::IdWidthButton);
		Style.WidthLabel = -1.0f;
	}

	void StylesPanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void StylesPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		Style.ColorText = NxFr::Colors::White;
		GUI::Draw::Text("Vars", &GUI::Style::GetStyle(GUI::Style::IdText_Title));
		for (auto& [Id, DataVar] : GUI::Style::GetVars())
		{
			GUI::Drawer<float>::Property(DataVar, Id, &Style);
		}

		ImGui::Dummy({ 0, ImGui::GetTextLineHeight() });

		Style.ColorText = NxFr::Colors::White;
		GUI::Draw::Text("Colors", &GUI::Style::GetStyle(GUI::Style::IdText_Title));
		for (auto& [Id, DataColor] : GUI::Style::GetColors())
		{
			Style.ColorText = DataColor;
			GUI::Draw::Label(Id, &Style);
			Style.Color = DataColor;
			Style.ColorText = NxFr::Colors::White;
			GUI::Draw::Button(Id, &Style);
		}

		ImGui::Dummy({ 0, ImGui::GetTextLineHeight() });

		Style.ColorText = NxFr::Colors::White;
		GUI::Draw::Text("Styles", &GUI::Style::GetStyle(GUI::Style::IdText_Title));
		for (auto& [Id, DataStyle] : GUI::Style::GetStyles())
		{
			GUI::Style Visual = GUI::Style::Copy(&DataStyle);
			Visual.Width = GUI::Style::GetVar(GUI::Style::IdWidthButton);
			Visual.WidthLabel = -1.0f;

			GUI::Draw::Label(Id, &Visual);
			GUI::Draw::Button(Id, &Visual);
		}
	}
}
