#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static NxFr::StringId GetStyle(NxFr::LoggerVerbosity Verbosity)
	{
		NxFr::StringId Id = 0;

		switch (Verbosity)
		{
		case NxFr::LoggerVerbosity::Info:
		case NxFr::LoggerVerbosity::Warning:
		case NxFr::LoggerVerbosity::Error:;
		case NxFr::LoggerVerbosity::Fatal:
			Id = NxFr::StringId(NxFr::StringUtility::ToString(Verbosity));
			break;

		case NxFr::LoggerVerbosity::None:
		case NxFr::LoggerVerbosity::All:
		case NxFr::LoggerVerbosity::COUNT:
		default: Id = 0; break;
		}

		return Id;
	}

	static SettingVar<bool>* SettingConsoleAutoScroll = SettingVar<bool>::Create("Settings", "ConsoleAutoScroll", true);

	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	ConsolePanel::ConsolePanel()
		: Menu(), Logs(), FlagsVerbosity(), FlagsChannels(), Command(128), Search(128), Scroll(false)
	{
	}

	ConsolePanel::~ConsolePanel()
	{
	}

	void ConsolePanel::Clear()
	{
		ClearLogs();
	}

	void ConsolePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);
		SetNameId("Console");

		Menu.AddMenuToggle("Settings/AutoScroll", &SettingConsoleAutoScroll->GetValue());
	}

	void ConsolePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.Show();

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
	}

	void ConsolePanel::OnDisable()
	{
		FlagsVerbosity.Clear();
		FlagsChannels.Clear();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->UnregisterCallback({ this, &ConsolePanel::AddLogs });

		Menu.Hide();
		Panel::OnDisable();
	}

	void ConsolePanel::OnDraw()
	{
		NxEn::GUI::Transform Visual;

		// Menu
		{
			Menu.Draw();

			Visual.Position.x = GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Search:").x + GUI::Styles::WidthInpuText() + GUI::Styles::WidthButton())).x;
			Visual.Size.x = GUI::Styles::WidthInpuText();
			Visual.Label = -1.0f;

			if (ImGui::BeginMenuBar())
			{
				GUI::Drawer<NxFr::String>::Field(Search, "Search", "", Visual);

				if (GUI::Draw::Button("Clear Logs", NxFr::Vector2f(GUI::Styles::WidthButton(), 0.0f)))
				{
					ClearLogs();
				}
			}
			ImGui::EndMenuBar();
		}

		// Logs
		{
			Visual.Position = -NxFr::Vector2f::One;
			Visual.Size = NxFr::Vector2f::Zero;
			Visual.Label = -1.0f;

			ImGui::BeginChild("Logs", { 0, GUI::Utils::Fill(NxFr::Vector2f(ImGui::GetTextLineHeight()), 3).y }, 0, ImGuiWindowFlags_HorizontalScrollbar);

			for (uint64 Index = 0; Index < Logs.GetCount(); ++Index)
			{
				Log& Log = Logs[Index];
				if (Log.Verbosity && Log.Channel && (Search.IsEmpty() || NxFr::StringUtility::Contains(Log.Text, Search)))
				{
					GUI::Style::Scope Color = GUI::Style::GetStyles().TryGet(Log.Style);

					GUI::Draw::TextSelectable(Log.Text, NxFr::StringUtility::ToString(Index));
				}
			}

			if (SettingConsoleAutoScroll->GetValue() && Scroll)
			{
				ImGui::SetScrollHereY(1.0f);
				Scroll = false;
			}

			ImGui::EndChild();
		}

		GUI::Draw::Separator();

		// Command
		{
			Visual.Position.x = -1.0f;
			Visual.Size.x = GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Command:").x + GUI::Styles::WidthButton())).x;
			Visual.Label = -1.0f;

			if (GUI::Drawer<NxFr::String>::Field(Command, "Command", "", Visual))
			{
				ExecuteCommand();
			}

			GUI::Utils::SameLine();

			if (GUI::Draw::Button("Execute", NxFr::Vector2f(GUI::Styles::WidthButton(), 0.0f)))
			{
				ExecuteCommand();
			}
		}
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

	void ConsolePanel::ExecuteCommand()
	{
		Command.Validate();

		Application::GetSystem<CommandsSystem>()->Run(Command);

		Command.Clear();
	}
}
