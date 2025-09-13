#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsPanel.h"

namespace NxEn
{
	static const float Width = 100.0f;
	static const uint64 Buffer = 128;

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
		FetchValues();
	}

	void SettingsPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		if (Pages.GetCount() == 0)
		{
			return;
		}

		ImGui::BeginChild("Pages", { Width, 0.0f }, true);
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
		NxFr::Array<NxFr::String>& SettingsValues = Values[Page];
		for (uint64 Index = 0; Index < SettingsNames.GetCount(); ++Index)
		{
			NxFr::String& Name = SettingsNames[Index];
			NxFr::String& Value = SettingsValues[Index];

			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text(Name.C());
			}

			ImGui::SameLine(Width);

			{
				NxFr::String ImGuiId = "##" + Name;
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::InputText(ImGuiId.C(), Value.C_Buffer(), Value.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					ApplyValue(Page, Index);
				}
			}
		}
		ImGui::EndChild();
	}

	void SettingsPanel::FetchValues()
	{
		auto Settings = SettingsSystem::GetAllSettings();

		uint64 PageIndex = 0;
		uint64 PageCount = Settings.GetCount();

		Pages = NxFr::Array<NxFr::String>(PageCount);
		Names = NxFr::Array<NxFr::Array<NxFr::String>>(PageCount);
		Values = NxFr::Array<NxFr::Array<NxFr::String>>(PageCount);

		for (auto& [Page, Instances] : Settings)
		{
			uint64 NameIndex = 0;
			uint64 NameCount = Instances.GetCount();

			Pages[PageIndex] = Page.ToString();
			Names[PageIndex] = NxFr::Array<NxFr::String>(NameCount);
			Values[PageIndex] = NxFr::Array<NxFr::String>(NameCount);

			NxFr::Array<NxFr::String>& PageName = Names[PageIndex];
			NxFr::Array<NxFr::String>& PageValue = Values[PageIndex];

			for (auto& [Name, Instance] : Instances)
			{
				PageName[NameIndex] = Name.ToString();
				PageValue[NameIndex] = Instance->Get();
				PageValue[NameIndex].Grow(Buffer);

				NameIndex++;
			}

			PageIndex++;
		}
	}

	void SettingsPanel::ApplyValues()
	{
		for (uint64 PageIndex = 0; PageIndex < Pages.GetCount(); ++PageIndex)
		{
			for (uint64 SettingIndex = 0; SettingIndex < Names[PageIndex].GetCount(); ++SettingIndex)
			{
				ApplyValue(PageIndex, SettingIndex);
			}
		}
	}

	void SettingsPanel::ApplyValue(uint64 PageIndex, uint64 SettingIndex)
	{
		NxFr::String& Page = Pages[PageIndex];
		NxFr::String& Name = Names[PageIndex][SettingIndex];
		NxFr::String& Value = Values[PageIndex][SettingIndex];

		Value.Validate();
		NxFr::String Key = Setting::Key(Page, Name);
		SettingsSystem::GetSetting(Key)->Set(Value);
	}

	void SettingsPanel::Load()
	{
		Application::GetSystem<SettingsSystem>()->LoadSettings();
		FetchValues();
	}

	void SettingsPanel::Save()
	{
		ApplyValues();
		Application::GetSystem<SettingsSystem>()->SaveSettings();
	}
}
