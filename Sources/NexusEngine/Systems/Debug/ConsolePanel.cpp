#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"

namespace NxEn
{
	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	static SettingVar<bool>* SettingConsoleAutoScroll = SettingVar<bool>::Create("Settings", "ConsoleAutoScroll", true);

	NX_OBJECT_IMPLEMENTATION(ConsolePanel)

	NxFr::StringId ConsolePanel::GetStyle(NxFr::LoggerVerbosity Verbosity)
	{
		NxFr::StringId Id = 0;

		switch (Verbosity)
		{
		case NxFr::LoggerVerbosity::Info: Id = GUI::Style::IdInfo; break;
		case NxFr::LoggerVerbosity::Warning: Id = GUI::Style::IdWarning; break;
		case NxFr::LoggerVerbosity::Error: Id = GUI::Style::IdError; break;
		case NxFr::LoggerVerbosity::Fatal: Id = GUI::Style::IdFatal; break;

		case NxFr::LoggerVerbosity::None:
		case NxFr::LoggerVerbosity::All:
		case NxFr::LoggerVerbosity::COUNT:
		default: Id = 0; break;
		}

		return Id;
	}

	ConsolePanel::ConsolePanel()
		: Menu(), Style(), Logs(), FlagsVerbosity(), FlagsChannels(), Command(128), Search(128), Scroll(false)
	{
	}

	ConsolePanel::~ConsolePanel()
	{
	}

	void ConsolePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Console");
	}

	void ConsolePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.SetEnabled(true);
		Style.Reset();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->RegisterCallback({ this, &ConsolePanel::AddLogs });

		uint8 VerbosityCount = NxFr::Enum::FlagIndex(NxFr::LoggerVerbosity::COUNT);
		FlagsVerbosity.Reserve(VerbosityCount);
		for (uint64 Index = 0; Index < VerbosityCount; ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount();
			NxFr::LoggerVerbosity Verbosity = (NxFr::LoggerVerbosity)((uint64)1 << Index);
			FlagsVerbosity.Append(Verbosity, Logger->CheckVerbosity(Verbosity));

			NxFr::String Path = NxFr::StringView("Verbosity/") + NxFr::StringUtility::ToString(Verbosity);
			Menu.AddMenuToggle(Path, &FlagsVerbosity[Verbosity], nullptr, Priority);
		}

		NxFr::Array<NxFr::StringId> Channels = Logger->GetChannels();
		NxFr::ContainerUtility::Sort<NxFr::StringId>(Channels, [](const NxFr::StringId& A, const NxFr::StringId& B) { return A.C() <= B.C(); });
		FlagsChannels.Reserve(Channels.GetCount());
		for (uint64 Index = 0; Index < Channels.GetCount(); ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount() + FlagsChannels.GetCount();
			NxFr::StringId Id = Channels[Index];
			FlagsChannels.Append(Id, Logger->CheckChannel(Id));

			NxFr::String Path = NxFr::StringView("Channels/") + Id.C();
			Menu.AddMenuToggle(Path, &FlagsChannels[Id], nullptr, Priority);
		}

		Menu.AddMenuToggle("Settings/AutoScroll", &SettingConsoleAutoScroll->GetValue());
	}

	void ConsolePanel::OnDisable()
	{
		FlagsVerbosity.Clear();
		FlagsChannels.Clear();
		Menu.Clear();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->UnregisterCallback({ this, &ConsolePanel::AddLogs });

		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void ConsolePanel::OnGui(float TimeStep)
	{
		// Menu
		{
			Menu.Tick(TimeStep);

			if (ImGui::BeginMenuBar())
			{
				Style.Position.x = GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Search:").x + GUI::Style::GetVar(GUI::Style::IdWidthInpuText) + GUI::Style::GetVar(GUI::Style::IdWidthButton)), 2, false, true).x;
				Style.Width = GUI::Style::GetVar(GUI::Style::IdWidthInpuText);
				GUI::Drawer<NxFr::String>::Field(Search, "Search", "", &Style);

				if (ImGui::Button("Clear Logs", { GUI::Style::GetVar(GUI::Style::IdWidthButton), 0.0f }))
				{
					ClearLogs();
				}
			}
			ImGui::EndMenuBar();
		}

		// Logs
		{
			ImGui::BeginChild("Logs", { 0, GUI::Utils::Fill(NxFr::Vector2f(ImGui::GetTextLineHeightWithSpacing()), 2).y }, 0, ImGuiWindowFlags_HorizontalScrollbar);

			for (uint64 Index = 0; Index < Logs.GetCount(); ++Index)
			{
				Log& Log = Logs[Index];
				if (Log.Verbosity && Log.Channel && (Search.IsEmpty() || NxFr::StringUtility::Contains(Log.Text, Search)))
				{
					GUI::Drawer<NxFr::String>::Property(Log.Text, "", &GUI::Style::GetStyle(Log.Style));
				}
			}

			if (SettingConsoleAutoScroll->GetValue() && Scroll)
			{
				ImGui::SetScrollHereY(1.0f);
				Scroll = false;
			}

			ImGui::EndChild();
		}

		ImGui::Separator();

		// Command
		{
			Style.Position.x = -1.0f;
			Style.WidthLabel = 0.0f;
			Style.Width = GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Command:").x + GUI::Style::GetVar(GUI::Style::IdWidthButton)), 1, false).x;
			if (GUI::Drawer<NxFr::String>::Field(Command, "Command", "", &Style))
			{
				ExecuteCommand();
			}

			ImGui::SameLine();

			if (ImGui::Button("Execute", { GUI::Style::GetVar(GUI::Style::IdWidthButton), 0.0f }))
			{
				ExecuteCommand();
			}
		}
	}

	void ConsolePanel::ExecuteCommand()
	{
		Command.Validate();

		Application::GetSystem<CommandsSystem>()->Run(Command);

		Command.Clear();
	}

	void ConsolePanel::AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message)
	{
		Logs.AppendConstruct(Message, GetStyle(Verbosity), FlagsVerbosity[Verbosity], FlagsChannels[Channel]);
		Scroll = true;
	}

	void ConsolePanel::ClearLogs()
	{
		Logs.Clear();
	}
}
