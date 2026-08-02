#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/StylesPanel.h"

namespace NxEn
{
	static StylesPanel* Panel = GUI::Panel::Create<StylesPanel>();

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

		Style.Size.x = GUI::Styles::WidthButton();
	}

	void StylesPanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void StylesPanel::OnDraw()
	{
		Menu.Draw();

		Style.ColorText = NxFr::Colors::White;
		GUI::Draw::Text("Vars", &GUI::Styles::TextTitle());
		for (auto It = GUI::Style::GetVars().Begin(); It != GUI::Style::GetVars().End(); ++It)
		{
			GUI::Drawer<float>::Property(It->Value, It->Key, &Style);
		}

		ImGui::Dummy({ 0, ImGui::GetTextLineHeight() });

		Style.ColorText = NxFr::Colors::White;
		GUI::Draw::Text("Colors", &GUI::Styles::TextTitle());
		for (auto It = GUI::Style::GetColors().Begin(); It != GUI::Style::GetColors().End(); ++It)
		{
			Style.ColorText = It->Value;
			GUI::Draw::Label(It->Key, &Style);
			Style.Color = It->Value;
			Style.ColorText = NxFr::Colors::White;
			GUI::Draw::Button(It->Key, &Style);
		}

		ImGui::Dummy({ 0, ImGui::GetTextLineHeight() });

		Style.ColorText = NxFr::Colors::White;
		GUI::Draw::Text("Styles", &GUI::Styles::TextTitle());
		for (auto It = GUI::Style::GetStyles().Begin(); It != GUI::Style::GetStyles().End(); ++It)
		{
			GUI::Style Visual = It->Value;
			Visual.Size.x = GUI::Styles::WidthButton();
			Visual.Label = GUI::Styles::WidthLabel();

			GUI::Draw::Label(It->Key, &Visual);
			GUI::Draw::Button(It->Key, &Visual);
		}
	}
}
