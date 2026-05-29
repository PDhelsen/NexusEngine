#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsPanel.h"

namespace NxEn
{
	static SettingsPanel* Panel = GUI::Panel::Create<SettingsPanel>();

	const static GUI::Menu::Item MenuItemSettings = GUI::Menu::Item::Create("File/Settings", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("GUI.Panel SettingsPanel");
	}));

	SettingsPanel::SettingsPanel()
		: Menu(), Style(), Settings(), Page(0)
	{
	}

	SettingsPanel::~SettingsPanel()
	{
	}

	void SettingsPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Settings");

		Menu.AddMenuItem("Load", []() { Application::GetSystem<SettingsSystem>()->LoadSettings(); });
		Menu.AddMenuItem("Save", []() { Application::GetSystem<SettingsSystem>()->SaveSettings(); });
		Menu.AddMenuItem("Apply", []() { Application::GetSystem<SettingsSystem>()->ApplySettings(); });
	}

	void SettingsPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void SettingsPanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.SetEnabled(true);
		Style.Reset();

		Settings = Application::GetSystem<SettingsSystem>()->GetAllSettingsSorted();
	}

	void SettingsPanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void SettingsPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		ImGui::BeginChild("Pages", { GUI::Style::GetVar(GUI::Style::IdWidthLabel), 0.0f}, true);
		for (uint64 Index = 0; Index < Settings.GetCount(); ++Index)
		{
			NxFr::StringView Label = Settings[Index][0]->GetPage();
			if (ImGui::Selectable(Label.C(true), Page == Index))
			{
				Page = Index;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Settings", { 0.0f, 0.0f }, false);
		for (uint64 Index = 0; Index < Settings[Page].GetCount(); ++Index)
		{
			Settings[Page][Index]->OnGui(&Style);
		}
		ImGui::EndChild();
	}
}
