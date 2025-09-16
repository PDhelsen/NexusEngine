#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsPanel.h"

namespace NxEn
{
	static SettingsPanel* Panel = GUI::Panel::Create<SettingsPanel>();

	const static GUI::Menu::Item MenuItemSettings = GUI::Menu::Item::Create("File/Settings", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("GUI.Panel,SettingsPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(SettingsPanel)

	SettingsPanel::SettingsPanel()
		: Menu(), Page(0), Pages(), Names(), Values()
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

		Menu.AddMenuItem("Apply", { this, &SettingsPanel::Apply });
		Menu.AddMenuItem("Load", { this, &SettingsPanel::Load });
		Menu.AddMenuItem("Save", { this, &SettingsPanel::Save });
	}

	void SettingsPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void SettingsPanel::OnEnable()
	{
		Panel::OnEnable();
		Fetch();
	}

	void SettingsPanel::OnDisable()
	{
		Panel::OnDisable();
	}

	void SettingsPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		if (Pages.GetCount() == 0)
		{
			return;
		}

		ImGui::BeginChild("Pages", { Indent, 0.0f }, true);
		for (uint64 Index = 0; Index < Pages.GetCount(); ++Index)
		{
			if (ImGui::Selectable(Pages[Index].C(), Page == Index))
			{
				Page = Index;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Settings", { 0.0f, 0.0f }, false);
		NxFr::Array<NxFr::String>& SettingsNames = Names[Page];
		NxFr::Array<Setting*>& SettingsValues = Values[Page];
		for (uint64 Index = 0; Index < SettingsNames.GetCount(); ++Index)
		{
			NxFr::String& Name = SettingsNames[Index];
			Setting* Value = SettingsValues[Index];

			ImGui::AlignTextToFramePadding();
			ImGui::Text(Name.C());
			ImGui::SameLine(Indent);
			Value->OnGui();
		}
		ImGui::EndChild();
	}

	void SettingsPanel::Fetch()
	{
		auto Settings = SettingsSystem::GetAllSettings();

		uint64 PageIndex = 0;
		uint64 PageCount = Settings.GetCount();

		Pages = NxFr::Array<NxFr::String>(PageCount);
		Names = NxFr::Array<NxFr::Array<NxFr::String>>(PageCount);
		Values = NxFr::Array<NxFr::Array<Setting*>>(PageCount);

		for (auto& [Page, Instances] : Settings)
		{
			uint64 NameIndex = 0;
			uint64 NameCount = Instances.GetCount();

			Pages[PageIndex] = Page.ToString();
			Names[PageIndex] = NxFr::Array<NxFr::String>(NameCount);
			Values[PageIndex] = NxFr::Array<Setting*>(NameCount);

			NxFr::Array<NxFr::String>& PageName = Names[PageIndex];
			NxFr::Array<Setting*>& PageValue = Values[PageIndex];

			for (auto& [Name, Instance] : Instances)
			{
				PageName[NameIndex] = Name.ToString();
				PageValue[NameIndex] = Instance;

				NameIndex++;
			}

			PageIndex++;
		}
	}

	void SettingsPanel::Apply()
	{
		Application::GetSystem<SettingsSystem>()->GetOnChange().Invoke();
	}

	void SettingsPanel::Load()
	{
		Application::GetSystem<SettingsSystem>()->LoadSettings();
	}

	void SettingsPanel::Save()
	{
		Application::GetSystem<SettingsSystem>()->SaveSettings();
	}
}
