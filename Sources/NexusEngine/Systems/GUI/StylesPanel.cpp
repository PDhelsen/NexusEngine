#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/StylesPanel.h"

namespace NxEn
{
	static StylesPanel* Panel = GUI::Panel::Create<StylesPanel>();

	StylesPanel::StylesPanel()
		: Menu()
	{
	}

	StylesPanel::~StylesPanel()
	{
	}

	void StylesPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetNameId("Styles");
		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);

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
		Menu.Show();
	}

	void StylesPanel::OnDisable()
	{
		Menu.Hide();
		Panel::OnDisable();
	}

	void StylesPanel::OnDraw()
	{
		GUI::Transform Visual = GUI::Transform(-NxFr::Vector2f::One, NxFr::Vector2f(GUI::Styles::WidthButton(), 0), GUI::Styles::WidthLabel());
		GUI::Style Style = GUI::Styles::Default();
		const GUI::Style* Title = &GUI::Styles::TextTitle();

		Menu.Draw();

		GUI::Utils::PushStyle(Title);
		GUI::Draw::Text("Vars");
		GUI::Utils::PopStyle(Title);
		for (auto It = GUI::Style::GetVars().Begin(); It != GUI::Style::GetVars().End(); ++It)
		{
			GUI::Drawer<float>::Property(It->Value, It->Key, Visual);
		}

		GUI::Draw::Space();

		GUI::Utils::PushStyle(Title);
		GUI::Draw::Text("Colors");
		GUI::Utils::PopStyle(Title);
		for (auto It = GUI::Style::GetColors().Begin(); It != GUI::Style::GetColors().End(); ++It)
		{
			Style.ColorText = It->Value;
			GUI::Utils::PushStyle(&Style);
			GUI::Draw::Label(It->Key, Visual);
			GUI::Utils::PopStyle(&Style);

			Style.Color = It->Value;
			Style.ColorText = NxFr::Colors::White;
			GUI::Utils::PushStyle(&Style);
			GUI::Draw::Button(It->Key, Visual);
			GUI::Utils::PopStyle(&Style);
		}

		GUI::Draw::Space();

		GUI::Utils::PushStyle(Title);
		GUI::Draw::Text("Styles");
		GUI::Utils::PopStyle(Title);
		for (auto It = GUI::Style::GetStyles().Begin(); It != GUI::Style::GetStyles().End(); ++It)
		{
			GUI::Utils::PushStyle(&It->Value);
			GUI::Draw::Label(It->Key, Visual);
			GUI::Draw::Button(It->Key, Visual);
			GUI::Utils::PopStyle(&It->Value);
		}
	}
}
