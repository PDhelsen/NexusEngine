#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsPanel.h"

namespace NxEn
{
	static NxFr::StringId WidthLabel = "WidthLabel"_Sid;

	static SettingsPanel* Panel = GUI::Panel::Create<SettingsPanel>();

	const static GUI::Menu::Item MenuItemSettings = GUI::Menu::Item::Create("File/Settings", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("GUI.Panel,SettingsPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(SettingsPanel)

	SettingsPanel::SettingsPanel()
		: Settings(), Menu(), Page(0)
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
		Settings = Application::GetSystem<SettingsSystem>()->GetAllSettingsSorted();
	}

	void SettingsPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		ImGui::BeginChild("Pages", { GUI::Style::GetVar(WidthLabel), 0.0f}, true);
		for (uint64 Index = 0; Index < Settings.GetCount(); ++Index)
		{
			NxFr::String Label = Settings[Index][0]->GetPage().ToString();
			if (ImGui::Selectable(Label.C(), Page == Index))
			{
				Page = Index;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Settings", { 0.0f, 0.0f }, false);
		NxFr::Array<Setting*>& Values = Settings[Page];
		for (uint64 Index = 0; Index < Values.GetCount(); ++Index)
		{
			Setting* Value = Values[Index];
			NxFr::StringView Name = Value->GetName();

			ImGui::AlignTextToFramePadding();
			ImGui::Text(Name.C());
			ImGui::SameLine(GUI::Style::GetVar(WidthLabel));
			Value->OnGui();
		}
		ImGui::EndChild();
	}
}
